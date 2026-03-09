# create_library_with_header

> Build and consume reusable C++ shared libraries | *Créer et utiliser des librairies C++ partagées réutilisables*

---

## 📖 What you learn / Ce qu'on apprend

- Create a shared library (`add_library(SHARED ...)`)
- Export headers for other packages (`ament_export_targets`)
- Consume a library from another package (`target_link_libraries`)
- Understand the difference between library and executable
- Organize reusable code to avoid duplication

---

## 📁 Structure

```
create_library_with_header/
├── publisher_library/         ← the library
│   ├── include/publisher_library/
│   │   └── timed_publisher.hpp    ← public API header
│   └── src/
│       └── timed_publisher.cpp    ← implementation
└── use_library/               ← consumes the library
    └── src/
        └── use_library.cpp
```

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws

# Build library first, then the consumer
colcon build --packages-select publisher_library
colcon build --packages-select use_library
source install/setup.bash

ros2 run use_library use_library
```

---

## 🧠 Key Concepts / Concepts clés

### publisher_library — CMakeLists.txt
```cmake
# Create a shared library (not an executable)
add_library(publisher_library SHARED
  src/timed_publisher.cpp
)

# Export headers for other packages
target_include_directories(publisher_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Export the library target
install(TARGETS publisher_library
  EXPORT publisher_library_targets
  DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

ament_export_targets(publisher_library_targets HAS_LIBRARY_TARGET)
```

### use_library — CMakeLists.txt
```cmake
find_package(publisher_library REQUIRED)

add_executable(use_library src/use_library.cpp)

# Link against the library
target_link_libraries(use_library
  publisher_library::publisher_library
)
```

### use_library.cpp
```cpp
// Import the library header
#include "publisher_library/timed_publisher.hpp"

// Use the library class
publisher1_ = std::make_shared<publisher_library::TimedPublisher>(
    shared_from_this(),   // current node
    "topic_fast",         // topic name
    0.5                   // period in seconds
);
publisher1_->setMessage("Fast message");
```

---

## 💡 Key advantage / Avantage clé

```
Without library:         With library:
PackageA has TimedPub    publisher_library has TimedPub
PackageB has TimedPub    PackageA uses publisher_library
PackageC has TimedPub    PackageB uses publisher_library
→ Bug fix = 3 changes    → Bug fix = 1 change ✅
```

---

## 🔗 See also / Voir aussi

- Python equivalent: [`create_library_with_header_py`](../create_library_with_header_py/README.md)
