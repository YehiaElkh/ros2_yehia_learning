from setuptools import setup

package_name = 'publisher_and_subscriber_py'

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
    description='Publisher and Subscriber examples in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'simple_publisher = publisher_and_subscriber_py.simple_publisher:main',
            'simple_subscriber = publisher_and_subscriber_py.simple_subscriber:main',
            'publisher_with_custom_msg = publisher_and_subscriber_py.publisher_with_custom_msg:main',
            'subscriber_with_custom_msg = publisher_and_subscriber_py.subscriber_with_custom_msg:main',
        ],
    },
)