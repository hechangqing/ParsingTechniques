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
#endif
