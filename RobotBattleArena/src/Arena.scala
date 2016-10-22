import actor.Initialize
import akka.actor.{ActorRef, ActorSystem}
import akka.util.Timeout
import com.typesafe.config.ConfigFactory
import game.Player
import ui.{FinishedException, Menu}

import scala.concurrent.duration.DurationInt
import scala.annotation.tailrec
import scala.concurrent.Await
import scala.io.StdIn

// TODO
// 1. jakieś prościutkie, sensowne algorytmy poruszania się robotów
// 4. wybór algorytmów dla robotów ???
// 6. exception: cannot cast!!! (pojawiło się 1 raz przy grze 4. osobowej...)
// 7. pociski pojawiające się z "niczego"!!! (bo brak sleepa...)

object Arena extends App {

  override def main(args: Array[String]) = {
    try {
      if (args.length < 2) {
        throw new RuntimeException("Musisz podać nazwę gracza z pliku application.conf")
      }

      val player = new Player(args.head, args(1).toInt)

      StdIn.readLine("Wciśnij ENTER aby wyszukać podłączonych graczy...")
      player.arenaActor ! Initialize(initialize_nodes(player.actorSystem, player.clients_connected))

      val menu = new Menu(player)
      executeCommands(menu)
    } catch {
      case e: FinishedException => {
        println("Zamykanie programu...")
        System.exit(0)
      }
      case e: Throwable => {
        e.printStackTrace()
        System.exit(1)
      }
    }
  }

  @tailrec
  private def executeCommands(menu: Menu) {
    menu.refresh
    menu.react(StdIn.readLine("Wybierz numer opcji: ")).apply()
    executeCommands(menu)
  }

  private def initialize_nodes(system: ActorSystem, clients_connected: Int): List[ActorRef] = {
    implicit val timeout = Timeout(10 seconds)
    val config = ConfigFactory.load()
    List.tabulate(clients_connected)(n => {
      val remote_port = config.getInt("Player" + n + ".akka.remote.netty.tcp.port")
      val node_future = system.actorSelection(s"akka.tcp://LocalArenaSystem@127.0.0.1:${remote_port}/user/ArenaActor").resolveOne()
      Await.result(node_future, 10 seconds)
    })
  }
}