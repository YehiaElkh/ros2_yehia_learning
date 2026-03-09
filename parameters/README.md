# parameters

> Runtime node configuration using ROS 2 parameters in C++ | *Configuration des nœuds à l'exécution en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Declare parameters with `declare_parameter(name, default_value)`
- Read parameters with `get_parameter(name).as_<type>()`
- Pass parameters from the command line with `--ros-args -p`
- Modify parameters at runtime with `ros2 param set`
- Understand why parameters are better than hardcoded values

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/node_with_params.cpp` | Node with timer_period, message, max_count parameters |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select parameters
source install/setup.bash

# Run with default values
ros2 run parameters node_with_params

# Run with custom values
ros2 run parameters node_with_params \
  --ros-args \
  -p timer_period:=0.2 \
  -p message:="Bonjour ROS2 !" \
  -p max_count:=5

# Inspect parameters at runtime (while node is running)
ros2 param list
ros2 param get /node_with_params message
ros2 param set /node_with_params message "Changed at runtime !"
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Declare parameters with default values
this->declare_parameter("timer_period", 1.0);      // double
this->declare_parameter("message", std::string("Hello !"));  // string
this->declare_parameter("max_count", 10);          // int

// Read parameters
double period = this->get_parameter("timer_period").as_double();
std::string msg = this->get_parameter("message").as_string();
int count = this->get_parameter("max_count").as_int();

// Available types:
// .as_double()  → double
// .as_string()  → string
// .as_int()     → int64
// .as_bool()    → bool
```

---

## 📊 Hardcoded vs Parameters

| | Hardcoded | Parameters |
|--|-----------|------------|
| Change value | Recompile | CLI / file |
| Flexibility | ❌ | ✅ |
| Production use | Never | Always |

---

## 🔗 See also / Voir aussi

- Python equivalent: [`parameters_py`](../parameters_py/README.md)
