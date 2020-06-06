#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <set>

class Arc {
public:
  enum IlabelValue {
    EPSILON = 0
  };
  Arc() {}
  Arc(int new_ilabel, int new_next_state) :
    ilabel(new_ilabel), 
    next_state(new_next_state) {
  }
  int ilabel;
  int next_state;
};

class State {
public:
  typedef std::map<int, std::vector<Arc> >::iterator ArcIter;
  typedef std::map<int, std::vector<Arc> >::const_iterator ConstArcIter;
  int add_arc(const Arc &arc);
  int remove_arcs_with_ilabel(int ilabel);
  int num_arcs() const;
  ArcIter arcs_begin();
  ConstArcIter arcs_begin() const;
  ArcIter arcs_end();
  ConstArcIter arcs_end() const;
  const std::vector<Arc> &get_arcs(int ilabel) const;
  bool has_ilabel(int ilabel) const;
private:
  std::map<int, std::vector<Arc> > arcs_;
};

class Graph {
public:
  typedef std::set<int>::iterator FinalStatesIterator;
  Graph() : state_cnt_(0), start_state_id_(0) { }
  int add_state();
  int max_state_id() const;
  int num_states() const;
  int set_start(int start_state_id);
  int get_start() const;
  bool is_final(int state_id) const;
  int set_final(int final_state_id);
  int remove_final(int final_state_id);
  int add_arc(int src_state_id, const Arc &arc);
  FinalStatesIterator final_states_begin();
  FinalStatesIterator final_states_end();
  const State &get_state(int state_id) const;
  static int concate_fa(const Graph &left, const Graph &right, Graph *fa);
  static int fa_alternative(const std::vector<Graph> &fa_graph, Graph *fa);
  static int fa_proper_sequence(Graph *fa);
  static int fa_kleene_star(Graph *fa);
  static int fa_optinal(Graph *fa);
  static int eliminate_eps_arc(Graph *fa);
private:
  std::map<int, State> states_;
  int state_cnt_;
  int start_state_id_;
  std::set<int> final_states_;
  int max_state_id_;
};

int generate_one_char_fa(int ilabel, Graph *fa);
int fa_char_set(const std::set<int> &char_set, Graph *fa);

#endif // GRAPH_H
