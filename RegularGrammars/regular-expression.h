#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#include "grammar.h"
#include "graph.h"

#include <vector>
#include <string>

class CompoundState {
public:
  CompoundState(const std::vector<int> &states);
  int num_states() const {
    return states_.size();
  }
  int get_state(int i) const {
    assert(i>=0 && i < states_.size());
    return states_[i];
  }
  std::string to_string() const {
    return str_;
  }
private:
  std::vector<int> states_;
  std::vector<int> states_exist_;
  std::string str_;
};

class RegularExpression {
public:
  int convert_regular_grammar_to_NFA(const Grammar &regular_grammar, Graph *nfa);
  int eps_closure(const Graph& graph, int state, std::vector<int> *all_states);
  int convert_NFA_to_DFA(const Graph& nfa, Graph *dfa);
private:
};

#endif
