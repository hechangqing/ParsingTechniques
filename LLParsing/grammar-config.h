#ifndef GRAMMAR_READER_H
#define GRAMMAR_READER_H

#include "../Lexer/lexer.h"
#include "common-types.h"

enum GrammarConfigLexType {
  KEYWORD_LEX,
  KEYWORD_RULE,
  KEYWORD_SET_START,
  KEYWORD_EPSILON,
  IDENTIFIER,
  ASSIGN,
  STRING,
  SEPARATOR,
  LEFT_CURLY_BRACKET,
  RIGHT_CURLY_BRACKET,
  WHITE_SPACE,
  EOF_TAG,
  UNKNOWN
};

const char *grammar_config_lex_type_to_str(int lex_type);

struct Token {
  Token() : type(EOF_TAG), start(0), end(0) {}
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
  std::vector<std::vector<std::string> > &get_rules();
  int name_to_type(const std::string &name);
  std::string get_start_symbol();
  int init_input(const char *input_text, int text_len);
  int next_token(Token *tok);
private:
  int get_token_text(const Token &tok, std::string *str);
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
  int parse_set_start();
private:
  Lexer lexer_;
  const char *input_;
  int start_pos_;
  int text_len_;
  Token look_ahead_tok_;
  Token cur_tok_;
  std::map<std::string, int> name_to_symbol_type_; // non_terminal or terminal
  std::vector<std::vector<std::string> > rules_;
  std::vector<std::pair<std::string, std::string> > lex_defs_;
  std::string start_non_terminal_;
};

#endif
