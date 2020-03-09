import re
import sys

import serial
import rclpy

from rclpy.node import Node
from std_msgs.msg import Int32


class RotaryEncoderDriver(Node):
    def __init__(self):
        super().__init__('rotary_encoder_driver')
        self.left_tick = self.create_publisher(Int32, 'wheel_left_tick', 10)
        self.right_tick = self.create_publisher(Int32, 'wheel_right_tick', 10)

        self.port = self.declare_parameter('port', '/dev/tty.usbmodem141301').value
        self.baudrate = self.declare_parameter('baud', 115200).value

        try:
            self.encoder_serial_reader = serial.Serial(port=self.port, baudrate=self.baudrate, timeout=2)
        except serial.SerialException as e:
            self.get_logger().fatal(f"Serial communication failed {e}")
            sys.exit()

        while rclpy.ok():
            data = self.read_data()
            try:
                sensor_id, tick = RotaryEncoderDriver.parse_data(data)
            except Exception as e:
                self.get_logger().warning(f"Data parsing failed for data: {data}, Error: {e}")
                continue

            msg = self.to_msg(tick)

            if sensor_id == "Left":
                self.left_tick.publish(msg)
            else:
                self.right_tick.publish(msg)

    def read_data(self):
        data = self.range_serial_reader.readline().strip()
        if isinstance(data, bytes):
            data = data.decode("utf-8")
        return data

    @staticmethod
    def parse_data(data):
        if data:
            parts = data.split(" ")
            if len(parts) != 2:
                # self.get_logger().warning("Could not parse sensor data")
                raise Exception("Incorrect data format")

            tick = float(parts[1])

            id_regex = r'Left|Right'
            sensor_id = re.findall(id_regex, parts[0])[0]

            return sensor_id, tick

        raise Exception("Data empty")

    def to_msg(self, data):
        msg = Int32()
        msg.data = data
        return msg


def main(args=None):
    rclpy.init(args=args)
    driver = RotaryEncoderDriver()

    rclpy.spin(driver)

    driver.destroy_node()

    rclpy.shutdown()


if __name__ == '__main__':
    main()