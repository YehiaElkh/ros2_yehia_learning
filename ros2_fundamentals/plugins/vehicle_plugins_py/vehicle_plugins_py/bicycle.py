from vehicle_base_py.vehicle_base import VehicleBase

class Bicycle(VehicleBase):
    def move(self) -> None:
        print("Bicycle is moving smoothly ! 🚲")

    def stop(self) -> None:
        print("Bicycle stopped !")

    def get_info(self) -> str:
        return "Bicycle - 2 wheels - eco friendly vehicle"