# action_tutorial

> Long-running tasks with feedback and cancellation in C++ | *Tâches longues avec feedback et annulation en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Create an action server with `rclcpp_action::create_server()`
- Create an action client with `rclcpp_action::create_client()`
- Handle the 3 action callbacks — goal, cancel, accepted
- Send feedback during execution with `publish_feedback()`
- Return a final result with `goal_handle->succeed()`
- Handle cancellation with `is_canceling()`
- Understand Goal / Feedback / Result lifecycle

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/action_server.cpp` | Action server — concatenates two strings with 3-step feedback |
| `src/action_client.cpp` | Action client — sends goal and receives feedback + result |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select action_tutorial
source install/setup.bash

# Terminal 1 — Start the server
ros2 run action_tutorial action_server

# Terminal 2 — Start the client
ros2 run action_tutorial action_client

# Or send a goal from terminal
ros2 action send_goal --feedback /concatenate \
  custom_msg_and_srv/action/Concatenate \
  "{first_string: 'Hello', second_string: 'ROS2 Actions !'}"
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Action server — 3 callbacks
server_ = rclcpp_action::create_server<Concatenate>(
    this, "concatenate",
    handleGoal,      // Accept or reject
    handleCancel,    // Handle cancellation
    handleAccepted   // Start execution in a thread
);

// Send feedback
feedback->current_status = "Step 1/3...";
goal_handle->publish_feedback(feedback);

// Send result
result->concatenated_string = "Hello World";
goal_handle->succeed(result);

// Action client — 3 callbacks
options.goal_response_callback = ...;   // Accepted/Rejected
options.feedback_callback = ...;        // Feedback during execution
options.result_callback = ...;          // Final result
client_->async_send_goal(goal, options);
```

---

## 📊 Service vs Action

| | Service | Action |
|--|---------|--------|
| Duration | Short | Long |
| Feedback | ❌ | ✅ |
| Cancellable | ❌ | ✅ |
| Use case | Quick calc | Navigation, arm movement |

---

## 🔍 Useful Commands / Commandes utiles

```bash
ros2 action list
ros2 action info /concatenate
ros2 action send_goal --feedback /concatenate \
  custom_msg_and_srv/action/Concatenate \
  "{first_string: 'Bonjour', second_string: 'le monde'}"
```

---

## 🔗 Dependencies / Dépendances

- `custom_msg_and_srv` — for `Concatenate.action`
- `rclcpp_action`

## 🔗 See also / Voir aussi

- Python equivalent: [`action_tutorial_py`](../action_tutorial_py/README.md)
