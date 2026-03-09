# start_with_simple_nodes

> First steps with ROS 2 nodes in C++ | *Premiers pas avec les nœuds ROS 2 en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Create a basic ROS 2 node using `rclcpp::Node`
- Use timers with `create_timer()` for periodic callbacks
- Log messages with `RCLCPP_INFO`, `RCLCPP_WARN`, `RCLCPP_ERROR`
- Understand `rclcpp::spin()` — the ROS 2 event loop
- Class-based node pattern (recommended for real projects)

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/my_first_node.cpp` | Simplest possible ROS 2 node |
| `src/node_with_timer.cpp` | Node with a 500ms periodic timer |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select start_with_simple_nodes
source install/setup.bash

# Simple node
ros2 run start_with_simple_nodes my_first_node

# Node with timer
ros2 run start_with_simple_nodes node_with_timer
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Create a node
auto node = std::make_shared<rclcpp::Node>("my_node");

// Create a timer (500ms period)
timer_ = this->create_timer(
    std::chrono::milliseconds(500),
    std::bind(&MyNode::timerCallback, this)
);

// Logging
RCLCPP_INFO(this->get_logger(), "Hello ROS2 !");

// Spin (keep node alive)
rclcpp::spin(node);
```

---

## 🔗 See also / Voir aussi

- Python equivalent: [`start_with_simple_nodes_py`](../start_with_simple_nodes_py/README.md)
