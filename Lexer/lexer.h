#ifndef LEXER_H
#define LEXER_H

#include "graph.h"

#include <string>
#include <vector>

class Lexer {
public:
  int compile(const std::vector<std::string> &regex_rules, const std::vector<int> &rule_ids);
  int next_token(const std::string &text, int start_pos, std::vector<int> *rule_ids, int *end_pos);
  int next_token(const char *text, int start_pos, int text_len, std::vector<int> *rule_id, int *end_pos);
private:
  Graph dfa_;
  std::map<int, std::vector<int> > dfa_final_state_to_rule_ids_;
};

#endif
