from setuptools import setup

package_name = 'action_tutorial_py'

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
    description='Action tutorial examples in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'action_server = action_tutorial_py.action_server:main',
            'action_client = action_tutorial_py.action_client:main',
        ],
    },
)