#ifndef C_LEXER_H
#define C_LEXER_H

#include "lexer.h"

// TODO add type KEYWORD
enum LexType {
  IDENTIFIER,
  INTEGER_CONSTANT,
  CHARACTER_CONSTANT,
  REAL_NUMBER_CONSTANT,
  STRING_CONSTANT,
  OPERATOR,
  SEPARATOR,
  WHITE_SPACE,
  UNKNOWN
};

const char *lex_type_to_str(int lex_type);

struct Token {
  int type;
  int start;
  int end;
};

class CLexer {
public:
  CLexer() : input_(NULL), start_pos_(0), text_len_(0) {
    init();
  }
  int init_input(const char *input_text, int text_len);
  int next_token(Token *tok);
private:
  int init();
private:
  Lexer lexer_;
  const char *input_;
  int start_pos_;
  int text_len_;
};

#endif
