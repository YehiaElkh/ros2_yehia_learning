from setuptools import setup
import os
from glob import glob

package_name = 'amr_controller'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    # Inclure le module Python amr_controller/
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        # Installer les launch files
        (os.path.join('share', package_name, 'launch'),
            glob('launch/*.py')),
        # Installer les fichiers de config
        (os.path.join('share', package_name, 'config'),
            glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    entry_points={
        'console_scripts': [
            'mission_planner = amr_controller.mission_planner:main',
        ],
    },
)
