#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#include <string>
#include <vector>

#include "graph.h"

class RegularExpression {
public:
  enum ErrorCode {
    ERROR_UNMATCHED_PARENTHESIS = -2,
    ERROR_SET = -3
  };
private:
  enum RulePattern {
    CONCATENATION = 1,   // a...
    ALTERNATIVE,     // (R1 | R2 | ...) ...
    KLEENE_STAR,     // R*...
    PROPER_SEQUENCE, // R+...
    OPTINAL,         // R?...
    NESTING,         // (R)...
    SET              // [abc...]...
  };
  enum ControlSymbol {
    CTRL_OR = 0x10001,
    CTRL_STAR,
    CTRL_ADD,
    CTRL_OPTINAL,
    CTRL_LEFT_PARENTHESIS,
    CTRL_RIGHT_PARENTHESIS,
    CTRL_LEFT_BRACKETS,
    CTRL_RIGHT_BRACKETS,
    CTRL_RANGE,
    CTRL_EXCLUDE
  };
public:
  static int convert_regular_expression_to_NFA(const std::string &regular_expression,
                                        Graph *nfa);
  static int convert_regular_expression_to_NFA(const std::vector<int> &preprocessed_regex, int start_idx, int end_idx, Graph *nfa);
  static int convert_regular_expression_to_DFA(const std::string &regular_expression, Graph *dfa);
  static int preprocess_regular_expression(const std::string &regular_expression,
                                    std::vector<int> *processed);
  static int process_parenthesis(const std::vector<int> &regular_expression,
                          const ControlSymbol left_symbol,
                          const ControlSymbol right_symbol,
                          int cur_idx,
                          std::vector<int> *or_indices);
  static int recognise_rule_pattern(const std::vector<int> &regular_expression, int cur_idx, int end_idx, int *sub_end_idx, std::vector<int> *or_indices);
  static int get_char_set(const std::vector<int> &preprocessed_regex, int start_idx, int end_idx, std::set<int> *char_set);
public:
  int compile(const std::string &regex_str);
  bool accept(const std::string &str);
private:
  Graph dfa_;
};

#endif
