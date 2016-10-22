package ui

object Helpers {

  def clear =
    1 to 25 foreach { i => println }

  def clear2 =
    print("\u001b[2J")
}
