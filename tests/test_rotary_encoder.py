import unittest

from rotary_encoder_driver.driver import RotaryEncoderDriver


class RotaryEncoderTest(unittest.TestCase):

    def test_parse_data(self):
        test_data = "Enc: 1 Direction: CCW  Tick: 0\nEnc: 1 Direction: CW  Tick: 0\nEnc: 1 Direction: CW  Tick: 1\nEnc: 0 Direction: CW  Tick: 0"

        parsed_data = []
        for line in test_data.split("\n"):
            parsed_data.append(RotaryEncoderDriver.parse_data(line))

        self.assertIn((0, "CCW", 0), parsed_data)


if __name__ == '__main__':
    unittest.main()
