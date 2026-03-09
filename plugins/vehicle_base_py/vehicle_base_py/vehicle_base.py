from abc import ABC, abstractmethod
# abc = Abstract Base Class
# ABC = classe de base pour créer des classes abstraites
# abstractmethod = décorateur pour les méthodes abstraites

class VehicleBase(ABC):
    # --------------------------------------------------------
    # MÉTHODES ABSTRAITES
    # --------------------------------------------------------
    # @abstractmethod = la classe fille DOIT implémenter cette méthode
    # sinon elle sera aussi abstraite (non instanciable)

    @abstractmethod
    def move(self) -> None:
        """Démarrer le véhicule"""
        pass

    @abstractmethod
    def stop(self) -> None:
        """Arrêter le véhicule"""
        pass

    @abstractmethod
    def get_info(self) -> str:
        """Retourner les infos du véhicule"""
        pass