#!/usr/bin/env python3
# ============================================================
# IMPORTS
# ============================================================
import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.callback_groups import ReentrantCallbackGroup
# ReentrantCallbackGroup : permet d'exécuter plusieurs callbacks en parallèle
# Nécessaire pour utiliser actions + services + topics en même temps

from nav2_msgs.action import NavigateToPose
# NavigateToPose : action Nav2 pour naviguer vers un point

from std_msgs.msg import String, Int32, Bool
# String  : publier l'état de la mission en texte
# Int32   : publier l'index du waypoint actuel
# Bool    : publier si la mission est active

from std_srvs.srv import Trigger, SetBool
# Trigger : service simple sans paramètres (juste appeler)
# SetBool : service avec un booléen en paramètre

import math
# math : fonctions mathématiques (sin, cos pour quaternion)

import time
# time : pour les délais entre waypoints

import yaml
# yaml : pour lire le fichier de configuration

import os
# os : pour construire les chemins de fichiers

from ament_index_python.packages import get_package_share_directory
# get_package_share_directory : trouver le chemin du package installé

# ============================================================
# ÉNUMÉRATION DES ÉTATS DE LA MISSION
# ============================================================
class MissionState:
    IDLE      = "IDLE"       # En attente — mission non démarrée
    RUNNING   = "RUNNING"    # Mission en cours
    PAUSED    = "PAUSED"     # Mission en pause
    COMPLETED = "COMPLETED"  # Mission terminée avec succès
    FAILED    = "FAILED"     # Mission échouée

# ============================================================
# CLASSE MISSION PLANNER
# ============================================================
class MissionPlanner(Node):

    def __init__(self):
        super().__init__("mission_planner")

        # --------------------------------------------------------
        # CALLBACK GROUP — pour les appels asynchrones
        # --------------------------------------------------------
        self.cb_group_ = ReentrantCallbackGroup()
        # ReentrantCallbackGroup permet d'appeler des services
        # et des actions depuis des callbacks

        # --------------------------------------------------------
        # DÉCLARER LES PARAMÈTRES
        # --------------------------------------------------------
        self.declare_parameter("mission_file", "")
        # Chemin vers le fichier YAML de mission
        # Si vide → utiliser le fichier par défaut

        self.declare_parameter("auto_start", False)
        # false : attendre un service pour démarrer
        # true  : démarrer automatiquement au lancement

        # Lire les paramètres
        mission_file = self.get_parameter("mission_file").get_parameter_value().string_value
        auto_start   = self.get_parameter("auto_start").get_parameter_value().bool_value

        # --------------------------------------------------------
        # CHARGER LE FICHIER DE MISSION
        # --------------------------------------------------------
        if not mission_file:
            # Utiliser le fichier par défaut dans le package
            pkg_path     = get_package_share_directory('amr_controller')
            mission_file = os.path.join(pkg_path, 'config', 'mission.yaml')

        self.mission_config_ = self.loadMission(mission_file)
        # Charger la configuration depuis le fichier YAML

        # --------------------------------------------------------
        # ÉTAT INITIAL
        # --------------------------------------------------------
        self.state_           = MissionState.IDLE
        self.current_wp_idx_  = 0
        # Index du waypoint actuel dans la liste
        self.retry_count_     = 0
        # Nombre de tentatives pour le waypoint actuel
        self.mission_stats_   = {
            'total': 0,      # nombre total de waypoints
            'reached': 0,    # waypoints atteints avec succès
            'failed': 0,     # waypoints échoués
            'skipped': 0     # waypoints ignorés
        }

        # --------------------------------------------------------
        # CLIENT D'ACTION NAV2
        # --------------------------------------------------------
        self.nav_client_ = ActionClient(
            self,
            NavigateToPose,
            'navigate_to_pose',
            callback_group=self.cb_group_
        )

        # --------------------------------------------------------
        # PUBLISHERS — publier l'état de la mission
        # --------------------------------------------------------

        self.state_pub_ = self.create_publisher(
            String,
            '/mission/state',
            10
        )
        # Publie l'état : IDLE, RUNNING, PAUSED, COMPLETED, FAILED

        self.waypoint_pub_ = self.create_publisher(
            Int32,
            '/mission/current_waypoint',
            10
        )
        # Publie l'index du waypoint actuel

        self.active_pub_ = self.create_publisher(
            Bool,
            '/mission/active',
            10
        )
        # Publie true si la mission est active

        # --------------------------------------------------------
        # SERVICES — contrôler la mission depuis le terminal
        # --------------------------------------------------------

        self.start_srv_ = self.create_service(
            Trigger,
            '/mission/start',
            self.startMissionCallback,
            callback_group=self.cb_group_
        )
        # Service pour démarrer la mission
        # ros2 service call /mission/start std_srvs/srv/Trigger

        self.stop_srv_ = self.create_service(
            Trigger,
            '/mission/stop',
            self.stopMissionCallback,
            callback_group=self.cb_group_
        )
        # Service pour arrêter la mission
        # ros2 service call /mission/stop std_srvs/srv/Trigger

        self.pause_srv_ = self.create_service(
            SetBool,
            '/mission/pause',
            self.pauseMissionCallback,
            callback_group=self.cb_group_
        )
        # Service pour mettre en pause / reprendre
        # ros2 service call /mission/pause std_srvs/srv/SetBool "{data: true}"
        # ros2 service call /mission/pause std_srvs/srv/SetBool "{data: false}"

        # --------------------------------------------------------
        # TIMER DE PUBLICATION D'ÉTAT
        # --------------------------------------------------------
        self.status_timer_ = self.create_timer(
            1.0,
            # Publier l'état toutes les secondes
            self.publishStatus,
            callback_group=self.cb_group_
        )

        self.get_logger().info("=" * 50)
        self.get_logger().info("🤖 Mission Planner started !")
        self.get_logger().info(
            f"📋 Mission : {self.mission_config_['mission']['name']}"
        )
        self.get_logger().info(
            f"📍 Waypoints : {len(self.mission_config_['mission']['waypoints'])}"
        )
        self.get_logger().info("=" * 50)

        # --------------------------------------------------------
        # DÉMARRAGE AUTOMATIQUE
        # --------------------------------------------------------
        if auto_start:
            # Attendre 3 secondes que Nav2 soit prêt puis démarrer
            self.create_timer(
                3.0,
                self.autoStart,
                callback_group=self.cb_group_
            )

    # ============================================================
    # CHARGER LA MISSION DEPUIS LE FICHIER YAML
    # ============================================================
    def loadMission(self, filepath):
        try:
            with open(filepath, 'r') as f:
                config = yaml.safe_load(f)
                # yaml.safe_load : parser le fichier YAML en dict Python
            self.get_logger().info(f"✅ Mission loaded from: {filepath}")
            self.mission_stats_['total'] = len(config['mission']['waypoints'])
            return config
        except Exception as e:
            self.get_logger().error(f"❌ Failed to load mission: {e}")
            # Retourner une mission vide si le fichier n'existe pas
            return {'mission': {'name': 'Empty', 'waypoints': [],
                               'loop': False, 'max_retries': 2,
                               'wait_between_waypoints': 1.0}}

    # ============================================================
    # DÉMARRAGE AUTOMATIQUE
    # ============================================================
    def autoStart(self):
        self.get_logger().info("Auto-start enabled — starting mission...")
        self.startMission()
        # Annuler le timer auto-start après le premier appel
        return

    # ============================================================
    # CALLBACKS DES SERVICES
    # ============================================================

    def startMissionCallback(self, request, response):
        # Callback du service /mission/start
        if self.state_ == MissionState.IDLE or \
           self.state_ == MissionState.COMPLETED or \
           self.state_ == MissionState.FAILED:
            self.startMission()
            response.success = True
            response.message = "Mission started !"
        else:
            response.success = False
            response.message = f"Cannot start — current state: {self.state_}"
        return response

    def stopMissionCallback(self, request, response):
        # Callback du service /mission/stop
        self.stopMission()
        response.success = True
        response.message = "Mission stopped !"
        return response

    def pauseMissionCallback(self, request, response):
        # Callback du service /mission/pause
        # request.data = True → pause
        # request.data = False → reprendre
        if request.data:
            if self.state_ == MissionState.RUNNING:
                self.state_ = MissionState.PAUSED
                self.get_logger().info("⏸️  Mission paused !")
                response.success = True
                response.message = "Mission paused !"
            else:
                response.success = False
                response.message = "Mission is not running !"
        else:
            if self.state_ == MissionState.PAUSED:
                self.state_ = MissionState.RUNNING
                self.get_logger().info("▶️  Mission resumed !")
                # Continuer depuis le waypoint actuel
                self.executeWaypoint()
                response.success = True
                response.message = "Mission resumed !"
            else:
                response.success = False
                response.message = "Mission is not paused !"
        return response

    # ============================================================
    # DÉMARRER LA MISSION
    # ============================================================
    def startMission(self):
        if not self.nav_client_.wait_for_server(timeout_sec=5.0):
            self.get_logger().error("❌ Nav2 not available !")
            self.state_ = MissionState.FAILED
            return

        # Réinitialiser l'état
        self.state_          = MissionState.RUNNING
        self.current_wp_idx_ = 0
        self.retry_count_    = 0
        self.mission_stats_  = {
            'total':   len(self.mission_config_['mission']['waypoints']),
            'reached': 0,
            'failed':  0,
            'skipped': 0
        }

        self.get_logger().info("🚀 Mission started !")
        self.get_logger().info(
            f"📍 {self.mission_stats_['total']} waypoints to visit"
        )

        # Exécuter le premier waypoint
        self.executeWaypoint()

    # ============================================================
    # ARRÊTER LA MISSION
    # ============================================================
    def stopMission(self):
        self.state_ = MissionState.IDLE
        self.current_wp_idx_ = 0
        self.retry_count_    = 0
        self.get_logger().info("⏹️  Mission stopped !")

    # ============================================================
    # EXÉCUTER LE WAYPOINT ACTUEL
    # ============================================================
    def executeWaypoint(self):

        # Vérifier si la mission est en pause
        if self.state_ == MissionState.PAUSED:
            self.get_logger().info("⏸️  Mission paused — waiting...")
            return

        waypoints = self.mission_config_['mission']['waypoints']

        # Vérifier si tous les waypoints ont été visités
        if self.current_wp_idx_ >= len(waypoints):
            self.missionComplete()
            return

        # Récupérer le waypoint actuel
        wp = waypoints[self.current_wp_idx_]

        self.get_logger().info(
            f"\n{'='*40}\n"
            f"📍 Waypoint {self.current_wp_idx_+1}/{len(waypoints)}\n"
            f"   Name : {wp['name']}\n"
            f"   Position : ({wp['x']:.2f}, {wp['y']:.2f})\n"
            f"   Yaw : {wp['yaw']:.2f} rad\n"
            f"{'='*40}"
        )

        # --------------------------------------------------------
        # CONSTRUIRE LE GOAL NAV2
        # --------------------------------------------------------
        goal_msg = NavigateToPose.Goal()
        goal_msg.pose.header.frame_id = "map"
        goal_msg.pose.header.stamp    = self.get_clock().now().to_msg()

        # Position
        goal_msg.pose.pose.position.x = float(wp['x'])
        goal_msg.pose.pose.position.y = float(wp['y'])
        goal_msg.pose.pose.position.z = 0.0

        # Orientation (yaw → quaternion)
        yaw = float(wp['yaw'])
        goal_msg.pose.pose.orientation.z = math.sin(yaw / 2.0)
        goal_msg.pose.pose.orientation.w = math.cos(yaw / 2.0)

        # --------------------------------------------------------
        # ENVOYER LE GOAL
        # --------------------------------------------------------
        send_goal_future = self.nav_client_.send_goal_async(
            goal_msg,
            feedback_callback=self.feedbackCallback
        )

        # Ajouter le callback de réponse
        send_goal_future.add_done_callback(self.goalResponseCallback)

    # ============================================================
    # CALLBACK RÉPONSE DU GOAL
    # ============================================================
    def goalResponseCallback(self, future):
        goal_handle = future.result()

        if not goal_handle.accepted:
            self.get_logger().error(
                f"❌ Waypoint {self.current_wp_idx_+1} rejected !"
            )
            self.handleFailure()
            return

        self.get_logger().info(
            f"✅ Waypoint {self.current_wp_idx_+1} accepted !"
        )

        # Attendre le résultat final
        result_future = goal_handle.get_result_async()
        result_future.add_done_callback(self.resultCallback)

    # ============================================================
    # CALLBACK FEEDBACK
    # ============================================================
    def feedbackCallback(self, feedback_msg):
        dist = feedback_msg.feedback.distance_remaining
        if dist > 0.1:
            # Afficher seulement si distance > 10cm pour réduire le bruit
            self.get_logger().info(
                f"🔄 Waypoint {self.current_wp_idx_+1} — "
                f"Distance: {dist:.2f}m",
                throttle_duration_sec=2.0
                # Afficher toutes les 2 secondes maximum
            )

    # ============================================================
    # CALLBACK RÉSULTAT
    # ============================================================
    def resultCallback(self, future):
        from rclpy.action import GoalStatus
        # GoalStatus : états possibles d'un goal

        status = future.result().status

        if status == GoalStatus.STATUS_SUCCEEDED:
            # Waypoint atteint avec succès
            wp = self.mission_config_['mission']['waypoints'][self.current_wp_idx_]
            self.get_logger().info(
                f"🎯 Waypoint '{wp['name']}' reached !"
            )
            self.mission_stats_['reached'] += 1
            self.retry_count_ = 0
            # Réinitialiser les tentatives pour le prochain waypoint

            # Attendre avant le prochain waypoint
            wait_time = self.mission_config_['mission']['wait_between_waypoints']
            self.get_logger().info(
                f"⏳ Waiting {wait_time}s before next waypoint..."
            )
            self.create_timer(
                wait_time,
                self.nextWaypoint
            )

        else:
            # Waypoint non atteint
            self.get_logger().warn(
                f"⚠️  Waypoint {self.current_wp_idx_+1} "
                f"failed (status: {status})"
            )
            self.handleFailure()

    # ============================================================
    # GÉRER LES ÉCHECS
    # ============================================================
    def handleFailure(self):
        max_retries = self.mission_config_['mission']['max_retries']

        if self.retry_count_ < max_retries:
            # Réessayer le même waypoint
            self.retry_count_ += 1
            self.get_logger().warn(
                f"🔁 Retrying waypoint {self.current_wp_idx_+1} "
                f"({self.retry_count_}/{max_retries})..."
            )
            self.create_timer(2.0, self.retryWaypoint)
        else:
            # Dépasser le nombre max de tentatives → passer au suivant
            self.get_logger().error(
                f"❌ Waypoint {self.current_wp_idx_+1} "
                f"failed after {max_retries} retries — skipping !"
            )
            self.mission_stats_['failed']  += 1
            self.mission_stats_['skipped'] += 1
            self.retry_count_ = 0
            self.current_wp_idx_ += 1
            self.executeWaypoint()

    # ============================================================
    # PASSER AU WAYPOINT SUIVANT
    # ============================================================
    def nextWaypoint(self):
        self.current_wp_idx_ += 1
        self.executeWaypoint()
        return  # Annuler le timer one-shot

    # ============================================================
    # RÉESSAYER LE WAYPOINT ACTUEL
    # ============================================================
    def retryWaypoint(self):
        self.executeWaypoint()
        return  # Annuler le timer one-shot

    # ============================================================
    # MISSION COMPLÈTE
    # ============================================================
    def missionComplete(self):
        loop = self.mission_config_['mission']['loop']

        self.get_logger().info("=" * 50)
        self.get_logger().info("🏁 MISSION COMPLETE !")
        self.get_logger().info(
            f"📊 Stats:\n"
            f"   Total    : {self.mission_stats_['total']}\n"
            f"   Reached  : {self.mission_stats_['reached']} ✅\n"
            f"   Failed   : {self.mission_stats_['failed']} ❌\n"
            f"   Skipped  : {self.mission_stats_['skipped']} ⏭️"
        )
        self.get_logger().info("=" * 50)

        if loop:
            # Répéter la mission depuis le début
            self.get_logger().info("🔁 Loop mode — restarting mission...")
            self.current_wp_idx_ = 0
            self.retry_count_    = 0
            self.executeWaypoint()
        else:
            self.state_ = MissionState.COMPLETED

    # ============================================================
    # PUBLIER L'ÉTAT DE LA MISSION
    # ============================================================
    def publishStatus(self):
        # Publier l'état texte
        state_msg = String()
        state_msg.data = self.state_
        self.state_pub_.publish(state_msg)

        # Publier l'index du waypoint actuel
        wp_msg = Int32()
        wp_msg.data = self.current_wp_idx_
        self.waypoint_pub_.publish(wp_msg)

        # Publier si la mission est active
        active_msg = Bool()
        active_msg.data = (self.state_ == MissionState.RUNNING)
        self.active_pub_.publish(active_msg)

# ============================================================
# MAIN
# ============================================================
def main(args=None):
    rclpy.init(args=args)

    # MultiThreadedExecutor : nécessaire pour ReentrantCallbackGroup
    # Permet d'exécuter plusieurs callbacks en parallèle
    from rclpy.executors import MultiThreadedExecutor
    executor = MultiThreadedExecutor()

    node = MissionPlanner()
    executor.add_node(node)

    try:
        executor.spin()
        # spin avec MultiThreadedExecutor au lieu de rclpy.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()
