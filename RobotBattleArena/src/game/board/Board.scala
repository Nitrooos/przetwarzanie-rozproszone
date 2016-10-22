package game.board

import java.io.File

import game.Player
import game.algorithm._
import game.robot.{Bullet, Robot}

import scala.annotation.tailrec
import scala.util.Random
import scala.io.Source

object Board {
  def available(dir: String): Map[Int, String] = {
    val d = new File(dir)
    if (d.exists && d.isDirectory) {
      d.listFiles.filter(_.isFile).zipWithIndex.map { case (file, no) => no + 1 -> file.getName() }.toMap
    } else {
      Map[Int, String]()
    }
  }

  def load(file: String): Board = {
    val source = Source.fromFile(file)
    try {
      val b = source.mkString.split("\n").toList.zipWithIndex.map {
        case (row, y) => row.zipWithIndex.map {
          case (char, x) => Field.create(char, x, y)
        }.toList
      }
      new Board(b)
    } finally source.close()
  }
}

case class Board(fields: List[List[Field]], robots: List[Robot] = List(), bullets: List[Bullet] = List()) {
  def arrange_robots(player: Player, robots_no: Int): Board = {
    val rand = new Random(System.currentTimeMillis())
    val flat_fields = fields.flatten

    @tailrec
    def choose_free_field: Field = {
      val random_field = flat_fields(rand.nextInt(flat_fields.length))
      if (random_field.tag == 'ground && !robots.exists(robot => robot.x == random_field.x && robot.y == random_field.y)) {
        random_field
      } else {
        choose_free_field
      }
    }

    val arranged_robots = List.tabulate(robots_no) { no =>
      val field = choose_free_field
      if (player.name == "Player0") {
        new Robot(player.color, field.x, field.y, Rotation.UP, new SuperIntelligence())
        //new Robot(player.color, field.x, field.y, Rotation.UP, new RandomAI())
      } else {
        new Robot(player.color, field.x, field.y, Rotation.UP, new RandomAI())
      }
    }
    new Board(fields, arranged_robots)
  }

  def apply_moves(moves: List[Move]): Board = {
    val shot_bullets = moves.filter(move => move.shot == true)
      .map(move => new Bullet(move.to._1, move.to._2, move.rotation))
    val bullets_after_moves = (bullets ::: shot_bullets)
      .filterNot(bullet => fields(bullet.y)(bullet.x).tag == 'wall)
      .map(bullet => bullet.move)
      .filterNot(b => b.x < 0 || b.x >= fields.length || b.y < 0 || b.y >= fields.length)
      .filterNot(bullet => fields(bullet.y)(bullet.x) match {
        case field: Field if field.tag == 'wall => true
        case _ => false
      })

    val valid_moves = moves.filter(move => fields(move.to._2)(move.to._1).tag == 'ground)
    val robots_after_moves = robots.map(robot => robot.move(valid_moves))
      .map { case robot =>
        robot -> bullets_after_moves.filter(
          bullet => robot.x == bullet.x && robot.y == bullet.y)
      }.toMap

    val alive_after_moves = robots_after_moves.filter {
      case (robot, List()) => true
      case (_, _) => false
    }
    val active_bullets = bullets_after_moves.filterNot(bullet => robots_after_moves.values.exists(list => list contains bullet))

    new Board(fields, alive_after_moves.keys.toList, active_bullets)
  }

  def visualise: String =
    VisualAdapter.visualise(this)
}