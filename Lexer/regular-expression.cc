#include "regular-expression.h"

#include <cassert>
#include <iostream>

int RegularExpression::preprocess_regular_expression(const std::string &regular_expression,
                                                     std::vector<int> *processed) {
  assert(processed != NULL);
  processed->clear();
  int i = 0;
  while (i < regular_expression.size()) {
    int this_char = regular_expression[i];
    if (this_char == '\\' && i+1 < regular_expression.size()) {
      int next_char = regular_expression[i+1];
      if (next_char == '|' || next_char == '*' || next_char == '+' || next_char == '?' || next_char == '(' || next_char == ')' || next_char == '[' || next_char == ']') {
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
      } else {
        processed->push_back(this_char);
      }
      i += 1;
    }
  }
  return 0;
}

int RegularExpression::process_parenthesis(const std::vector<int> &regular_expression, int cur_idx,
                                           std::vector<int> *or_indices) {
  assert(or_indices != NULL);
  or_indices->clear();
  assert(cur_idx < regular_expression.size());
  assert(regular_expression[cur_idx] == CTRL_LEFT_PARENTHESIS);
  std::vector<int> stk;
  for (; cur_idx < regular_expression.size(); cur_idx++) {
    int this_char = regular_expression[cur_idx];
    if (this_char == CTRL_LEFT_PARENTHESIS) {
      stk.push_back(this_char);
    } else if (this_char == CTRL_RIGHT_PARENTHESIS) {
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

int RegularExpression::recognise_rule_pattern(const std::vector<int> &regular_expression, int cur_idx, int *end_idx) {
  assert(end_idx != NULL);
  assert(cur_idx < regular_expression.size());
  int this_char = regular_expression[cur_idx];
  if (this_char == CTRL_LEFT_PARENTHESIS) {
    std::vector<int> or_indices;
    int right_parenthesis_idx = process_parenthesis(regular_expression, cur_idx, &or_indices);
    if (ERROR_UNMATCHED_PARENTHESIS == right_parenthesis_idx) {
      std::cerr << "wrong regular expression" << std::endl;
      return -1;
    } else {
      assert(regular_expression[right_parenthesis_idx] == CTRL_RIGHT_PARENTHESIS);
      if (cur_idx + 1 < regular_expression.size()) {
        int next_char = regular_expression[cur_idx + 1];
        if (next_char == CTRL_STAR) {
          return KLEENE_STAR;
        } else if (next_char == CTRL_ADD) {
          return PROPER_SEQUENCE;
        } else if (next_char == CTRL_OPTINAL) {
          return OPTINAL;
        }
      }
      if (or_indices.size() == 0) {
        return NESTING;
      } else {
        return ALTERNATIVE;
      }
    }
  }
  return 0;
}

int RegularExpression::convert_regular_expression_to_NFA(const std::string &regular_expression, Graph *nfa) {
  std::vector<int> preprocessed_regex;
  preprocess_regular_expression(regular_expression, &preprocessed_regex);
  int rule_pattern = recognise_rule_pattern(preprocessed_regex, 0);
  return 0;
}
