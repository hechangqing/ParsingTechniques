#include "graph.h"

#include <cassert>
#include <iostream>

int State::add_arc(const Arc &arc) {
  arcs_[arc.ilabel].push_back(arc);
  return 0;
}

int Graph::add_state() {
  int state_id = state_cnt_;
  state_cnt_ += 1;
  return state_id;
}

int Graph::set_start(int start_state_id) {
  start_state_id_ = start_state_id;
  return 0;
}

int Graph::set_final(int final_state_id) {
  final_states_.insert(final_state_id);
  return 0;
}

int Graph::add_arc(int src_state_id, const Arc &arc) {
  states_[src_state_id].add_arc(arc);
  return 0;
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
  return 0;
}

int fa_proper_sequence(Graph *fa) {
  return 0;
}

int fa_optinal(Graph *fa) {
  return 0;
}

int concate_fa(const Graph &left, const Graph &right, Graph *fa) {
  return 0;
}

int fa_alternative(const std::vector<Graph> &nfa_graph, Graph *nfa) {
  return 0;
}

int fa_char_set(const std::set<int> &char_set, Graph *nfa) {
  return 0;
}
