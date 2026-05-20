# publisher_and_subscriber

> One-way communication between nodes using topics in C++ | *Communication unidirectionnelle entre nœuds en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Publish messages on a topic with `create_publisher()`
- Subscribe to a topic with `create_subscription()`
- Use standard messages — `std_msgs/String`
- Use custom messages — `EmployeeSalary.msg`
- Understand QoS (Quality of Service) — queue size
- Understand topic decoupling — publisher and subscriber don't know each other

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/simple_publisher.cpp` | Publisher using `std_msgs/String` on `/chatter` |
| `src/simple_subscriber.cpp` | Subscriber listening to `/chatter` |
| `src/publisher_with_custom_msg.cpp` | Publisher using `EmployeeSalary.msg` on `/employee` |
| `src/subscriber_with_custom_msg.cpp` | Subscriber listening to `/employee` |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select publisher_and_subscriber
source install/setup.bash

# Standard messages
# Terminal 1
ros2 run publisher_and_subscriber simple_publisher
# Terminal 2
ros2 run publisher_and_subscriber simple_subscriber

# Custom messages
# Terminal 1
ros2 run publisher_and_subscriber publisher_with_custom_msg
# Terminal 2 (Python subscriber works too!)
ros2 run publisher_and_subscriber_py subscriber_with_custom_msg
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Create a publisher
publisher_ = this->create_publisher<std_msgs::msg::String>(
    "chatter",  // topic name
    10          // QoS queue size
);

// Publish a message
auto msg = std_msgs::msg::String();
msg.data = "Hello ROS2 !";
publisher_->publish(msg);

// Create a subscriber
subscriber_ = this->create_subscription<std_msgs::msg::String>(
    "chatter",  // topic name
    10,         // QoS queue size
    std::bind(&MyNode::callback, this, std::placeholders::_1)
);

// Callback
void callback(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());
}
```

---

## 🔍 Useful Commands / Commandes utiles

```bash
# List active topics
ros2 topic list

# Monitor a topic
ros2 topic echo /chatter
ros2 topic echo /employee

# Check publishing frequency
ros2 topic hz /chatter

# Topic info
ros2 topic info /chatter
```

---

## 🔗 Dependencies / Dépendances

- `custom_msg_and_srv` — for `EmployeeSalary.msg`

## 🔗 See also / Voir aussi

- Python equivalent: [`publisher_and_subscriber_py`](../publisher_and_subscriber_py/README.md)
- Custom message definitions: [`custom_msg_and_srv`](../custom_msg_and_srv/README.md)
