#include "regular-expression.h"

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <sstream>
#include <stack>

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

bool RegularExpression::is_standard_form(const Rule &rule) const {
  // four (extended) standard form
  assert(rule.left.type == NON_TERMINAL);
  if (rule.right.size() == 0) { // P -> <eps> (extended standard)
    return true;
  } else if (rule.right.size() == 1) { // P -> a (standard)  or  P -> Q (extended standard)
    assert(rule.right[0].type == TERMINAL || rule.right[0].type == NON_TERMINAL);
    return true;
  } else if (rule.right.size() == 2 &&
             rule.right[0].type == TERMINAL &&
             rule.right[1].type == NON_TERMINAL) { // P -> aQ (standard)
    return true;
  } else {
    return false;
  }
}

//
int RegularExpression::recognise_rule_pattern(const Rule &rule, int *end_idx,
                                              std::vector<int> *or_indices) {
  assert(end_idx != NULL && or_indices != NULL);
  assert(rule.right.size() > 1);
  or_indices->clear();
  if (rule.right[0].id == '(') {
    std::stack<int> stk;
    bool meet_or = false;
    size_t i = 0;
    for (i = 0; i < rule.right.size(); i++) {
      if (rule.right[i].id == '(') {
        stk.push(rule.right[i].id);
      } else if (rule.right[i].id == ')') {
        if (!stk.empty()) {
          stk.pop();
        } else {
          std::cerr << "Error: unmatch ( ) in regular rexpression";
          assert(0);
        }
        if (stk.empty()) {
          break;
        }
      } else if (rule.right[i].id == '|' && stk.size() == 1) {
        or_indices->push_back(i);
        meet_or = true;
      }
    }
    if (!stk.empty()) {
      std::cerr << "Error: unmatch ( ) in regular rexpression";
      assert(0);
    }

    assert(rule.right[i].id == ')');

    if (i+1 < rule.right.size()) {
      if (rule.right[i+1].id == '*') {
        // P -> R*
        *end_idx = i+1;
        return 4;
      } else if (rule.right[i+1].id == '+') {
        // P -> R+
        *end_idx = i+1;
        return 5;
      } else if (rule.right[i+1].id == '?') {
        // P -> R?
        *end_idx = i+1;
        return 6;
      } else if (meet_or) {
        // P -> (R1|R2|...)...
        *end_idx = i;
        return 2;
      } else {
        // P -> (R)...
        *end_idx = i;
        return 3;
      }
    } else if (meet_or) {
      // P -> (R1|R2|...)
      *end_idx = i;
      return 2;
    } else {
      // P -> (R)
      *end_idx = i;
      return 3;
    }
  } else if (rule.right[0].id == '[') {
    if (rule.right.size() <= 2) {
      std::cerr << "Error: empty [] " << std::endl;
      assert(0);
    }
    for (size_t i = 1; i < rule.right.size(); i++) {
      if (rule.right[i].id == ']') {
        *end_idx = i;
        break;
      }
    }
    // P -> [abc...]...
    return 7;
  } else if (rule.right[0].type == TERMINAL) {
    if (rule.right[1].id == '*') {
      *end_idx = 1;
      return 4;
    } else if (rule.right[1].id == '+') {
      *end_idx = 1;
      return 5;
    } else if (rule.right[1].id == '?') {
      *end_idx = 1;
      return 6;
    } else {
      // P -> a...
      return 1;
    }
  } else {
    // Unknown type
    return 0;
  }
}

int RegularExpression::convert_regular_expression_to_regular_grammar(
    const std::string &regular_expression,
    Grammar *regular_grammar) {
  Symbol start_non_terminal(NON_TERMINAL, regular_grammar->add_id());
  regular_grammar->set_start(start_non_terminal);
  Rule init_rule;
  init_rule.left = start_non_terminal;
  for (size_t i = 0; i < regular_expression.size(); i++) {
    init_rule.right.push_back(Symbol(TERMINAL, regular_expression[i]));
  }
  if (is_standard_form(init_rule)) {
    regular_grammar->add_rule(init_rule);
    return 0;
  }
  std::queue<Rule> que;
  que.push(init_rule);
  while (!que.empty()) {
    const Rule this_rule = que.front();
    que.pop();
    assert(!is_standard_form(this_rule));
    assert(this_rule.right.size() > 1);
    int end_idx = 0;
    std::vector<int> or_indices;
    int rule_pattern = recognise_rule_pattern(this_rule, &end_idx, &or_indices);
    if (rule_pattern == 1) {
      // P -> a...
      // P -> aT ; T -> ...
      Rule rule1;
      rule1.left = this_rule.left;
      rule1.right.push_back(this_rule.right[0]);
      Symbol new_non_terminal(NON_TERMINAL, regular_grammar->add_id());
      rule1.right.push_back(new_non_terminal);
      assert(is_standard_form(rule1));
      regular_grammar->add_rule(rule1);
      Rule rule2;
      rule2.left = new_non_terminal;
      rule2.right.insert(rule2.right.end(), this_rule.right.begin() + 1, this_rule.right.end());
      if (is_standard_form(rule2)) {
        regular_grammar->add_rule(rule2);
      } else {
        que.push(rule2);
      }
    } else if (rule_pattern == 2) {
      // P -> (R1|R2|...)...
      // P -> R1... ; P -> R2... ; ...
      assert(this_rule.right[0].id == '(');
      assert(this_rule.right[end_idx].id == ')');
      or_indices.push_back(end_idx);
      int prev_idx = 0;
      for (size_t i = 0; i < or_indices.size(); i++) {
        Rule rule;
        rule.left = this_rule.left;
        rule.right.insert(rule.right.end(),
                          this_rule.right.begin() + prev_idx + 1,
                          this_rule.right.begin() + or_indices[i]);
        rule.right.insert(rule.right.end(),
                          this_rule.right.begin() + end_idx + 1,
                          this_rule.right.end());
        prev_idx = or_indices[i];
        if (is_standard_form(rule)) {
          regular_grammar->add_rule(rule);
        } else {
          que.push(rule);
        }
      }
    } else if (rule_pattern == 3) {
      // P -> (R)...
      // P -> R...
      assert(this_rule.right[0].id == '(');
      assert(this_rule.right[end_idx].id == ')');
      Rule rule;
      rule.left = this_rule.left;
      rule.right.insert(rule.right.end(),
                        this_rule.right.begin() + 1,
                        this_rule.right.begin() + end_idx);
      rule.right.insert(rule.right.end(),
                        this_rule.right.begin() + end_idx + 1,
                        this_rule.right.end());
      if (is_standard_form(rule)) {
        regular_grammar->add_rule(rule);
      } else {
        que.push(rule);
      }
    } else if (rule_pattern == 4) {
      // P -> R*...
      // P -> T; T -> RT; T -> ...
      assert(this_rule.right[end_idx].id == '*');
      Symbol new_non_terminal(NON_TERMINAL, regular_grammar->add_id());
      // P -> T
      Rule rule1;
      rule1.left = this_rule.left;
      rule1.right.push_back(new_non_terminal);
      assert(is_standard_form(rule1));
      regular_grammar->add_rule(rule1);
      // T -> RT
      Rule rule2;
      rule2.left = new_non_terminal;
      rule2.right.insert(rule2.right.end(),
                         this_rule.right.begin(),
                         this_rule.right.begin() + end_idx);
      rule2.right.push_back(new_non_terminal);
      if (is_standard_form(rule2)) {
        regular_grammar->add_rule(rule2);
      } else {
        que.push(rule2);
      }
      // T -> ...
      Rule rule3;
      rule3.left = new_non_terminal;
      rule3.right.insert(rule3.right.end(), 
                         this_rule.right.begin() + end_idx + 1,
                         this_rule.right.end());
      if (is_standard_form(rule3)) {
        regular_grammar->add_rule(rule3);
      } else {
        que.push(rule3);
      }
    } else if (rule_pattern == 5) {
      // P -> R+...
      // P -> RT ; T -> RT ; T -> ...
      assert(this_rule.right[end_idx].id == '+');
      Symbol new_non_terminal(NON_TERMINAL, regular_grammar->add_id());
      // P -> RT
      Rule rule1;
      rule1.left = this_rule.left;
      rule1.right.insert(rule1.right.end(),
                         this_rule.right.begin(),
                         this_rule.right.begin() + end_idx);
      rule1.right.push_back(new_non_terminal);
      if (is_standard_form(rule1)) {
        regular_grammar->add_rule(rule1);
      } else {
        que.push(rule1);
      }
      // T -> RT
      Rule rule2;
      rule2.left = new_non_terminal;
      rule2.right.insert(rule2.right.end(),
                         this_rule.right.begin(),
                         this_rule.right.begin() + end_idx);
      rule2.right.push_back(new_non_terminal);
      if (is_standard_form(rule2)) {
        regular_grammar->add_rule(rule2);
      } else {
        que.push(rule2);
      }
      // T -> ...
      Rule rule3;
      rule3.left = new_non_terminal;
      rule3.right.insert(rule3.right.end(),
                         this_rule.right.begin() + end_idx + 1,
                         this_rule.right.end());
      if (is_standard_form(rule3)) {
        regular_grammar->add_rule(rule3);
      } else {
        que.push(rule3);
      }
    } else if (rule_pattern == 6) {
      // P -> R?...
      // P -> R... ; P -> ...
      assert(this_rule.right[end_idx].id == '?');
      Rule rule1;
      rule1.left = this_rule.left;
      rule1.right.insert(rule1.right.end(),
                         this_rule.right.begin(),
                         this_rule.right.begin() + end_idx);
      rule1.right.insert(rule1.right.end(),
                         this_rule.right.begin() + end_idx + 1,
                         this_rule.right.end());
      if (is_standard_form(rule1)) {
        regular_grammar->add_rule(rule1);
      } else {
        que.push(rule1);
      }
      Rule rule2;
      rule2.left = this_rule.left;
      rule2.right.insert(rule2.right.end(),
                         this_rule.right.begin() + end_idx + 1,
                         this_rule.right.end());
      if (is_standard_form(rule2)) {
        regular_grammar->add_rule(rule2);
      } else {
        que.push(rule2);
      }
    } else if (rule_pattern == 7) {
      // P -> [abc...]...
      // P -> (a|b|c|...)...
      assert(this_rule.right.size() >= 3);
      assert(this_rule.right[end_idx].id == ']');
      Rule rule;
      rule.left = this_rule.left;
      rule.right.push_back(Symbol(TERMINAL, '('));
      rule.right.push_back(this_rule.right[1]);
      for (size_t i = 2; i < end_idx; i++) {
        rule.right.push_back(Symbol(TERMINAL, '|'));
        rule.right.push_back(this_rule.right[i]);
      }
      rule.right.push_back(Symbol(TERMINAL, ')'));
      rule.right.insert(rule.right.end(),
                        this_rule.right.begin() + end_idx + 1,
                        this_rule.right.end());
      assert(!is_standard_form(rule));
      que.push(rule);
    } else {
      std::cerr << "Error: unkown rule_pattern " << rule_pattern << std::endl;
      assert(0);
    }
  }
  return 0;
}
