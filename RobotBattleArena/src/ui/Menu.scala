package ui

import akka.util.Timeout
import game.{Player, Game}

import scala.concurrent.duration.DurationInt
import scala.language.postfixOps

class Menu(player: Player) {

  implicit val timeout = new Timeout(10 seconds)

  val commands = Map(
    1 -> ("Nowa gra", () => start_new_game),
    2 -> ("O autorze", () => about),
    0 -> ("Wyjście",  () => exit)
  )

  def refresh = {
    println("----- Robot Battle Arena v0.1 -----")
    println("-----------------------------------")
    println("-- Menu:")
    commands.foreach {
      case (no, (name, command)) =>
        println("-- " + no + ": " + name)
    }
  }

  def react(input: String): () => Unit = {
    val invalid_input = () => println("Wybierz numer pozycji w menu")
    try {
      input.toInt match {
        case no if commands contains (no) => {
          val (_, action) = commands(no)
          action
        }
        case _ => invalid_input
      }
    } catch {
      case e: NumberFormatException => invalid_input
    }
  }

  private def start_new_game =
    Game.prepare_and_run(player)

  private def about =
    println("\nAutor: Bartosz Kostaniak, Politechnika Poznańska 2016\n")

  private def exit = {
    throw new FinishedException
  }
}

class FinishedException extends Throwable