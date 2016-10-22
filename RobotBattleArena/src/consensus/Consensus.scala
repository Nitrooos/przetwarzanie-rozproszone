package consensus

import actor._
import akka.actor._
import scala.collection.mutable.Set

class ConsensusActor[T >: Null <: Proposition](parent: ArenaActor, nodes: List[ActorRef], choice_fun: Set[T] => T) extends Actor {
  var decided: T = null
  var proposal_set = Set[T]()
  var correct_this_round = Set[ActorRef]()
  var correct_last_round = Set[ActorRef]()
  var round_no = 1
  var awaiting: ActorRef = null
`
  def receive = {
    case start: StartConsensus[T] => {
      cleanup
      awaiting = sender
      proposal_set += start.value
      nodes.foreach { case node => node.tell(new PropositionMessage(round_no, proposal_set.toSet), parent.self) }
    }
    case proposition: PropositionMessage[T] => {
      proposal_set = proposal_set ++ proposition.values
      correct_this_round += sender
      self ! new CheckEndCondition
    }
    case _: CheckEndCondition => {
      val correct_nodes = parent.correct
      if ((correct_nodes & correct_this_round) == correct_nodes && decided == null) {
        if (correct_this_round == correct_last_round) {
          decided = choice_fun(proposal_set)
          nodes.foreach { case node => node.tell(new DecisionMessage(round_no, decided), parent.self) }
        } else {
          round_no += 1
          correct_last_round = correct_this_round
          correct_this_round = Set()
          nodes.foreach { case node => node.tell(new PropositionMessage(round_no, proposal_set.toSet), parent.self) }
        }
      }
    }
    case decision: DecisionMessage[T] => {
      if (decided == null) {
        decided = decision.value
        round_no += 1
        nodes.foreach { case node => node.tell(new DecisionMessage(round_no, decided), parent.self) }
      }
      if (awaiting != null) {
        awaiting ! decided
        awaiting = null
      }
    }
  }

  private def cleanup = {
    decided = null
    proposal_set.clear()
    correct_this_round.clear()
    correct_last_round.clear()
    round_no = 1
    awaiting = null
  }
}
