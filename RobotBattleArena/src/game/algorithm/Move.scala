package game.algorithm

import game.algorithm.Rotation.Rotation

object Rotation extends Enumeration {
  type Rotation = Value
  val UP, DOWN, LEFT, RIGHT = Value
}

case class Move(from: (Int, Int), to: (Int, Int), shot: Boolean, rotation: Rotation) { }
