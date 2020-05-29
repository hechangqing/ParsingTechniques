#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>

class Arc {
public:
  int ilabel;
  int next_state;
};

class State {
private:
  std::vector<Arc> arcs_;
};

class Graph {
private:
  std::map<int, State> states_;
};

int generate_one_char_fa(int ilabel, Graph *fa);
int fa_kleene_star(Graph *fa);
int fa_proper_sequence(Graph *fa);
int fa_optinal(Graph *fa);
int concate_fa(const Graph &left, const Graph &right, Graph *fa);
#endif
