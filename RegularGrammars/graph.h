#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cassert>
#include <string>
#include <map>

#define EPSILON -1

class Arc {
public:
  Arc() {}
  Arc(int ilabel, int next_state) : ilabel_(ilabel), next_state_(next_state) { }
  int get_ilabel() const {
    return ilabel_;
  }
  int get_next_state() const {
    return next_state_;
  }
private:
  int ilabel_;
  int next_state_;
};

class State {
public:
  int num_arcs() const {
    return arcs_.size();
  }
  const Arc& get_arc(int i) const {
    assert(i>= 0 && i < arcs_.size());
    return arcs_[i];
  }
  int add_arc(const Arc &arc) {
    arcs_.push_back(arc);
    return 0;
  }
private:
  std::vector<Arc> arcs_;
};

class Graph {
public:
  int num_states() const {
    return states_.size();
  }
  int add_state();
  int set_start(int state) {
    start_state_ = state;
    return 0;
  }
  int set_final(int state) {
    is_final_[state] = 1;
    return 0;
  }
  int add_arc(int state, const Arc &arc);
  int get_start() const {
    return start_state_;
  }
  const State& get_state(int i) const {
    assert(i>= 0 && i < states_.size());
    return states_[i];
  }
  bool is_final(int state) const {
    return (is_final_.find(state) != is_final_.end());
  }
  std::string to_string() const;
private:
  std::vector<State> states_;
  int start_state_;
  std::map<int, int> is_final_;
};

#endif
