package game.board

object Field {
  def create(c: Char, x: Int, y: Int): Field = c match {
    case '0' => Ground(x, y)
    case '1' => Wall(x, y)
  }
}

trait Field {
  val x: Int
  val y: Int

  def tag: Symbol
}

case class Ground(x: Int, y: Int) extends Field {
  override def tag = 'ground
}

case class Wall(x: Int, y: Int) extends Field {
  override def tag = 'wall
}
