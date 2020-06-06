#include "graph.h"

#include <cassert>
#include <iostream>
#include <queue>

int State::add_arc(const Arc &arc) {
  arcs_[arc.ilabel].push_back(arc);
  return 0;
}

int State::remove_arcs_with_ilabel(int ilabel) {
  arcs_.erase(ilabel);
  return 0;
}

int State::num_arcs() const {
  return arcs_.size();
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

const std::vector<Arc> &State::get_arcs(int ilabel) const {
  assert(arcs_.find(ilabel) != arcs_.end());
  return arcs_.at(ilabel);
}

bool State::has_ilabel(int ilabel) const {
  return arcs_.find(ilabel) != arcs_.end();
}

int Graph::add_state() {
  int state_id = state_cnt_;
  if (state_id > max_state_id_) {
    max_state_id_ = state_id;
  }
  state_cnt_ += 1;
  return state_id;
}

int Graph::max_state_id() const {
  return max_state_id_;
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

bool Graph::is_final(int state_id) const {
  return final_states_.find(state_id) != final_states_.end();
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
  if (src_state_id > max_state_id_) {
    max_state_id_ = src_state_id;
  }
  if (arc.next_state > max_state_id_) {
    max_state_id_ = arc.next_state;
  }
  return 0;
}

Graph::FinalStatesIterator Graph::final_states_begin() {
  return final_states_.begin();
}

Graph::FinalStatesIterator Graph::final_states_end() {
  return final_states_.end();
}

const State &Graph::get_state(int state_id) const {
  assert(states_.find(state_id) != states_.end());
  return states_.at(state_id);
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

int Graph::fa_kleene_star(Graph *fa) {
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

int Graph::fa_proper_sequence(Graph *fa) {
  Graph left = *fa;
  Graph right = *fa;
  fa_kleene_star(&right);
  Graph out_fa;
  concate_fa(left, right, &out_fa);
  *fa = out_fa;
  return 0;
}

int Graph::fa_optinal(Graph *fa) {
  for (Graph::FinalStatesIterator iter = fa->final_states_begin();
       iter != fa->final_states_end(); ++iter) {
    fa->add_arc(fa->get_start(), Arc(Arc::EPSILON, *iter));
  }
  return 0;
}

int Graph::concate_fa(const Graph &left, const Graph &right, Graph *fa) {
  *fa = left;
  int left_max_state_id = left.max_state_id();

  int new_right_start_state_id = right.get_start() + left_max_state_id + 1;
  std::vector<int> new_final_state_vec;
  for (std::map<int, State>::const_iterator iter = right.states_.begin();
       iter != right.states_.end(); ++iter) {
    int this_state_id = iter->first;
    int new_state_id = this_state_id + left_max_state_id + 1;
    fa->add_state();
    if (right.is_final(this_state_id)) {
      new_final_state_vec.push_back(new_state_id);
    }
    const State &this_state = iter->second;
    State::ConstArcIter arc_iter = this_state.arcs_begin();
    for (; arc_iter != this_state.arcs_end(); arc_iter++) {
      int ilabel = arc_iter->first;
      const std::vector<Arc> &this_ilabel_arcs = arc_iter->second;
      for (int i = 0; i < this_ilabel_arcs.size(); i++) {
        int new_next_state = this_ilabel_arcs[i].next_state + left_max_state_id + 1;
        fa->add_arc(new_state_id, Arc(ilabel, new_next_state));
      }
    }
  }

  std::vector<int> orig_final_state_vec;
  for (Graph::FinalStatesIterator iter = fa->final_states_begin();
       iter != fa->final_states_end(); iter++) {
    fa->add_arc(*iter, Arc(Arc::EPSILON, new_right_start_state_id));
    orig_final_state_vec.push_back(*iter);
  }

  for (int i = 0; i < orig_final_state_vec.size(); i++) {
    fa->remove_final(orig_final_state_vec[i]);
  }
  for (int i = 0; i < new_final_state_vec.size(); i++) {
    fa->set_final(new_final_state_vec[i]);
  }
  return 0;
}

int Graph::fa_alternative(const std::vector<Graph> &fa_graph_vec, Graph *fa) {
  if (fa_graph_vec.size() == 0) {
  } else if (fa_graph_vec.size() == 1) {
    *fa = fa_graph_vec[0];
  } else {
    int start_state_id = fa->add_state();
    fa->set_start(start_state_id);
    int final_state_id = fa->add_state();
    fa->set_final(final_state_id);
    int cur_max_state_id = fa->max_state_id();
    std::vector<int> orig_final_state_ids;
    for (int i = 0; i < fa_graph_vec.size(); i++) {
      const Graph &this_fa = fa_graph_vec[i];
      cur_max_state_id = fa->max_state_id();
      for (std::map<int, State>::const_iterator iter = this_fa.states_.begin();
           iter != this_fa.states_.end(); iter++) {
        int this_state_id = iter->first;
        int new_state_id = this_state_id + cur_max_state_id + 1;
        fa->add_state();
        if (this_fa.get_start() == this_state_id) {
          fa->add_arc(start_state_id, Arc(Arc::EPSILON, new_state_id));
        }
        if (this_fa.is_final(this_state_id)) {
          orig_final_state_ids.push_back(new_state_id);
        }
        const State &this_state = iter->second;
        for (State::ConstArcIter arc_iter = this_state.arcs_begin();
             arc_iter != this_state.arcs_end(); arc_iter++) {
          int ilabel = arc_iter->first;
          const std::vector<Arc> &this_ilabel_arcs = arc_iter->second;
          for (int a = 0; a < this_ilabel_arcs.size(); a++) {
            int new_next_state = this_ilabel_arcs[i].next_state + cur_max_state_id + 1;
            fa->add_arc(new_state_id, Arc(ilabel, new_next_state));
          }
        }
      }
    }
    for (int i = 0; i < orig_final_state_ids.size(); i++) {
      fa->add_arc(orig_final_state_ids[i], Arc(Arc::EPSILON, final_state_id));
    }
  }
  return 0;
}

int fa_char_set(const std::set<int> &char_set, Graph *fa) {
  int start_state = fa->add_state();
  int final_state = fa->add_state();
  fa->set_start(start_state);
  for (std::set<int>::const_iterator iter = char_set.begin();
       iter != char_set.end(); iter++) {
    fa->add_arc(start_state, Arc(*iter, final_state));
  }
  fa->set_final(final_state);
  return 0;
}

int eps_cloure(const Graph &graph, int start_state, std::set<int> *state_set) {
  std::queue<int> que;
  que.push(start_state);
  state_set->clear();
  state_set->insert(start_state);
  while (!que.empty()) {
    int this_state_id = que.front();
    que.pop();
    const State &this_state = graph.get_state(this_state_id);
    if (this_state.has_ilabel(Arc::EPSILON)) {
      const std::vector<Arc> &eps_arcs = this_state.get_arcs(Arc::EPSILON);
      for (std::vector<Arc>::const_iterator iter = eps_arcs.begin();
           iter != eps_arcs.end(); iter++) {
        int this_next_state = iter->next_state;
        if (state_set->find(this_next_state) != state_set->end()) {
          que.push(this_next_state);
          state_set->insert(this_next_state);
        }
      }
    }
  }
}

int Graph::eliminate_eps_arc(Graph *fa) {
  Graph new_fa = *fa;
  std::set<int> valid_states;
  valid_states.insert(fa->get_start());
  for (std::map<int, State>::const_iterator iter = fa->states_.begin();
       iter != fa->states_.end(); iter++) {
    int this_state_id = iter->first;
    const State &this_state = iter->second;
    for (State::ConstArcIter iter = this_state.arcs_begin();
         iter != this_state.arcs_end(); iter++) {
      int ilabel = iter->first;
      const std::vector<Arc> &this_ilabel_arcs = iter->second;
      if (Arc::EPSILON != ilabel) {
        for (std::vector<Arc>::const_iterator iter = this_ilabel_arcs.begin();
             iter != this_ilabel_arcs.end(); iter++) {
          valid_states.insert(iter->next_state);
        }
      }
    }
  }

  for (std::set<int>::const_iterator iter = valid_states.begin();
       iter != valid_states.end(); iter++) {
    int this_valid_state = *iter;
    std::set<int> this_valid_state_eps_cloure;
    eps_cloure(*fa, this_valid_state, &this_valid_state_eps_cloure);
    this_valid_state_eps_cloure.erase(this_valid_state);

    for (std::set<int>::const_iterator eps_iter = this_valid_state_eps_cloure.begin();
         eps_iter != this_valid_state_eps_cloure.end(); eps_iter++) {
      const State &this_state = fa->get_state(*eps_iter);
      if (fa->is_final(*eps_iter)) {
        new_fa.set_final(this_valid_state);
      }
      for (State::ConstArcIter arc_iter = this_state.arcs_begin();
           arc_iter != this_state.arcs_end(); arc_iter++) {
        int this_ilabel = arc_iter->first;
        if (Arc::EPSILON != this_ilabel) {
          const std::vector<Arc> &this_arcs = arc_iter->second;
          for (std::vector<Arc>::const_iterator insert_arc_iter = this_arcs.begin();
               insert_arc_iter != this_arcs.end(); insert_arc_iter++) {
            new_fa.add_arc(this_valid_state, *insert_arc_iter);
          }
        }
      }
    }
  }

  std::vector<int> invalid_state_ids;
  for (std::map<int, State>::iterator state_iter = new_fa.states_.begin();
       state_iter != new_fa.states_.end(); state_iter++) {
    int this_state_id = state_iter->first;
    State &this_state = state_iter->second;
    if (this_state.has_ilabel(Arc::EPSILON)) {
      this_state.remove_arcs_with_ilabel(Arc::EPSILON);
    }

    if (this_state.num_arcs() == 0) {
      invalid_state_ids.push_back(this_state_id);
    }
  }

  for (std::vector<int>::iterator iter = invalid_state_ids.begin();
       iter != invalid_state_ids.end(); iter++) {
    new_fa.states_.erase(*iter);
  }
  return 0;
}
