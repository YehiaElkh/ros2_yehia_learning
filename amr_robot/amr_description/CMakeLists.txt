# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.8)

# Nom du projet — doit correspondre au nom dans package.xml
project(amr_description)

# --------------------------------------------------------
# Options de compilation pour C++
# --------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
  # -Wall    : activer tous les warnings courants
  # -Wextra  : warnings supplémentaires
  # -Wpedantic : respect strict du standard C++
endif()

# --------------------------------------------------------
# DÉPENDANCES
# --------------------------------------------------------
find_package(ament_cmake REQUIRED)
# ament_cmake : système de build ROS2 basé sur CMake
# REQUIRED : erreur si non trouvé

find_package(urdf REQUIRED)
# urdf : bibliothèque pour parser les fichiers URDF

find_package(xacro REQUIRED)
# xacro : outil de conversion xacro → URDF

# --------------------------------------------------------
# INSTALLATION DES RESSOURCES
# --------------------------------------------------------
# Pour amr_description, on n'a pas de code C++ à compiler
# On installe juste les fichiers de ressources (URDF, launch, etc.)

install(DIRECTORY
  urdf      # dossier contenant robot.urdf.xacro
  launch    # dossier contenant display.launch.py
  rviz      # dossier contenant les configs RViz
  DESTINATION share/${PROJECT_NAME}
  # DESTINATION : où installer dans le workspace
  # share/amr_description/ → accessible via get_package_share_directory()
)

# --------------------------------------------------------
# FINALISER LE PACKAGE
# --------------------------------------------------------
ament_package()
# Macro obligatoire — génère les fichiers de config CMake
# Doit toujours être la DERNIÈRE ligne du CMakeLists.txt