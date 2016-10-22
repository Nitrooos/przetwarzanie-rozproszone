package game

import actor._
import akka.pattern.ask
import akka.util.Timeout
import consensus.{NextMovesProposition, ArrangeRobotsProposition, BoardProposition}
import game.board.Board
import ui.Helpers

import scala.concurrent.Await
import scala.concurrent.duration._
import scala.annotation.tailrec
import scala.io.StdIn

object Game {
  implicit val timeout = Timeout(60 seconds)

  def prepare_and_run(player: Player) = {
    val game = prepare(player)
    val points = run(game)
    println("Zdobyłeś " + points + " punktów")
  }

  private def prepare(player: Player) = {
    // użytkownik wybiera opcje gry
    val board_proposition = collect_board_proposition
    println("Oczekiwanie na decyzje innych graczy...")

    // uzgodnienie wspólnej planszy
    player.arenaActor ? ChangeConsensusToBoard
    val board_future = player.arenaActor ? new StartConsensus(board_proposition)
    val negotiated_board = Await.result(board_future, Duration.Inf).asInstanceOf[BoardProposition]
    new Game(player, negotiated_board)
  }

  @tailrec
  def collect_board_proposition: BoardProposition = {
    val boards_dir = "src/config/boards/"
    val available_boards = Board.available(boards_dir)
    println("")
    available_boards.foreach { case (no, name) => println(no + ". " + name) }

    val board_no = StdIn.readLine("Wybierz numer planszy do gry: ")
    val rounds_no = StdIn.readLine("Podaj liczbę rund gry: ")
    val robots_per_turn = StdIn.readLine("Ile robotów ma mieć gracz do dyspozycji na początku rundy?: ")

    try {
      new BoardProposition(boards_dir + available_boards(board_no.toInt), rounds_no.toInt, robots_per_turn.toInt)
    } catch {
      case e: NumberFormatException => {
        println("Podaj numeryczne wartości planszy, liczby rund i liczby robotów na turę!")
        collect_board_proposition
      }
    }
  }

  private def run(game: Game): Int = {
    // sumuj liczbę punktów zdobytą w poszczególnych rundach
    List.tabulate(game.start_values.rounds)(round_no =>
      game.run_round(round_no + 1)
    ).sum
  }
}

case class Game(player: Player, start_values: BoardProposition) {
  implicit val timeout = Timeout(10 seconds)
  val board = Board.load(start_values.board_file)

  def run_round(round_no: Int): Int = {
    @tailrec
    def choose_moves(cur_board: Board, points: Int = 0): Int = {
      val moves_in_turn = cur_board.robots.map(robot => robot.next_move(cur_board))
      val moves_future = player.arenaActor ? new StartConsensus(new NextMovesProposition(moves_in_turn))
      val negotiated_moves = Await.result(moves_future, Duration.Inf).asInstanceOf[NextMovesProposition]
      val board_after_turn = cur_board.apply_moves(negotiated_moves.moves)

      visualise(board_after_turn, points, round_no)

      board_after_turn.robots match {
        case r :: Nil => points                           // ostatni kończy rundę
        case _ => choose_moves(board_after_turn, points +
                                                 board_after_turn.robots.filter(robot => robot.color == player.color).length)
      }
    }

    player.arenaActor ? ChangeConsensusToArrangement
    val board_with_robots = agree_robot_arrangement(board.arrange_robots(player, start_values.robots_per_turn))
    player.arenaActor ? ChangeConsensusToMoves
    Thread.sleep(100)

    visualise(board_with_robots, 0, 1)
    choose_moves(board_with_robots)
  }

  private def agree_robot_arrangement(pre_arrangement: Board): Board = {
    val arrangement_future = player.arenaActor ? new StartConsensus(new ArrangeRobotsProposition(pre_arrangement))
    val negotiated_arrangement = Await.result(arrangement_future, Duration.Inf).asInstanceOf[ArrangeRobotsProposition]

    negotiated_arrangement.board
  }

  private def visualise(board: Board, points: Int, round_no: Int) = {
    println(board.visualise)
    println("Twój wynik: " + points)
    println("Runda: " + round_no)
    Thread.sleep(1000)
    Helpers.clear
  }
}