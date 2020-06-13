#include "lexer.h"
#include "regular-expression.h"
#include "graph.h"

#include <cassert>
#include <iostream>

int Lexer::compile(const std::vector<std::string> &regex_rules, const std::vector<int> &rule_ids) {
  assert(regex_rules.size() == rule_ids.size());
  std::vector<Graph> dfa_vec(regex_rules.size());
  for (int i = 0; i < regex_rules.size(); i++) {
    RegularExpression::convert_regular_expression_to_DFA(regex_rules[i], &(dfa_vec[i]));
    //std::cout << "dfa " << i << "\n" << dfa_vec[i].to_str() << "\n";
  }

  Graph combine_nfa;
  std::map<int, int> combine_nfa_final_state_to_rule_id;
  Graph::fa_alternative(dfa_vec, rule_ids, &combine_nfa, &combine_nfa_final_state_to_rule_id);

  //std::cout << "combined eps-nfa " << combine_nfa.to_str() << "\n";

  Graph::eliminate_eps_arc(&combine_nfa);

  std::map<int, std::set<int> > dfa_state_to_nfa_states;
  Graph::convert_nfa_to_dfa(combine_nfa, &dfa_, &dfa_state_to_nfa_states);

  //std::cout << "combined dfa " << dfa_.to_str() << "\n";

  for (Graph::FinalStatesIterator iter = dfa_.final_states_begin();
       iter != dfa_.final_states_end(); iter++) {
    int this_final_state_id = *iter;
    assert(dfa_state_to_nfa_states.find(this_final_state_id) != dfa_state_to_nfa_states.end());
    std::set<int> &nfa_states = dfa_state_to_nfa_states[this_final_state_id];
    for (std::set<int>::iterator nfa_state_iter = nfa_states.begin();
         nfa_state_iter != nfa_states.end(); nfa_state_iter++) {
      std::map<int, int>::iterator find_iter = combine_nfa_final_state_to_rule_id.find(*nfa_state_iter);
      if (find_iter != combine_nfa_final_state_to_rule_id.end()) {
        int rule_id = find_iter->second;
        dfa_final_state_to_rule_ids_[this_final_state_id].push_back(rule_id);
      }
    }
  }
  return 0;
}

int Lexer::next_token(const std::string &text, int start_pos, std::vector<int> *rule_id, int *end_pos) {
  assert(start_pos >= 0);
  int cur_state = dfa_.get_start();
  std::vector<int> final_state_ids_through;
  int last_idx = start_pos;
  while (start_pos < text.size()) {
    int this_char = text[start_pos];
    if (dfa_.state_has_arcs(cur_state)) {
      const State &state = dfa_.get_state(cur_state);
      int next_state = 0;
      if (state.has_ilabel(this_char)) {
        const std::vector<Arc> &arcs = state.get_arcs(this_char);
        assert(arcs.size() == 1);
        next_state = arcs[0].next_state;
        if (dfa_.is_final(next_state)) {
          final_state_ids_through.push_back(next_state);
          last_idx = start_pos + 1;
        }
      } else {
        break;
      }
      cur_state = next_state;
      start_pos++;
    } else {
      break;
    }
  }

  if (final_state_ids_through.size() > 0) {
    int last_final_state = final_state_ids_through[final_state_ids_through.size() - 1];
    assert(dfa_final_state_to_rule_ids_.find(last_final_state) != dfa_final_state_to_rule_ids_.end());
    *rule_id = dfa_final_state_to_rule_ids_[last_final_state];
    *end_pos = last_idx;
    return 0;
  } else {
    // not accept
    return 100;
  }
}
