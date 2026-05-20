from vehicle_base_py.vehicle_base import VehicleBase

class Truck(VehicleBase):
    def move(self) -> None:
        print("Truck is moving heavily ! 🚛")

    def stop(self) -> None:
        print("Truck stopped !")

    def get_info(self) -> str:
        return "Truck - 18 wheels - heavy vehicle"