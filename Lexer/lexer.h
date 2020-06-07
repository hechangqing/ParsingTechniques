#ifndef LEXER_H
#define LEXER_H

class Lexer {
public:
  int compile(const std::vector<std::string> > &regex_rules, const std::vector<int> &rule_ids);
  int next_token(const char *text, int start_pos, int *rule_id, int *end_pos);
private:
};

#endif
