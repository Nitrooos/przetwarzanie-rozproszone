package game.robot

import game.algorithm.Rotation
import game.algorithm.Rotation.Rotation

case class Bullet(x: Int, y: Int, dir: Rotation) {
  def move: Bullet = {
    dir match {
      case Rotation.LEFT  => new Bullet(x - 1, y, Rotation.LEFT)
      case Rotation.RIGHT => new Bullet(x + 1, y, Rotation.RIGHT)
      case Rotation.UP    => new Bullet(x, y - 1, Rotation.UP)
      case Rotation.DOWN  => new Bullet(x, y + 1, Rotation.DOWN)
    }
  }

  def visualise: Symbol = dir match {
    case Rotation.LEFT  => 'bulletLeft
    case Rotation.RIGHT => 'bulletRight
    case Rotation.UP    => 'bulletUp
    case Rotation.DOWN  => 'bulletDown
  }
}
