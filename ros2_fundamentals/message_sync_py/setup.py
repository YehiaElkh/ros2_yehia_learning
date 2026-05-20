from setuptools import setup

package_name = 'message_sync_py'

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
    description='Message synchronization examples in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'multi_publisher  = message_sync_py.multi_publisher:main',
            'sync_subscriber  = message_sync_py.sync_subscriber:main',
        ],
    },
)