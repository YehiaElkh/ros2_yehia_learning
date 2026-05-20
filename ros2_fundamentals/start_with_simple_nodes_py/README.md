# start_with_simple_nodes_py

> First steps with ROS 2 nodes in Python | *Premiers pas avec les nœuds ROS 2 en Python*

---

## 📖 What you learn / Ce qu'on apprend

- Create a basic ROS 2 node using `rclpy.node.Node`
- Use timers with `create_timer()` for periodic callbacks
- Log messages with `get_logger().info()`, `.warn()`, `.error()`
- Understand `rclpy.spin()` — the ROS 2 event loop
- Class-based node pattern (recommended for real projects)

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `my_first_node.py` | Simplest possible ROS 2 node |
| `node_with_timer.py` | Node with a 500ms periodic timer |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select start_with_simple_nodes_py
source install/setup.bash

# Simple node
ros2 run start_with_simple_nodes_py my_first_node

# Node with timer
ros2 run start_with_simple_nodes_py node_with_timer
```

---

## 🧠 Key Concepts / Concepts clés

```python
# Create a node
class MyNode(Node):
    def __init__(self):
        super().__init__("my_node")

        # Create a timer (500ms period)
        self.timer_ = self.create_timer(0.5, self.timer_callback)

    def timer_callback(self):
        # Logging
        self.get_logger().info("Hello ROS2 !")

# Spin (keep node alive)
rclpy.spin(node)
```

---

## 🔗 See also / Voir aussi

- C++ equivalent: [`start_with_simple_nodes`](../start_with_simple_nodes/README.md)
