import os
from glob import glob

from setuptools import setup

PACKAGE_NAME = 'rotary_encoder_driver'
SHARE_DIR = os.path.join("share", PACKAGE_NAME)

setup(
    name=PACKAGE_NAME,
    version='0.0.1',
    packages=["rotary_encoder_driver"],
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + PACKAGE_NAME]),
        ('share/' + PACKAGE_NAME, ['package.xml']),
        (os.path.join(SHARE_DIR, "launch"), glob(os.path.join("launch", "*.launch.py"))),
        (os.path.join(SHARE_DIR, "config"), glob(os.path.join("config", "*.yaml"))),
    ],
    package_dir={'': 'src', },
    py_modules=[],
    zip_safe=True,
    install_requires=['setuptools',
                      'pyserial'
                      ],
    author='Andreas Klintberg',
    author_email='andreas.klintberg@gmail.com',
    description='ROS2 rotary encoder driver.',
    license='MIT',
    entry_points={
        'console_scripts': [
            'rotary_encoder_node = rotary_encoder_driver.driver:main',
        ],
    },
)
