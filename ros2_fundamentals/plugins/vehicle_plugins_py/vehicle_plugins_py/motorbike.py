from vehicle_base_py.vehicle_base import VehicleBase

class Motorbike(VehicleBase):
    # --------------------------------------------------------
    # IMPLÉMENTER TOUTES LES MÉTHODES ABSTRAITES
    # --------------------------------------------------------

    def move(self) -> None:
        print("Motorbike is moving fast ! 🏍️")

    def stop(self) -> None:
        print("Motorbike stopped !")

    def get_info(self) -> str:
        return "Motorbike - 2 wheels - fast vehicle"