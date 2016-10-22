package game.robot

import game.algorithm.Rotation.Rotation
import game.algorithm.{Rotation, Move, Algorithm}
import game.board.Board

case class Robot(color: Int, x: Int, y: Int, dir: Rotation, AI: Algorithm) {
  def next_move(board: Board): Move = AI.next_move(this, board)

  def move(moves: List[Move]): Robot = {
    val my_moves = moves.filter(move => move.from == (x, y))
    my_moves match {
      case move :: _ => new Robot(color, move.to._1, move.to._2, move.rotation, AI)
      case Nil => this
    }
  }

  def fire: Move = {
    new Move((x, y), (x, y), true, dir)
  }

  def rotate(clockwise: Boolean): Move = {
    if (clockwise) {
      dir match {
        case Rotation.UP    => new Move((x, y), (x, y), false, Rotation.RIGHT)
        case Rotation.RIGHT => new Move((x, y), (x, y), false, Rotation.DOWN)
        case Rotation.DOWN  => new Move((x, y), (x, y), false, Rotation.LEFT)
        case Rotation.LEFT  => new Move((x, y), (x, y), false, Rotation.UP)
      }
    } else {
      dir match {
        case Rotation.UP    => new Move((x, y), (x, y), false, Rotation.LEFT)
        case Rotation.RIGHT => new Move((x, y), (x, y), false, Rotation.UP)
        case Rotation.DOWN  => new Move((x, y), (x, y), false, Rotation.RIGHT)
        case Rotation.LEFT  => new Move((x, y), (x, y), false, Rotation.DOWN)
      }
    }
  }

  def rotate(rotation: Rotation): Move = {
    new Move((x, y), (x, y), false, rotation)
  }

  def go(fire: Boolean): Move = {
    dir match {
      case Rotation.UP    => new Move((x, y), (x, y - 1), fire, Rotation.UP)
      case Rotation.RIGHT => new Move((x, y), (x + 1, y), fire, Rotation.RIGHT)
      case Rotation.DOWN  => new Move((x, y), (x, y + 1), fire, Rotation.DOWN)
      case Rotation.LEFT  => new Move((x, y), (x - 1, y), fire, Rotation.LEFT)
    }
  }

  def visualise: Symbol = dir match {
    case Rotation.LEFT  => 'robotLeft
    case Rotation.RIGHT => 'robotRight
    case Rotation.UP    => 'robotUp
    case Rotation.DOWN  => 'robotDown
  }
}
