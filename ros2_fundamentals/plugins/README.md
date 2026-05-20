# plugins — vehicle_base & vehicle_plugins

> Dynamically loadable plugin system using pluginlib in C++ | *Système de plugins chargés dynamiquement avec pluginlib en C++*

---

## 📖 What you learn / Ce qu'on apprend

- Define an abstract base class for plugins
- Implement concrete plugin classes
- Register plugins with `PLUGINLIB_EXPORT_CLASS`
- Declare plugins in `plugins.xml`
- Load plugins dynamically with `pluginlib::ClassLoader`
- Change behavior at runtime without recompiling

---

## 📁 Files / Fichiers

### vehicle_base
| File | Description |
|------|-------------|
| `include/vehicle_base/vehicle_base.hpp` | Abstract base class with pure virtual methods |
| `src/vehicle_loader.cpp` | Loads and uses plugins dynamically |

### vehicle_plugins
| File | Description |
|------|-------------|
| `include/vehicle_plugins/motorbike.hpp` | Motorbike plugin declaration |
| `include/vehicle_plugins/bicycle.hpp` | Bicycle plugin declaration |
| `include/vehicle_plugins/truck.hpp` | Truck plugin declaration |
| `src/motorbike.cpp` | Motorbike implementation + PLUGINLIB_EXPORT_CLASS |
| `src/bicycle.cpp` | Bicycle implementation + PLUGINLIB_EXPORT_CLASS |
| `src/truck.cpp` | Truck implementation + PLUGINLIB_EXPORT_CLASS |
| `plugins.xml` | Plugin descriptions for pluginlib |

---

## 🚀 Build & Run

```bash
cd ~/ros2_ws
# Build base first, then plugins
colcon build --packages-select vehicle_base vehicle_plugins
source install/setup.bash

ros2 run vehicle_base create_vehicle
```

---

## 🧠 Key Concepts / Concepts clés

```cpp
// Abstract base class
class VehicleBase {
public:
    virtual void move() = 0;   // pure virtual
    virtual void stop() = 0;
    virtual std::string getInfo() = 0;
};

// Concrete plugin
class Motorbike : public VehicleBase {
public:
    void move() override { /* ... */ }
    void stop() override { /* ... */ }
    std::string getInfo() override { return "Motorbike"; }
};

// Register the plugin — MANDATORY macro
PLUGINLIB_EXPORT_CLASS(vehicle_plugins::Motorbike, vehicle_base::VehicleBase)

// Load plugin dynamically
pluginlib::ClassLoader<VehicleBase> loader("vehicle_base", "vehicle_base::VehicleBase");
auto vehicle = loader.createSharedInstance("vehicle_plugins::Motorbike");
vehicle->move();  // calls Motorbike::move()
```

---

## 📝 plugins.xml
```xml
<library path="vehicle_plugins">
  <class name="vehicle_plugins::Motorbike"
         type="vehicle_plugins::Motorbike"
         base_class_type="vehicle_base::VehicleBase">
    <description>A motorbike plugin</description>
  </class>
</library>
```

---

## 💡 Key advantage / Avantage clé

```cpp
// Change plugin = change ONE line, NO recompilation !
auto vehicle = loader.createSharedInstance("vehicle_plugins::Motorbike");
// → change to:
auto vehicle = loader.createSharedInstance("vehicle_plugins::Truck");
```

---

## 🔗 See also / Voir aussi

- Python equivalent: [`vehicle_base_py`](../vehicle_base_py/README.md) & [`vehicle_plugins_py`](../vehicle_plugins_py/README.md)
