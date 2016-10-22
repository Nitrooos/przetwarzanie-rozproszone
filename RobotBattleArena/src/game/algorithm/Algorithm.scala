package game.algorithm

import game.board.{Field, Board}
import game.robot.{Bullet, Robot}

import scala.annotation.tailrec
import scala.collection.immutable.Queue
import scala.util.Random

trait Algorithm {
  def next_move(robot: Robot, board: Board): Move
}

case class Lazyman() extends Algorithm {
  override def next_move(robot: Robot, board: Board): Move = {
    new Move((robot.x, robot.y), (robot.x, robot.y), false, robot.dir)
  }
}

case class RandomAI() extends Algorithm {
  override def next_move(robot: Robot, board: Board): Move = {
    new Random().nextFloat() match {
      case f if f < 0.1 => robot.rotate(true)
      case f if f < 0.2 => robot.rotate(false)
      case _ => robot.go(true)
    }
  }
}

case class SuperIntelligence() extends Algorithm {
  override def next_move(robot: Robot, board: Board): Move = {
    val rotation = step_to_nearest_enemy(robot, board) match {
      case field if field.x == robot.x && field.y  < robot.y => Rotation.UP
      case field if field.x == robot.x && field.y  > robot.y => Rotation.DOWN
      case field if field.x  < robot.x && field.y == robot.y => Rotation.LEFT
      case field if field.x  > robot.x && field.y == robot.y => Rotation.RIGHT
      case _ => robot.dir
    }

    if (robot.dir == rotation) {
      robot.go(true)
    } else {
      robot.rotate(rotation)
    }
  }

  private def is_field_safe(field: Field, board: Board): Boolean = {
    @tailrec
    def will_reach_field(bullet: Bullet): Boolean = {
      bullet.move match {
        case Bullet(field.x, field.y, _) => true
        case Bullet(x, y, _)
          if x < 0 || x >= board.fields(y).length ||
             y < 0 || y >= board.fields.length ||
             board.fields(y)(x).tag == 'wall => false
        case bullet => will_reach_field(bullet)
      }
    }

    board.bullets
      .filter(bullet => bullet.x == field.x || bullet.y == field.y)
      .filter(bullet => will_reach_field(bullet))
      .length == 0
  }

  private def step_to_nearest_enemy(from: Robot, board: Board): Field = {

    @tailrec
    def way_to(cur_field: Field, distances: Map[Field, Int], cur_list: List[Field] = Nil): List[Field] = {
      cur_field match {
        case dest if dest.x == from.x && dest.y == from.y => cur_list match {
          case Nil => Nil
          case l => l.tail
        }
        case _ =>
          distances
            .filter {
              case (field, _) if (Math.abs(field.x - cur_field.x) + Math.abs(field.y - cur_field.y)) == 1 => true
              case (_, _) => false
            }
            .find {
              case (field, dist) if dist == distances(cur_field) - 1 => true
              case (_, _) => false
            }
          match {
            case None => Nil
            case Some((next_field, _)) => {
              way_to(next_field, distances, next_field :: cur_list)
            }
          }
      }
    }

    val distances = distance_map(from, board)
    val maybe_nearest_enemy = board.robots
      .filter(robot => robot.color != from.color)
      .map(robot => robot -> distances(board.fields(robot.y)(robot.x)))
      .toSeq
      .sortBy(_._2)

    maybe_nearest_enemy.headOption match {
      case None => board.fields(from.y)(from.x)
      case Some(head) =>
      val nearest_enemy = head._1
      way_to(board.fields(nearest_enemy.y)(nearest_enemy.x), distances) match {
        case Nil => board.fields(from.y)(from.x)
        case x :: xs => x
      }
    }
  }

  private def distance_map(from: Robot, board: Board): Map[Field, Int] = {
    @tailrec
    def BFS(board: List[List[Field]], queue: Queue[(Field, Int)], cur_list: List[(Field, Int)] = Nil): Map[Field, Int] = {
      queue match {
        case (field, dist) +: queue_rest => {
          val adjacents = List(
            board(field.y - 1)(field.x), board(field.y + 1)(field.x), board(field.y)(field.x - 1), board(field.y)(field.x + 1)
          ) .filter(f => f.tag != 'wall)
            .filter(f => (cur_list ::: queue.toList).toMap.get(f).isEmpty)
            .map(f => (f, dist + 1))

          BFS(board, queue_rest.enqueue(adjacents), (field, dist) :: cur_list)
        }
        case _ => cur_list.toMap
      }
    }

    val root_field = board.fields(from.y)(from.x)
    BFS(board.fields, Queue((root_field, 0)))
  }
}
