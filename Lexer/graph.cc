#include "graph.h"

#include <cassert>
#include <iostream>

int State::add_arc(const Arc &arc) {
  arcs_[arc.ilabel].push_back(arc);
  return 0;
}

State::ArcIter State::arcs_begin() {
  return arcs_.begin();
}

State::ConstArcIter State::arcs_begin() const {
  return arcs_.begin();
}

State::ArcIter State::arcs_end() {
  return arcs_.end();
}

State::ConstArcIter State::arcs_end() const {
  return arcs_.end();
}

int Graph::add_state() {
  int state_id = state_cnt_;
  state_cnt_ += 1;
  return state_id;
}

int Graph::max_state_id() const {
  return state_cnt_ - 1;
}

int Graph::num_states() const {
  return state_cnt_;
}

int Graph::set_start(int start_state_id) {
  start_state_id_ = start_state_id;
  return 0;
}

int Graph::get_start() const {
  return start_state_id_;
}

int Graph::set_final(int final_state_id) {
  final_states_.insert(final_state_id);
  return 0;
}

int Graph::remove_final(int final_state_id) {
  final_states_.erase(final_state_id);
  return 0;
}

int Graph::add_arc(int src_state_id, const Arc &arc) {
  states_[src_state_id].add_arc(arc);
  return 0;
}

Graph::FinalStatesIterator Graph::final_states_begin() {
  return final_states_.begin();
}

Graph::FinalStatesIterator Graph::final_states_end() {
  return final_states_.end();
}

int generate_one_char_fa(int ilabel, Graph *fa) {
  assert(fa != NULL);
  int start_state = fa->add_state();
  fa->set_start(start_state);
  int final_state = fa->add_state();
  fa->set_final(final_state);
  fa->add_arc(start_state, Arc(ilabel, final_state));
  return 0;
}

int fa_kleene_star(Graph *fa) {
  int new_start_and_final_state = fa->add_state();
  int orig_start_state = fa->get_start();
  fa->add_arc(new_start_and_final_state, Arc(Arc::EPSILON, orig_start_state));
  fa->set_start(new_start_and_final_state);

  std::vector<int> final_states_ids;
  for (Graph::FinalStatesIterator iter = fa->final_states_begin();
       iter != fa->final_states_end(); ++iter) {
    fa->add_arc(*iter, Arc(Arc::EPSILON, new_start_and_final_state));
    final_states_ids.push_back(*iter);
  }

  for (int i = 0; i < final_states_ids.size(); i++) {
    fa->remove_final(final_states_ids[i]);
  }

  fa->set_final(new_start_and_final_state);
  return 0;
}

int fa_proper_sequence(Graph *fa) {
  return 0;
}

int fa_optinal(Graph *fa) {
  for (Graph::FinalStatesIterator iter = fa->final_states_begin();
       iter != fa->final_states_end(); ++iter) {
    fa->add_arc(fa->get_start(), Arc(Arc::EPSILON, *iter));
  }
  return 0;
}

int Graph::concate_fa(const Graph &left, const Graph &right, Graph *fa) {
  *fa = left;
  int left_max_state_id = left.max_state_id();

  for (std::map<int, State>::const_iterator iter = right.states_.begin();
       iter != right.states_.end(); ++iter) {
    int this_state_id = iter->first;
    int new_state_id = this_state_id + left_max_state_id + 1;
    const State &this_state = iter->second;
    State::ConstArcIter arc_iter = this_state.arcs_begin();
    for (; arc_iter != this_state.arcs_end(); arc_iter++) {
      int ilabel = arc_iter->first;
      const std::vector<Arc> &this_ilabel_arcs = arc_iter->second;
      for (int i = 0; i < this_ilabel_arcs.size(); i++) {
        int new_next_state = this_ilabel_arcs[i].next_state + left_max_state_id + 1;
      }
    }
  }
  return 0;
}

int fa_alternative(const std::vector<Graph> &nfa_graph, Graph *nfa) {
  return 0;
}

int fa_char_set(const std::set<int> &char_set, Graph *nfa) {
  return 0;
}
