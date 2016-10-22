package actor

import akka.actor.ActorRef

abstract class Message

case class Heartbeat(actor: ActorRef) extends Message
case class Initialize(nodes: List[ActorRef]) extends Message
case class ChangeConsensusToBoard() extends Message
case class ChangeConsensusToArrangement() extends Message
case class ChangeConsensusToMoves() extends Message

abstract class ConsensusMessage extends Message
case class StartConsensus[T](value: T) extends ConsensusMessage
case class PropositionMessage[T](round_no: Int, values: Set[T]) extends ConsensusMessage
case class CheckEndCondition() extends ConsensusMessage
case class DecisionMessage[T](round_no: Int, value: T) extends ConsensusMessage