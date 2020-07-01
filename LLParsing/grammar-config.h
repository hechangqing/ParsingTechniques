#ifndef GRAMMAR_READER_H
#define GRAMMAR_READER_H

#include "../Lexer/lexer.h"

enum GrammarConfigLexType {
  KEYWORD_LEX,
  KEYWORD_RULE,
  IDENTIFIER,
  ASSIGN,
  STRING,
  SEPARATOR,
  LEFT_CURLY_BRACKET,
  RIGHT_CURLY_BRACKET,
  WHITE_SPACE,
  EOF,
  UNKNOWN
};

const char *grammar_config_lex_type_to_str(int lex_type);

struct Token {
  Token() : type(EOF), start(0), end(0) {}
  int type;
  int start;
  int end;
};

class GrammarConfig {
public:
  GrammarConfig() : input_(NULL), start_pos_(0), text_len_(0) {
    init();
  }
  int parse_config(const char *config_text, int text_len);
  int init_input(const char *input_text, int text_len);
  int next_token(Token *tok);
private:
  int init();
  int match(int expect_type);
  int get_next();
  int look_ahead();
  int parse_cfg();
  int parse_lexical();
  int parse_lex_entry();
  int parse_lex_entry_list();
  int parse_rule();
  int parse_rule_entry_list();
  int parse_rule_entry();
private:
  Lexer lexer_;
  const char *input_;
  int start_pos_;
  int text_len_;
  Token look_ahead_tok_;
};

#endif
