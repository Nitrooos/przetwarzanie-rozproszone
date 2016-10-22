package consensus

import game.algorithm.Move
import game.board.{Field, Board}
import game.robot.Robot

import scala.collection.immutable.SortedMap
import scala.collection.mutable.Set

trait Proposition

object BoardProposition {
  def make_choice(proposals: Set[BoardProposition]): BoardProposition = {
    val proposed_maps = proposals.map(p => p.board_file).groupBy(identity).mapValues(_.size)
    val chosen_map = SortedMap(proposed_maps.toSeq:_*).firstKey

    val rounds_no = Math.round(proposals.toList.map(_.rounds).sum / proposals.size)
    val robots_per_turn = Math.round(proposals.toList.map(_.robots_per_turn).sum / proposals.size)

    new BoardProposition(chosen_map, rounds_no, robots_per_turn)
  }
}

object ArrangeRobotsProposition {
  def make_choice(proposals: Set[ArrangeRobotsProposition]): ArrangeRobotsProposition = {
    val naive_merged = proposals.foldLeft(List[Robot]())((acc, prop) => acc ::: prop.board.robots)
    val merged = naive_merged
      .foldLeft(List[Robot]())((acc, robot) => acc.find(r => r.x == robot.x && r.y == robot.y) match {
        case Some(_) => {
          val alternative_field = relocate(robot, proposals.head.board, naive_merged)
          alternative_field match {
            case Some(field) => new Robot(robot.color, field.x, field.y, robot.dir, robot.AI) :: acc
            case None => acc
          }
        }
        case None => robot :: acc
      })
    new ArrangeRobotsProposition(new Board(proposals.head.board.fields, merged))
  }

  private def relocate(robot: Robot, board: Board, all_robots: List[Robot]): Option[Field] = {
    board.fields
      .flatten
      .map(field => field -> (Math.abs(field.x - robot.x) + Math.abs(field.y - robot.y)))
      .filter {
        case (_, distance) if distance > 0 => true
        case (_, _) => false
      }
      .sortBy { case (_, distance) => distance }
      .find { case (field, _) => field.tag == 'ground && !all_robots.exists(r => r.x == field.x && r.y == field.y) }
      .map { case (field, _) => field }
  }
}

object NextMovesProposition {
  def make_choice(proposals: Set[NextMovesProposition]): NextMovesProposition = {
    val merged = proposals.foldLeft(List[Move]())((acc, prop) => acc ::: prop.moves)
      .foldLeft(List[Move]())((acc, move) => acc.find(m => m.to._1 == move.to._1 && m.to._2 == move.to._2) match {
        case Some(_) => acc
        case None => move :: acc
      })
    new NextMovesProposition(merged)
  }
}

case class BoardProposition(board_file: String, rounds: Int, robots_per_turn: Int) extends Proposition
case class ArrangeRobotsProposition(board: Board) extends Proposition
case class NextMovesProposition(moves: List[Move]) extends Proposition