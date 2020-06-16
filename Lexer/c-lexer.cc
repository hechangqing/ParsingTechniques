#include "c-lexer.h"

CLexer::CLexer() :input_(NULL), start_pos_(0) {
  init();
}

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
  regex_rules.push_back(",");
  rule_ids.push_back(); regex_rules.push_back("(\\(|\\)|\\[|\\]|{|}|;|,|.|:)");
  rule_ids.push_back(SEPARATOR);
  regex_rules.push_back("(.|(\\->))");
  rule_ids.push_back(OPERATOR);
  regex_rules.push_back("(\\?|:)");
  rule_ids.push_back(SEPARATOR);
  regex_rules.push_back("[ \t\n\v\f]+");
  rule_ids.push_back(WHITE_SPACE);
  lexer.compile(regex_rules, rule_ids);

  return 0;
}

int CLexer::init_input(const char *input_text) {
  input_ = input_text;
  return 0;
}

int CLexer::next_token(Token *tok) {
  assert(tok != NULL);

  
  return 0;
}
