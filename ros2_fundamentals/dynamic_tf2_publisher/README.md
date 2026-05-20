# dynamic_tf2_publisher

> Publish dynamic TF2 coordinate transforms in C++ | *Publier des transformations TF2 dynamiques en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Broadcast TF2 transforms with `TransformBroadcaster`
- Build a `TransformStamped` message
- Convert yaw angle to quaternion
- Use parameters to configure frames dynamically
- Inspect the TF2 tree with `tf2_tools`
- Understand parent/child frame relationships

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/dynamic_tf2_publisher.cpp` | TF2 broadcaster configurable via parameters |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select dynamic_tf2_publisher
source install/setup.bash

# Run with default parameters (map → base_link)
ros2 run dynamic_tf2_publisher dynamic_tf2_publisher

# Run with custom parameters
ros2 run dynamic_tf2_publisher dynamic_tf2_publisher \
  --ros-args \
  -p parent_frame:=map \
  -p child_frame:=base_link \
  -p x:=1.0 \
  -p y:=0.5 \
  -p z:=0.0 \
  -p yaw:=1.57

# Inspect the transform
ros2 run tf2_ros tf2_echo map base_link

# View the full TF2 tree
ros2 run tf2_tools view_frames
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Create a TF2 broadcaster
tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

// Build the transform message
geometry_msgs::msg::TransformStamped transform;
transform.header.stamp    = this->get_clock()->now();
transform.header.frame_id = "map";        // parent frame
transform.child_frame_id  = "base_link";  // child frame

// Translation
transform.transform.translation.x = 1.0;
transform.transform.translation.y = 0.5;
transform.transform.translation.z = 0.0;

// Rotation (quaternion from yaw)
transform.transform.rotation.z = std::sin(yaw / 2.0);
transform.transform.rotation.w = std::cos(yaw / 2.0);

// Broadcast the transform
tf_broadcaster_->sendTransform(transform);
```

---

## 📊 TF2 Tree / Arbre TF2

```
map (world frame)
 └── base_link (robot body)
      ├── camera_link
      ├── lidar_link
      └── arm_link
```

---

## ⚙️ Parameters / Paramètres

| Parameter | Default | Description |
|-----------|---------|-------------|
| `parent_frame` | `map` | Parent coordinate frame |
| `child_frame` | `base_link` | Child coordinate frame |
| `x` | `0.0` | Translation X (meters) |
| `y` | `0.0` | Translation Y (meters) |
| `z` | `0.0` | Translation Z (meters) |
| `yaw` | `0.0` | Rotation around Z (radians) |

---

## 🔗 Dependencies / Dépendances

- `tf2_ros`
- `geometry_msgs`

```bash
sudo apt install ros-humble-tf2-ros ros-humble-tf2-tools
```

## 🔗 See also / Voir aussi

- Python equivalent: [`dynamic_tf2_publisher_py`](../dynamic_tf2_publisher_py/README.md)
