# custom_msg_and_srv

> Custom message, service and action definitions | *Définitions de messages, services et actions personnalisés*

---

## 📖 What you learn / Ce qu'on apprend

- Define custom `.msg` files for topics
- Define custom `.srv` files for services
- Define custom `.action` files for actions
- Generate C++ and Python code with `rosidl_generate_interfaces`
- Use `ros2 interface show` to inspect definitions

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `msg/EmployeeSalary.msg` | Custom message with employee data |
| `srv/AddTwoInts.srv` | Custom service to add two integers |
| `action/Concatenate.action` | Custom action to concatenate strings |

---

## 📝 Definitions / Définitions

### EmployeeSalary.msg
```
string first_name
string last_name
float32 salary
```

### AddTwoInts.srv
```
int64 a
int64 b
---
int64 sum
```

### Concatenate.action
```
# Goal
string first_string
string second_string
---
# Result
string concatenated_string
---
# Feedback
string current_status
```

---

## 🚀 Build & Verify

```bash
cd ~/ros2_ws
colcon build --packages-select custom_msg_and_srv
source install/setup.bash

# Verify generated interfaces
ros2 interface show custom_msg_and_srv/msg/EmployeeSalary
ros2 interface show custom_msg_and_srv/srv/AddTwoInts
ros2 interface show custom_msg_and_srv/action/Concatenate
```

---

## 🧠 Key Concepts / Concepts clés

```cmake
# CMakeLists.txt — generate interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/EmployeeSalary.msg"
  "srv/AddTwoInts.srv"
  "action/Concatenate.action"
)
```

```cpp
// C++ — include generated header
// .msg → snake_case .hpp
#include "custom_msg_and_srv/msg/employee_salary.hpp"
#include "custom_msg_and_srv/srv/add_two_ints.hpp"
#include "custom_msg_and_srv/action/concatenate.hpp"
```

```python
# Python — import generated module
from custom_msg_and_srv.msg import EmployeeSalary
from custom_msg_and_srv.srv import AddTwoInts
from custom_msg_and_srv.action import Concatenate
```

---

## 🔗 Used by / Utilisé par

- `publisher_and_subscriber` / `publisher_and_subscriber_py`
- `service_server_and_client` / `service_server_and_client_py`
- `actions/action_tutorial` / `actions/action_tutorial_py`
