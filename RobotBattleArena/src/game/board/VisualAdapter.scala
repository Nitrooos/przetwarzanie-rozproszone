package game.board

object VisualAdapter {
  private type FieldChunk = List[String]
  private type Row = List[Field]

  private val chunks = Map(
    'ground      -> List("    ",
                         "    "),
    'wall        -> List("::::",
                         "::::"),
    'robotUp     -> List(" |  ",
                         "|x| "),
    'robotDown   -> List("|x| ",
                         " |  "),
    'robotLeft   -> List("--x ",
                         "    "),
    'robotRight  -> List("x-- ",
                         "    "),
    'bulletUp    -> List(" ^  ",
                         "    "),
    'bulletDown  -> List("    ",
                         " v  "),
    'bulletLeft  -> List(" <  ",
                         "    "),
    'bulletRight -> List("  > ",
                         "    ")
  )

  private val field_chunk = Map(
    'width  -> 4,
    'height -> 2
  )

  def visualise(board: Board): String = {
    val chunks_board =
      List.fill(board.fields.length)(List[FieldChunk]()).zipWithIndex.map {
        case (row, y) => List.fill(board.fields(y).length)(chunks('ground)).zipWithIndex.map {
          case (field, x) =>
            set_proper_chunk(board, x, y)
        }
      }

    chunks_board.map(chunks_row => visualise_chunks_row(chunks_row)).mkString("\n")
  }

  private def visualise_chunks_row(row: List[FieldChunk]): String =
    List.fill(field_chunk('height))("").zipWithIndex.map({
      case (line, i) => row.map(chunk => chunk(i)).mkString
    }).mkString("\n")

  private def set_proper_chunk(board: Board, x: Int, y: Int): FieldChunk = {
    val robot_on_field =  board.robots.find(robot => robot.x == x && robot.y == y)
    val bullet_on_field = board.bullets.find(bullet => bullet.x == x && bullet.y == y)

    robot_on_field match {
      case Some(robot) => colorize(chunks(robot.visualise), robot.color)
      case None => {
        bullet_on_field match {
          case Some(bullet) => chunks(bullet.visualise)
          case None => chunks(board.fields(y)(x).tag)
        }
      }
    }
  }

  private def colorize(chunk: FieldChunk, color: Int): FieldChunk = {
    chunk.map(line => "\033[" + color + "m" + line + "\033[m")
  }
}
