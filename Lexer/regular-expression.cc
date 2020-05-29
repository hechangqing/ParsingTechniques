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

int RegularExpression::recognise_rule_pattern(const std::vector<int> &regular_expression, int cur_idx, int *end_idx, std::vector<int> *or_indices) {
  assert(end_idx != NULL);
  assert(cur_idx < regular_expression.size());
  int this_char = regular_expression[cur_idx];
  if (this_char == CTRL_LEFT_PARENTHESIS) {
    int right_idx = process_parenthesis(regular_expression, CTRL_LEFT_PARENTHESIS, CTRL_RIGHT_PARENTHESIS, cur_idx, or_indices);
    if (ERROR_UNMATCHED_PARENTHESIS == right_idx) {
      std::cerr << "wrong regular expression" << std::endl;
      return -1;
    }
    assert(regular_expression[right_idx] == CTRL_RIGHT_PARENTHESIS);
    if (right_idx + 1 < regular_expression.size()) {
      int next_char = regular_expression[right_idx + 1];
      *end_idx = right_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    *end_idx = right_idx;
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
    if (right_idx + 1 < regular_expression.size()) {
      int next_char = regular_expression[right_idx + 1];
      *end_idx = right_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    *end_idx = right_idx;
    return SET;
  } else {
    if (cur_idx + 1 < regular_expression.size()) {
      int next_char = regular_expression[cur_idx + 1];
      *end_idx = cur_idx + 1;
      if (next_char == CTRL_STAR) {
        return KLEENE_STAR;
      } else if (next_char == CTRL_ADD) {
        return PROPER_SEQUENCE;
      } else if (next_char == CTRL_OPTINAL) {
        return OPTINAL;
      }
    }
    return CONCATENATION;
  }
  return 0;
}

int RegularExpression::convert_regular_expression_to_NFA(const std::vector<int> &preprocessed_regex, int start_idx, int end_idx, Graph *nfa) {
  assert(end_idx <= preprocessed_regex.size());
  assert(start_idx < end_idx);
  assert(start_idx >= 0);
  if (start_idx + 1 == end_idx) {
    generate_one_char_fa(preprocessed_regex[start_idx], nfa);
    return 0;
  } else {
    std::vector<int> or_indices;
    int end_idx = 0;
    int rule_pattern = recognise_rule_pattern(preprocessed_regex, 0, &end_idx, &or_indices);
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

  std::vector<int> or_indices;
  int end_idx = 0;
  int rule_pattern = recognise_rule_pattern(preprocessed_regex, 0, &end_idx, &or_indices);
  std::cout << rule_pattern << std::endl;
  return 0;
}
