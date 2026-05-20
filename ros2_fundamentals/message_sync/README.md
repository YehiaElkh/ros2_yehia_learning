# message_sync

> Synchronize messages from multiple topics in C++ | *Synchroniser des messages de plusieurs topics en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Publish on multiple topics simultaneously
- Use `message_filters::Subscriber` for sync-aware subscribers
- Use `ApproximateTimeSynchronizer` for approximate time sync
- Understand why synchronization matters in robotics
- Process multiple sensor streams together

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/multi_publisher.cpp` | Publishes on `/camera_data` and `/lidar_data` simultaneously |
| `src/sync_subscriber.cpp` | Receives both topics synchronized with ApproximateTime |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select message_sync
source install/setup.bash

# Terminal 1 — Publisher
ros2 run message_sync multi_publisher

# Terminal 2 — Sync Subscriber (C++)
ros2 run message_sync sync_subscriber

# Or Python subscriber
ros2 run message_sync_py sync_subscriber
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// message_filters subscribers (not rclcpp::Subscription !)
sub1_ = std::make_shared<message_filters::Subscriber<std_msgs::msg::String>>(
    this, "camera_data"
);
sub2_ = std::make_shared<message_filters::Subscriber<std_msgs::msg::Int32>>(
    this, "lidar_data"
);

// Synchronizer with ApproximateTime policy
typedef message_filters::sync_policies::ApproximateTime<
    std_msgs::msg::String, std_msgs::msg::Int32> SyncPolicy;
typedef message_filters::Synchronizer<SyncPolicy> Synchronizer;

sync_ = std::make_shared<Synchronizer>(SyncPolicy(10), *sub1_, *sub2_);

// Synchronized callback — called ONLY when both messages are available
sync_->registerCallback(std::bind(&SyncSubscriber::syncCallback, this,
    std::placeholders::_1, std::placeholders::_2));

void syncCallback(
    const std_msgs::msg::String::ConstSharedPtr msg1,
    const std_msgs::msg::Int32::ConstSharedPtr msg2)
{
    // Both messages are time-aligned !
}
```

---

## 📊 Without sync vs With sync

```
Without sync:
/camera : t=0.0  t=0.1  t=0.2  t=0.3
/lidar  : t=0.0         t=0.2
→ Mismatched data processing ❌

With ApproximateTime sync:
/camera : t=0.0         t=0.2
/lidar  : t=0.0         t=0.2
→ Aligned data processing ✅
```

---

## ⚙️ Sync Policies

| Policy | Description | Use case |
|--------|-------------|----------|
| `ApproximateTime` | Timestamps close enough | Real sensors |
| `ExactTime` | Identical timestamps | Simulation |

---

## 🔗 Dependencies / Dépendances

```bash
sudo apt install ros-humble-message-filters
```

## 🔗 See also / Voir aussi

- Python equivalent: [`message_sync_py`](../message_sync_py/README.md)
