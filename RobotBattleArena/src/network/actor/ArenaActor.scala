package actor

import akka.actor.{Props, Actor, ActorRef}
import consensus.{NextMovesProposition, ArrangeRobotsProposition, BoardProposition, ConsensusActor}
import akka.util.Timeout
import scala.language.postfixOps
import scala.concurrent.duration.DurationInt
import scala.concurrent.ExecutionContext.Implicits.global
import scala.collection.mutable.Map

class ArenaActor(clients_connected: Int) extends Actor {
  implicit val timeout = new Timeout(10 seconds)
  val AcceptableDelay = 10000
  var nodes = Map[ActorRef, Long]()
  var boardConsensusActor: ActorRef = null
  var arrangeRobotsActor: ActorRef = null
  var movesConsensusActor: ActorRef = null
  var activeConsensus: ActorRef = null

  def correct: Set[ActorRef] = {
    val cur_time = System.currentTimeMillis()
    nodes.filter { case (_, last_heartbeat) => cur_time - last_heartbeat < AcceptableDelay }.keys.toSet
  }

  def receive = {
    case "Heartbeat Time" => {
      nodes.foreach { case (actor, _) => actor ! Heartbeat(self) }
    }
    case Initialize(found_nodes: List[ActorRef]) => {
      val cur_time = System.currentTimeMillis()

      nodes = collection.mutable.Map(found_nodes.map(node => node -> cur_time).toMap.toSeq: _*)
      boardConsensusActor = context.actorOf(Props(new ConsensusActor(this, nodes.keys.toList, BoardProposition.make_choice)), "BoardConsensusActor")
      arrangeRobotsActor  = context.actorOf(Props(new ConsensusActor(this, nodes.keys.toList, ArrangeRobotsProposition.make_choice)), "ArrangeRobotsConsensusActor")
      movesConsensusActor = context.actorOf(Props(new ConsensusActor(this, nodes.keys.toList, NextMovesProposition.make_choice)), "NextMovesConsensusActor")
      activeConsensus = boardConsensusActor

      context.system.scheduler.schedule(5 seconds, 5 seconds, self, "Heartbeat Time")
    }
    case Heartbeat(actor) => {
      val cur_time = System.currentTimeMillis()
      val since_last = cur_time - nodes(actor)
      if (since_last < AcceptableDelay) {
        nodes(actor) = cur_time
      }
    }
    case ChangeConsensusToBoard => {
      activeConsensus = boardConsensusActor
      sender ! true
    }
    case ChangeConsensusToArrangement => {
      activeConsensus = arrangeRobotsActor
      sender ! true
    }
    case ChangeConsensusToMoves => {
      activeConsensus = movesConsensusActor
      sender ! true
    }
    case consensusMsg: ConsensusMessage => {
      activeConsensus forward consensusMsg
    }
  }
}