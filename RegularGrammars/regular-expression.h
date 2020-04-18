#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#include "grammar.h"
#include "graph.h"

#include <vector>
#include <string>
#include <map>

class TransitionTable {
public:
  int init (const Graph& dfa);
  int get_next_state(int cur_state, int input_symbol) const {
    assert(cur_state >= 0 && cur_state < states_.size());
    const std::map<int, int>& state = states_[cur_state];
    std::map<int, int>::const_iterator iter = state.find(input_symbol);
    if (iter != state.end()) {
      return iter->second;
    } else {
      return -1;
    }
  }
  int get_start() const {
    return start_state_;
  }
  bool is_final(int state) const {
    return (is_final_.find(state) != is_final_.end());
  }
private:
  std::vector<std::map<int, int> > states_;
  int start_state_;
  std::map<int, int> is_final_;
};

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
  bool states_contains_final(const Graph& graph, const std::vector<int>& states);
  int convert_NFA_to_DFA(const Graph& nfa, Graph *dfa);
  bool is_standard_form(const Rule &rule) const;
  int recognise_rule_pattern(const Rule &rule, int *end_idx,
                             std::vector<int> *or_indices);
  int convert_regular_expression_to_regular_grammar(const std::string &regular_expression,

                                                    Grammar *regular_grammar);
  int run_text_on_transition_table(const std::string& text, const TransitionTable& trans_table);
private:
};

class FastTextSearch {
public:
  int compile(const std::string& regular_expression);
  int search(const std::string& text);
private:
  TransitionTable transition_table_;
  RegularExpression regex_;
  std::string regex_str_;
};

#endif
