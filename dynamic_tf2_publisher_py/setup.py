from setuptools import setup

package_name = 'dynamic_tf2_publisher_py'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Yehia Elkh',
    maintainer_email='yehiaelkh@gmail.com',
    description='Dynamic TF2 publisher in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'dynamic_tf2_publisher = dynamic_tf2_publisher_py.dynamic_tf2_publisher:main',
        ],
    },
)