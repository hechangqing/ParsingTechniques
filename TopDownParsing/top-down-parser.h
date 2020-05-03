#ifndef TOP_DOWN_PARSER_H
#define TOP_DOWN_PARSER_H

#include <vector>
#include <string>
#include <map>
#include <stack>

#include "grammar.h"

class BackTrackingParser {
public:
  int compile(const Grammar &grammar);
  int parse(const std::string &input);
  int print_instantaneous_description(int input_idx);
  void clear_index(std::stack<Symbol> *stk);
private:
  std::stack<Symbol> analysis_stack_;
  std::stack<Symbol> prediction_stack_;
  Grammar grammar_;
};

#endif
