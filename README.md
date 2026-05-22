# 🤖 ros2_yehia_learning

> **A complete collection of ROS 2 Humble examples in C++ and Python — built from scratch**

[![ROS2](https://img.shields.io/badge/ROS2-Humble-22314E?logo=ros)](https://docs.ros.org/en/humble)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Ubuntu](https://img.shields.io/badge/Ubuntu-22.04%20Jammy-orange)](https://ubuntu.com/)
[![C++](https://img.shields.io/badge/C++-17-brightgreen)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/Python-3.10-yellow)](https://www.python.org/)
[![GitHub release](https://img.shields.io/github/v/release/YehiaElkh/ros2_yehia_learning)](https://github.com/YehiaElkh/ros2_yehia_learning/releases)
[![GitHub stars](https://img.shields.io/github/stars/YehiaElkh/ros2_yehia_learning)](https://github.com/YehiaElkh/ros2_yehia_learning/stargazers)
---

## 📖 Description

This repository is a **hands-on learning journey** through ROS 2 Humble core concepts.
Every concept is implemented in **both C++ (`rclcpp`) and Python (`rclpy`)** side by side,
making it easy to compare and understand the two APIs.

---

*Ce dépôt est un **parcours d'apprentissage pratique** des concepts fondamentaux de ROS 2 Humble.
Chaque concept est implémenté en **C++ (`rclcpp`) et Python (`rclpy`)** en parallèle,
ce qui permet de comparer et comprendre les deux APIs.*

---

## 🛠️ Prerequisites / Prérequis

- Ubuntu 22.04 LTS (Jammy Jellyfish)
- ROS 2 Humble installed and sourced
- `colcon` build tools
- `rosdep`

```bash
source /opt/ros/humble/setup.bash
```

---

## 🚀 Getting Started / Démarrage

```bash
# Clone the repository
git clone https://github.com/yehiaelkh/ros2_yehia_learning.git
cd ros2_yehia_learning

# Install dependencies
rosdep install --from-paths src --ignore-src --rosdistro humble -y

# Build all packages
cd ~/ros2_ws
colcon build --symlink-install

# Source the workspace
source install/setup.bash
```

---

## 📦 Repository Structure / Structure du dépôt

```
ros2_yehia_learning/
│
├── start_with_simple_nodes/           # C++ — Simple nodes & timers
├── start_with_simple_nodes_py/        # Python — Simple nodes & timers
│
├── publisher_and_subscriber/          # C++ — Topics, pub/sub, custom msgs
├── publisher_and_subscriber_py/       # Python — Topics, pub/sub, custom msgs
│
├── custom_msg_and_srv/                # C++ — Custom .msg, .srv, .action definitions
│
├── service_server_and_client/         # C++ — Services server & client
├── service_server_and_client_py/      # Python — Services server & client
│
├── parameters/                        # C++ — Parameters declaration & usage
├── parameters_py/                     # Python — Parameters declaration & usage
│
├── actions/
│   ├── action_tutorial/               # C++ — Action server & client
│   └── action_tutorial_py/            # Python — Action server & client
│
├── dynamic_tf2_publisher/             # C++ — TF2 dynamic broadcaster
├── dynamic_tf2_publisher_py/          # Python — TF2 dynamic broadcaster
│
├── plugins/
│   ├── vehicle_base/                  # C++ — Abstract base class (pluginlib)
│   ├── vehicle_plugins/               # C++ — Concrete plugin implementations
│   ├── vehicle_base_py/               # Python — Abstract base class (abc)
│   └── vehicle_plugins_py/            # Python — Concrete plugin implementations
│
├── message_sync/                      # C++ — Multi-topic synchronization
├── message_sync_py/                   # Python — Multi-topic synchronization
│
└── create_library_with_header/
    ├── publisher_library/             # C++ — Reusable shared library
    └── use_library/                   # C++ — Consumes the shared library
└── create_library_with_header_py/
    ├── publisher_library_py/          # Python — Reusable Python module
    └── use_library_py/                # Python — Consumes the Python module
```

---

## 📚 Concepts Covered / Concepts couverts

### 1. 🟢 Simple Nodes — `start_with_simple_nodes`

The very first step — creating a ROS 2 node.

*Le tout premier pas — créer un nœud ROS 2.*

```bash
ros2 run start_with_simple_nodes my_first_node
ros2 run start_with_simple_nodes node_with_timer
ros2 run start_with_simple_nodes_py my_first_node
ros2 run start_with_simple_nodes_py node_with_timer
```

**What you learn / Ce qu'on apprend :**
- `rclcpp::Node` / `rclpy.node.Node` — base class for all nodes
- `create_timer()` — periodic callbacks
- `RCLCPP_INFO` / `get_logger().info()` — logging
- `rclcpp::spin()` / `rclpy.spin()` — event loop

---

### 2. 📡 Publisher & Subscriber — `publisher_and_subscriber`

One-way communication between nodes using topics.

*Communication unidirectionnelle entre nœuds via des topics.*

```bash
# Standard messages
ros2 run publisher_and_subscriber simple_publisher
ros2 run publisher_and_subscriber simple_subscriber

# Custom messages
ros2 run publisher_and_subscriber publisher_with_custom_msg
ros2 run publisher_and_subscriber_py subscriber_with_custom_msg
```

**What you learn / Ce qu'on apprend :**
- `create_publisher()` / `create_subscription()` — pub/sub API
- `std_msgs/String` — standard message types
- Custom `.msg` definitions — `EmployeeSalary.msg`
- QoS (Quality of Service) — queue size

---

### 3. 🔧 Services — `service_server_and_client`

Two-way request/response communication.

*Communication requête/réponse bidirectionnelle.*

```bash
# Terminal 1
ros2 run service_server_and_client simple_server

# Terminal 2
ros2 run service_server_and_client_py simple_client

# From terminal
ros2 service call /add_two_ints custom_msg_and_srv/srv/AddTwoInts "{a: 10, b: 25}"
```

**What you learn / Ce qu'on apprend :**
- `create_service()` / `create_client()` — service API
- Custom `.srv` definitions — `AddTwoInts.srv`
- Async service calls — `async_send_request()` / `call_async()`
- Service callbacks — request/response pattern

---

### 4. ⚙️ Parameters — `parameters`

Configure nodes without recompiling.

*Configurer les nœuds sans recompiler.*

```bash
# Default values
ros2 run parameters node_with_params

# Custom values
ros2 run parameters node_with_params \
  --ros-args -p timer_period:=0.2 -p message:="Hello!" -p max_count:=5

# Change at runtime
ros2 param set /node_with_params message "Changed at runtime!"
```

**What you learn / Ce qu'on apprend :**
- `declare_parameter()` — declare with default value
- `get_parameter().as_<type>()` — read parameter value
- `ros2 param` CLI — inspect and modify parameters at runtime

---

### 5. 🎯 Actions — `actions/action_tutorial`

Long-running tasks with feedback and cancellation.

*Tâches longues avec feedback et annulation.*

```bash
# Terminal 1
ros2 run action_tutorial action_server

# Terminal 2
ros2 run action_tutorial_py action_client

# From terminal (with feedback)
ros2 action send_goal --feedback /concatenate \
  custom_msg_and_srv/action/Concatenate \
  "{first_string: 'Hello', second_string: 'World'}"
```

**What you learn / Ce qu'on apprend :**
- `rclcpp_action` / `rclpy.action` — action API
- Goal / Feedback / Result — 3-part action lifecycle
- Custom `.action` definitions — `Concatenate.action`
- Goal cancellation handling

---

### 6. 🗺️ TF2 — `dynamic_tf2_publisher`

Coordinate frame transformations.

*Transformations entre repères de coordonnées.*

```bash
ros2 run dynamic_tf2_publisher dynamic_tf2_publisher \
  --ros-args -p parent_frame:=map -p child_frame:=base_link \
  -p x:=1.0 -p y:=0.5 -p z:=0.0

# Inspect transforms
ros2 run tf2_ros tf2_echo map base_link
ros2 run tf2_tools view_frames
```

**What you learn / Ce qu'on apprend :**
- `TransformBroadcaster` — publish TF2 transforms
- `TransformStamped` — stamped transform message
- Quaternion rotation — yaw to quaternion conversion
- TF2 tree — parent/child frame relationships

---

### 7. 🔌 Plugins — `plugins/`

Dynamically loadable class implementations.

*Implémentations de classes chargées dynamiquement.*

```bash
# C++ (pluginlib)
ros2 run vehicle_base create_vehicle

# Python (entry_points)
ros2 run vehicle_base_py vehicle_loader
```

**What you learn / Ce qu'on apprend :**
- `pluginlib` — C++ plugin system
- `abc.ABC` + `abstractmethod` — Python abstract base class
- `setuptools entry_points` — Python plugin registration
- Dynamic class loading at runtime — no recompilation needed

---

### 8. 🔄 Message Sync — `message_sync`

Synchronize messages from multiple topics.

*Synchroniser des messages de plusieurs topics.*

```bash
# Terminal 1
ros2 run message_sync multi_publisher

# Terminal 2
ros2 run message_sync sync_subscriber
# or Python
ros2 run message_sync_py sync_subscriber
```

**What you learn / Ce qu'on apprend :**
- `message_filters` — multi-topic synchronization
- `ApproximateTimeSynchronizer` — approximate time sync
- Callback with multiple messages — synchronized processing

---

### 9. 📚 Reusable Libraries — `create_library_with_header`

Build and consume shared C++ and Python libraries.

*Créer et utiliser des librairies C++ et Python réutilisables.*

```bash
ros2 run use_library use_library
ros2 run use_library_py use_library
```

**What you learn / Ce qu'on apprend :**
- `add_library(SHARED ...)` — build a shared library in CMake
- `ament_export_targets()` — export library for other packages
- `target_link_libraries()` — link against a custom library
- Python modules as reusable packages

---

## 🗺️ Learning Path / Parcours d'apprentissage

```
1. Simple Nodes     →  Understand the node lifecycle
2. Pub/Sub          →  One-way communication
3. Custom Messages  →  Define your own data types
4. Services         →  Two-way request/response
5. Parameters       →  Runtime configuration
6. Actions          →  Long-running tasks with feedback
7. TF2              →  Coordinate transforms
8. Plugins          →  Extensible architecture
9. Message Sync     →  Multi-sensor data fusion
10. Libraries       →  Code reuse and modularity
```

---

## 🔍 Useful ROS 2 Commands / Commandes utiles

```bash
# Topics
ros2 topic list
ros2 topic echo /topic_name
ros2 topic hz /topic_name
ros2 topic info /topic_name

# Services
ros2 service list
ros2 service call /service_name pkg/srv/Type "{field: value}"

# Parameters
ros2 param list
ros2 param get /node_name param_name
ros2 param set /node_name param_name value

# Actions
ros2 action list
ros2 action send_goal --feedback /action_name pkg/action/Type "{}"

# Nodes
ros2 node list
ros2 node info /node_name

# TF2
ros2 run tf2_tools view_frames
ros2 run tf2_ros tf2_echo parent_frame child_frame

# Interfaces
ros2 interface show pkg/msg/Type
ros2 interface show pkg/srv/Type
ros2 interface show pkg/action/Type
```

---

## 👤 Author / Auteur

**Yehia Elkh*
- GitHub: [@yehiaelkh](https://github.com/YehiaElkh)
- LinkedIn: [yehiaelkh](https://www.linkedin.com/in/yahya-elkhoulati-535580331/)

---

## 📄 License

This project is licensed under the Apache 2.0 License — see the [LICENSE](LICENSE) file for details.

---

## ⭐ If this repo helped you, give it a star!
## ⭐ Si ce dépôt vous a aidé, donnez-lui une étoile !

 
