#include "c-lexer.h"

#include <cassert>

int CLexer::init() {
  std::vector<std::string> regex_rules;
  std::vector<int> rule_ids;
  regex_rules.push_back("[_a-zA-Z][_a-zA-Z0-9]*");
  rule_ids.push_back(IDENTIFIER);
  regex_rules.push_back("(0x|0X)[0-9a-fA-F]+((u|U)?(l|L)?|(l|L)?(u|U)?)");
  rule_ids.push_back(INTEGER_CONSTANT);
  regex_rules.push_back("'([^'\\\\]|\\\\\\\\|\\\\\\?|\\\\'|\\\\\"|(\\\\[abefnrtv])|\\\\([0-7]|[0-7][0-7]|[0-7][0-7][0-7])|\\\\x[0-9a-fA-F]+)'");
  rule_ids.push_back(CHARACTER_CONSTANT);
  regex_rules.push_back("([0-9]+.[0-9]*|.[0-9]+|[0-9]+)(e(\\+|\\-)?[0-9]+)?");
  rule_ids.push_back(REAL_NUMBER_CONSTANT);
  regex_rules.push_back("\"([^\"]|\\\\\")*\"");
  rule_ids.push_back(STRING_CONSTANT);
  regex_rules.push_back("(=|\\+=|\\-=|\\*=|/=|%=|<<=|>>=|&=|\\^=|\\|=)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(\\+\\+|\\-\\-)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(\\+|\\-|\\*|/|%)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(==|!=|<|>|<=|>=)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(&&|(\\|\\|)|!)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(<<|>>)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(&|(\\|)|(\\^)|~)");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(\\(|\\)|\\[|\\]|{|}|;|,|.|:)");
  rule_ids.push_back(SEPARATOR);
  regex_rules.push_back("(.|(\\->))");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(\\?|:)");
  rule_ids.push_back(SEPARATOR);
  regex_rules.push_back("[ \t\n\v\f]+");
  rule_ids.push_back(WHITE_SPACE);
  lexer_.compile(regex_rules, rule_ids);

  return 0;
}

int CLexer::init_input(const char *input_text, int text_len) {
  input_ = input_text;
  text_len_ = text_len;
  start_pos_ = 0;
  return 0;
}

int CLexer::next_token(Token *tok) {
  assert(tok != NULL);

  if (start_pos_ < text_len_) {
    std::vector<int> ret_rules;
    int end_pos = 0;
    int ret = lexer_.next_token(input_, start_pos_, text_len_, &ret_rules, &end_pos);
    if (ret == 0) { // accept
      assert(ret_rules.size() >= 1);
      tok->type = ret_rules[0];
      tok->start = start_pos_;
      tok->end = end_pos;
      start_pos_ = end_pos;
    } else { // not accept
      tok->type = UNKNOWN;
      tok->start = start_pos_;
      tok->end = start_pos_ + 1;
      start_pos_ += 1;
    }
    return 0;
  } else {
    return 1;
  }
}

const char *lex_type_to_str(int lex_type) {
  switch (lex_type) {
    case IDENTIFIER:
      return "ID";
      break;
    case INTEGER_CONSTANT:
      return "INT";
      break;
    case CHARACTER_CONSTANT:
      return "CHAR";
      break;
    case REAL_NUMBER_CONSTANT:
      return "REAL";
      break;
    case STRING_CONSTANT:
      return "STR";
      break;
    case OPERATOR:
      return "OP";
      break;
    case SEPARATOR:
      return "SEP";
      break;
    case WHITE_SPACE:
      return "WHITE_SPACE";
      break;
  }
  return "UNKNOWN";
}

