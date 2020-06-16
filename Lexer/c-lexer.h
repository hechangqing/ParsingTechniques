#ifndef C_LEXER_H
#define C_LEXER_H

#include "lexer.h"

// TODO add type KEYWORD
enum LexType {
  IDENTIFIER,
  INTEGER_CONSTANT,
  CHARACTER_CONSTANT,
  REAL_NUMBER_CONSTANT,
  STRING_CONTANT,
  OPERATOR,
  SEPARATOR,
  WHITE_SPACE
};

class Token {
  LexType type,
  int start,
  int end
};

class CLexer {
public:
  int init_input(const char *input_text, int text_size);
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
