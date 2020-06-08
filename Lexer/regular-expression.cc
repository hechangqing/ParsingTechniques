#include "regular-expression.h"

#include <cassert>
#include <iostream>
#include <set>

int RegularExpression::preprocess_regular_expression(const std::string &regular_expression,
                                                     std::vector<int> *processed) {
  assert(processed != NULL);
  processed->clear();
  int i = 0;
  while (i < regular_expression.size()) {
    int this_char = regular_expression[i];
    if (this_char == '\\' && i+1 < regular_expression.size()) {
      int next_char = regular_expression[i+1];
      if (next_char == '|' || next_char == '*' || next_char == '+' || next_char == '?' || next_char == '(' || next_char == ')' || next_char == '[' || next_char == ']' || next_char == '-') {
        processed->push_back(next_char);
      } else {
        processed->push_back(this_char);
        processed->push_back(next_char);
      }
      i += 2;
    } else {
      if (this_char == '|') {
        processed->push_back(CTRL_OR);
      } else if (this_char == '*') {
        processed->push_back(CTRL_STAR);
      } else if (this_char == '+') {
        processed->push_back(CTRL_ADD);
      } else if (this_char == '?') {
        processed->push_back(CTRL_OPTINAL);
      } else if (this_char == '(') {
        processed->push_back(CTRL_LEFT_PARENTHESIS);
      } else if (this_char == ')') {
        processed->push_back(CTRL_RIGHT_PARENTHESIS);
      } else if (this_char == '[') {
        processed->push_back(CTRL_LEFT_BRACKETS);
      } else if (this_char == ']') {
        processed->push_back(CTRL_RIGHT_BRACKETS);
      } else if (this_char == '-') {
        processed->push_back(CTRL_RANGE);
      } else {
        processed->push_back(this_char);
      }
      i += 1;
    }
  }
  return 0;
}

int RegularExpression::process_parenthesis(const std::vector<int> &regular_expression,
                                           const ControlSymbol left_symbol,
                                           const ControlSymbol right_symbol,
                                           int cur_idx,
                                           std::vector<int> *or_indices) {
  assert(or_indices != NULL);
  or_indices->clear();
  assert(cur_idx < regular_expression.size());
  assert(regular_expression[cur_idx] == left_symbol);
  std::vector<int> stk;
  for (; cur_idx < regular_expression.size(); cur_idx++) {
    int this_char = regular_expression[cur_idx];
    if (this_char == left_symbol) {
      stk.push_back(this_char);
    } else if (this_char == right_symbol) {
      if (!stk.empty()) {
        stk.pop_back();
        if (stk.empty()) {
          break;
        }
      } else {
        return ERROR_UNMATCHED_PARENTHESIS;
      }
    } else if (this_char == CTRL_OR) {
      if (stk.size() == 1) {
        or_indices->push_back(cur_idx);
      }
    }
  }
  if (stk.empty()) {
    return cur_idx;
  } else {
    return ERROR_UNMATCHED_PARENTHESIS;
  }
}

int RegularExpression::recognise_rule_pattern(const std::vector<int> &regular_expression,
                                              int cur_idx,
                                              int end_idx,
                                              int *sub_end_idx,
                                              std::vector<int> *or_indices) {
  assert(sub_end_idx != NULL);
  assert(cur_idx < regular_expression.size());
  assert(cur_idx < end_idx);
  assert(end_idx <= regular_expression.size());
  int this_char = regular_expression[cur_idx];
  if (this_char == CTRL_LEFT_PARENTHESIS) {
    int right_idx = process_parenthesis(regular_expression, CTRL_LEFT_PARENTHESIS, CTRL_RIGHT_PARENTHESIS, cur_idx, or_indices);
    if (ERROR_UNMATCHED_PARENTHESIS == right_idx) {
      std::cerr << "wrong regular expression" << std::endl;
      return -1;
    }
    assert(regular_expression[right_idx] == CTRL_RIGHT_PARENTHESIS);
    if (right_idx + 1 < end_idx) {
      int next_char = regular_expression[right_idx + 1];
      *sub_end_idx = right_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    *sub_end_idx = right_idx;
    if (or_indices->size() == 0) {
      return NESTING;
    } else {
      return ALTERNATIVE;
    }
  } else if (this_char == CTRL_LEFT_BRACKETS) {
    std::vector<int> dummy_indices;
    int right_idx = process_parenthesis(regular_expression, CTRL_LEFT_BRACKETS, CTRL_RIGHT_BRACKETS, cur_idx, &dummy_indices);
    if (ERROR_UNMATCHED_PARENTHESIS == right_idx) {
      std::cerr << "wrong regular expression" << std::endl;
      return -1;
    }
    assert(regular_expression[right_idx] == CTRL_RIGHT_BRACKETS);
    if (right_idx + 1 < end_idx) {
      int next_char = regular_expression[right_idx + 1];
      *sub_end_idx = right_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    *sub_end_idx = right_idx;
    return SET;
  } else {
    if (cur_idx + 1 < end_idx) {
      int next_char = regular_expression[cur_idx + 1];
      *sub_end_idx = cur_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    *sub_end_idx = cur_idx;
    return CONCATENATION;
  }
  assert(0);
  return 0;
}

int RegularExpression::get_char_set(const std::vector<int> &preprocessed_regex, int start_idx, int end_idx, std::set<int> *char_set) {
  assert(end_idx <= preprocessed_regex.size());
  assert(start_idx >= 0);
  assert(start_idx < end_idx);
  assert(preprocessed_regex[start_idx] == CTRL_LEFT_BRACKETS);
  assert(preprocessed_regex[end_idx - 1] == CTRL_RIGHT_BRACKETS);
  int i = start_idx + 1;
  while (i < end_idx - 1) {
    int this_char = preprocessed_regex[i];
    if (i + 1 < end_idx - 1 && preprocessed_regex[i+1] == CTRL_RANGE) {
      if (i + 2 < end_idx - 1) {
        int range_end_char = preprocessed_regex[i+2];
        if (this_char > range_end_char) {
          std::cerr << "error regular expression: error set" << std::endl;
          return ERROR_SET;
        }
        assert(this_char <= range_end_char);
        for (int c = this_char; c <= range_end_char; c++) {
          char_set->insert(c);
        }
        i += 3;
      } else {
        std::cerr << "error regular expression: error set" << std::endl;
        return ERROR_SET;
      }
    } else {
      char_set->insert(this_char);
      i += 1;
    }
  }
  return 0;
}

// [start_idx, end_idx)
int RegularExpression::convert_regular_expression_to_NFA(const std::vector<int> &preprocessed_regex, int start_idx, int end_idx, Graph *nfa) {
  assert(end_idx <= preprocessed_regex.size());
  assert(start_idx <= end_idx);
  assert(start_idx >= 0);
  if (start_idx == end_idx) {
    // empty nfa
    return 0;
  } else if (start_idx + 1 == end_idx) {
    generate_one_char_fa(preprocessed_regex[start_idx], nfa);
    return 0;
  } else {
    std::vector<int> or_indices;
    int sub_end_idx = 0;
    int rule_pattern = recognise_rule_pattern(preprocessed_regex, start_idx, end_idx, &sub_end_idx, &or_indices);
    Graph nfa_left;
    if (KLEENE_STAR == rule_pattern || PROPER_SEQUENCE == rule_pattern || OPTINAL == rule_pattern) {
      int ret = convert_regular_expression_to_NFA(preprocessed_regex, start_idx, sub_end_idx, &nfa_left);
      if (KLEENE_STAR == rule_pattern) {
        Graph::fa_kleene_star(&nfa_left);
      } else if (PROPER_SEQUENCE == rule_pattern) {
        Graph::fa_proper_sequence(&nfa_left);
      } else if (OPTINAL == rule_pattern) {
        Graph::fa_optinal(&nfa_left);
      } else {
        assert(0);
      }
    } else if (NESTING == rule_pattern) {
      int ret = convert_regular_expression_to_NFA(preprocessed_regex, start_idx + 1, sub_end_idx, &nfa_left);
    } else if (ALTERNATIVE == rule_pattern) {
      std::vector<Graph> nfa_vec;
      or_indices.push_back(sub_end_idx);
      int this_start_idx = start_idx + 1;
      for (int i = 0; i < or_indices.size(); i++) {
        Graph this_nfa;
        int ret = convert_regular_expression_to_NFA(preprocessed_regex, this_start_idx, or_indices[i], &this_nfa);
        this_start_idx = or_indices[i] + 1;
        nfa_vec.push_back(this_nfa);
      }
      Graph::fa_alternative(nfa_vec, &nfa_left);
    } else if (SET == rule_pattern) {
      std::set<int> char_set;
      get_char_set(preprocessed_regex, start_idx, sub_end_idx + 1, &char_set);
      fa_char_set(char_set, &nfa_left);
    } else if (CONCATENATION == rule_pattern) {
      assert(sub_end_idx == start_idx);
      int ret = convert_regular_expression_to_NFA(preprocessed_regex, start_idx, start_idx + 1, &nfa_left);
    } else {
      assert(0);
    }
    Graph nfa_right;
    int ret = convert_regular_expression_to_NFA(preprocessed_regex, sub_end_idx + 1, end_idx, &nfa_right);
    Graph::concate_fa(nfa_left, nfa_right, nfa);
  }
  return 0;
}

int RegularExpression::convert_regular_expression_to_NFA(const std::string &regular_expression, Graph *nfa) {
  std::vector<int> preprocessed_regex;
  preprocess_regular_expression(regular_expression, &preprocessed_regex);

  for (int i = 0; i < preprocessed_regex.size(); i++) {
    if (preprocessed_regex[i] < 256) {
      std::cout << static_cast<char>(preprocessed_regex[i]) << " ";
    } else {
      std::cout << preprocessed_regex[i] << " ";
    }
  }
  std::cout << "\n";

  int ret = convert_regular_expression_to_NFA(preprocessed_regex, 0, preprocessed_regex.size(), nfa);

  return ret;
}

int RegularExpression::convert_regular_expression_to_DFA(const std::string &regular_expression, Graph *dfa) {
  Graph nfa;
  int ret = convert_regular_expression_to_NFA(regular_expression, &nfa);
  Graph::convert_nfa_to_dfa(nfa, dfa);
  return ret;
}

int RegularExpression::compile(const std::string &regex_str) {
  int ret = convert_regular_expression_to_DFA(regex_str, &dfa_);
  return ret;
}

bool RegularExpression::accept(const std::string &str) {
  int cur_state = dfa_.get_start();
  for (int i = 0; i < str.size(); i++) {
    int this_char = str[i];
    const State &state = dfa_.get_state(cur_state);
    int next_state = 0;
    if (state.has_ilabel(this_char)) {
      const std::vector<Arc> &arcs = state.get_arcs(this_char);
      assert(arcs.size() == 1);
      next_state = arcs[0].next_state;
      cur_state = next_state;
    } else {
      return false;
    }
  }
  if (dfa_.is_final(cur_state)) {
    return true;
  } else {
    return false;
  }
}
