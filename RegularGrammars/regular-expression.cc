#include "regular-expression.h"

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <sstream>

CompoundState::CompoundState(const std::vector<int> &states) {
  states_ = states;
  for (int i = 0; i < states_.size(); i++) {
    int this_state = states_[i];
    if (this_state >= states_exist_.size()) {
      states_exist_.resize(this_state+1, 0);
    }
    states_exist_[this_state] = 1;
  }
  std::stringstream ss;
  for (int i = 0; i < states_exist_.size(); i++) {
    ss << states_exist_[i] << ",";
  }
  str_ = ss.str();
}

int RegularExpression::convert_regular_grammar_to_NFA(const Grammar &regular_grammar, Graph *nfa) {
  std::map<int, int> non_terminal_to_state;
  const Symbol &start = regular_grammar.get_start();
  int start_state = nfa->add_state();
  non_terminal_to_state[start.id] = start_state;
  nfa->set_start(start_state);
  int final_state = nfa->add_state();
  nfa->set_final(final_state);

  for (int i = 0; i < regular_grammar.num_rules(); i++) {
    const Rule &rule = regular_grammar.get_rule(i);
    const Symbol &rule_left = rule.left;
    const std::vector<Symbol> &rule_right = rule.right;
    int state = 0;
    if (non_terminal_to_state.find(rule_left.id) != non_terminal_to_state.end()) {
      state = non_terminal_to_state[rule_left.id];
    } else {
      state = nfa->add_state();
      non_terminal_to_state[rule_left.id] = state;
    }
    if (rule_right.size() == 0) { // A -> <eps>
      nfa->add_arc(state, Arc(EPSILON, final_state));
    } else if (rule_right.size() == 1) { // A -> B or A -> b
      if (rule_right[0].type == NON_TERMINAL) { // A -> B
        int next_state = 0;
        if (non_terminal_to_state.find(rule_right[0].id) != non_terminal_to_state.end()) {
          next_state = non_terminal_to_state[rule_right[0].id];
        } else {
          next_state = nfa->add_state();
          non_terminal_to_state[rule_right[0].id] = next_state;
        }
        nfa->add_arc(state, Arc(EPSILON, next_state));
      } else { // A -> b
        nfa->add_arc(state, Arc(rule_right[0].id, final_state));
      }
    } else if (rule_right.size() == 2) { // A -> a B
      int next_state = 0;
      if (non_terminal_to_state.find(rule_right[1].id) != non_terminal_to_state.end()) {
        next_state = non_terminal_to_state[rule_right[1].id];
      } else {
        next_state = nfa->add_state();
        non_terminal_to_state[rule_right[1].id] = next_state;
      }
      nfa->add_arc(state, Arc(rule_right[0].id, next_state));
    } else {
      std::cerr << "regular grammar error, rule right has more than 2 symbol" << std::endl;
      assert(0);
    }
  }
  return 0;
}

int RegularExpression::eps_closure(const Graph& graph, int state, std::vector<int> *all_states) {
  all_states->clear();
  std::queue<int> que;
  std::set<int> visited;
  que.push(state);
  all_states->push_back(state);
  visited.insert(state);
  while (!que.empty()) {
    int this_state_id = que.front();
    que.pop();
    const State& this_state = graph.get_state(this_state_id);
    for (int i = 0; i < this_state.num_arcs(); i++) {
      const Arc &this_arc = this_state.get_arc(i);
      int next_state = this_arc.get_next_state();
      if (this_arc.get_ilabel() == EPSILON && visited.find(next_state) == visited.end()) {
        que.push(next_state);
        all_states->push_back(next_state);
        visited.insert(next_state);
      }
    }
  }
}

int RegularExpression::convert_NFA_to_DFA(const Graph& nfa, Graph *dfa) {
  int start_state = nfa.get_start();
  std::vector<int> start_state_eps_closure;
  eps_closure(nfa, start_state, &start_state_eps_closure);

  int dfa_start_state = dfa->add_state();
  dfa->set_start(dfa_start_state);

  std::map<std::string, int> new_state_string_to_new_state_id;
  CompoundState start_compound_state(start_state_eps_closure);
  new_state_string_to_new_state_id[start_compound_state.to_string()] = dfa_start_state;
  std::queue<CompoundState> que;
  que.push(start_compound_state);

  while (!que.empty()) {
    CompoundState this_compound_state = que.front();
    assert(new_state_string_to_new_state_id.find(this_compound_state.to_string()) != new_state_string_to_new_state_id.end());
    int this_new_state_id = new_state_string_to_new_state_id[this_compound_state.to_string()];
    que.pop();
    std::map<int, std::vector<int> > input_symbol_to_states;
    for (int i = 0; i < this_compound_state.num_states(); i++) {
      int this_old_state_id = this_compound_state.get_state(i);
      const State& this_old_state = nfa.get_state(this_old_state_id);
      for (int j = 0; j < this_old_state.num_arcs(); j++) {
        const Arc &this_arc = this_old_state.get_arc(j);
        int this_ilabel = this_arc.get_ilabel();
          if (this_ilabel != EPSILON) {
          int this_next_state = this_arc.get_next_state();
          std::vector<int> next_state_eps_closure;
          eps_closure(nfa, this_next_state, &next_state_eps_closure);
          for (int k = 0; k < next_state_eps_closure.size(); k++) {
            input_symbol_to_states[this_ilabel].push_back(next_state_eps_closure[k]);
          }
        }
      }
    }
    // 
    std::map<int, std::vector<int> >::iterator iter = input_symbol_to_states.begin();
    for (; iter != input_symbol_to_states.end(); iter++) {
      int ilabel = iter->first;
      std::vector<int> &states = iter->second;
      CompoundState this_compound_state(states);
      std::string this_compound_state_str = this_compound_state.to_string();
      if (new_state_string_to_new_state_id.find(this_compound_state_str) != new_state_string_to_new_state_id.end()) {
        dfa->add_arc(this_new_state_id, Arc(ilabel, new_state_string_to_new_state_id[this_compound_state_str]));
      } else {
        int new_next_state = dfa->add_state();
        dfa->add_arc(this_new_state_id, Arc(ilabel, new_next_state));
        new_state_string_to_new_state_id[this_compound_state_str] = new_next_state;
        que.push(this_compound_state);
      }
    }
  }

  return 0;
}

