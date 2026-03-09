from setuptools import setup

package_name = 'vehicle_plugins_py'

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
    description='Concrete vehicle plugin implementations in Python',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [],

        # --------------------------------------------------------
        # ENREGISTRER LES PLUGINS
        # --------------------------------------------------------
        # groupe : vehicle_base_py.plugins (même que dans vehicle_loader.py)
        # nom    : vehicle_plugins_py.Motorbike (nom du plugin)
        # valeur : vehicle_plugins_py.motorbike:Motorbike (module:classe)
        'vehicle_base_py.plugins': [
            'vehicle_plugins_py.Motorbike = vehicle_plugins_py.motorbike:Motorbike',
            'vehicle_plugins_py.Bicycle   = vehicle_plugins_py.bicycle:Bicycle',
            'vehicle_plugins_py.Truck     = vehicle_plugins_py.truck:Truck',
        ],
    },
)