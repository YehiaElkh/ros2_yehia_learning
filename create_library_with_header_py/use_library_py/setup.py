from setuptools import setup

package_name = 'use_library_py'

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
    description='Uses the publisher library in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'use_library = use_library_py.use_library:main',
        ],
    },
)