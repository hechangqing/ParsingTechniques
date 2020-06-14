#ifndef C_LEXER_H
#define C_LEXER_H

#include "lexer.h"

class Token {
};

class CLexer {
public:
  int init();
  int next_token(Token *tok);
private:
};

#endif
