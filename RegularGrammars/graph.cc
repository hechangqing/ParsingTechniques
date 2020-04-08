#include "graph.h"

#include <sstream>

int Graph::add_state() {
  int state_id = states_.size();
  states_.resize(state_id + 1);
  return state_id;
}

int Graph::add_arc(int state, const Arc &arc) {
  assert(state < states_.size());
  states_[state].add_arc(arc);
  return 0;
}

std::string Graph::to_string() const {
  std::stringstream ss;
  ss << "start " << start_state_ << "\n";
  for (std::map<int, int>::const_iterator iter = is_final_.begin(); iter != is_final_.end(); iter++) {
    ss << "final " << iter->first << " \n";
  }
  for (int i = 0; i < states_.size(); i++) {
    const State &state = states_[i];
    for (int j = 0; j < state.num_arcs(); j++) {
      const Arc &arc = state.get_arc(j);
      int ilabel = arc.get_ilabel();
      int next_state = arc.get_next_state();
      ss << i << " -> " << next_state << " ilabel " << ilabel << " \n";
    }
  }
  return ss.str();
}
