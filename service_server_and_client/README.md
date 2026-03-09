# service_server_and_client

> Two-way request/response communication in C++ | *Communication requête/réponse bidirectionnelle en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Create a service server with `create_service()`
- Create a service client with `create_client()`
- Send async requests with `async_send_request()`
- Handle service callbacks — request and response
- Wait for service availability with `wait_for_service()`
- Understand the difference between services and topics

---

## 📁 Files / Fichiers

| File | Description |
|------|-------------|
| `src/simple_server.cpp` | Service server — adds two integers |
| `src/simple_client.cpp` | Service client — sends a:10, b:25 |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
colcon build --packages-select service_server_and_client
source install/setup.bash

# Terminal 1 — Start the server
ros2 run service_server_and_client simple_server

# Terminal 2 — Start the client
ros2 run service_server_and_client simple_client

# Or call directly from terminal
ros2 service call /add_two_ints \
  custom_msg_and_srv/srv/AddTwoInts "{a: 10, b: 25}"
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Create a service server
server_ = this->create_service<custom_msg_and_srv::srv::AddTwoInts>(
    "add_two_ints",
    std::bind(&SimpleServer::serviceCallback, this,
        std::placeholders::_1, std::placeholders::_2)
);

// Service callback
void serviceCallback(
    const Request::SharedPtr request,
    Response::SharedPtr response)
{
    response->sum = request->a + request->b;
}

// Create a service client
client_ = this->create_client<custom_msg_and_srv::srv::AddTwoInts>("add_two_ints");

// Send async request
auto request = std::make_shared<AddTwoInts::Request>();
request->a = 10; request->b = 25;
client_->async_send_request(request, callback);
```

---

## 📊 Topic vs Service

| | Topic | Service |
|--|-------|---------|
| Direction | One-way | Two-way |
| Response | ❌ | ✅ |
| Cancellable | ❌ | ❌ |
| Use case | Continuous data | One-time request |

---

## 🔍 Useful Commands / Commandes utiles

```bash
ros2 service list
ros2 service type /add_two_ints
ros2 service call /add_two_ints custom_msg_and_srv/srv/AddTwoInts "{a: 5, b: 3}"
```

---

## 🔗 Dependencies / Dépendances

- `custom_msg_and_srv` — for `AddTwoInts.srv`

## 🔗 See also / Voir aussi

- Python equivalent: [`service_server_and_client_py`](../service_server_and_client_py/README.md)
