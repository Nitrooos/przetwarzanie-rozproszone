package game

import actor.ArenaActor
import akka.actor.{Props, ActorSystem}
import com.typesafe.config.ConfigFactory

case class Player(name: String, clients_no: Int) {
  private val colors = Map[String, Int] (
    "Player0" -> 31,
    "Player1" -> 32,
    "Player2" -> 33,
    "Player3" -> 34
  )

  val clients_connected = clients_no.toInt
  val config = ConfigFactory.load().getConfig(name)
  val actorSystem = ActorSystem("LocalArenaSystem", config)
  val arenaActor = actorSystem.actorOf(Props(new ArenaActor(clients_connected)), "ArenaActor")
  val color = colors(name)
}
