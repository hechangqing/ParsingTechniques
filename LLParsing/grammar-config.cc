#include "grammar-config.h"

#include <cassert>
#include <iostream>
#include <iomanip>

int GrammarConfig::parse_config(const char *config_text, int text_len) {
  init_input(config_text, text_len);
  get_next();
  parse_cfg();
}

int GrammarConfig::init() {
  std::vector<std::string> regex_rules;
  std::vector<int> rule_ids;
  regex_rules.push_back("(lexical)");
  rule_ids.push_back(KEYWORD_LEX);
  regex_rules.push_back("(rule)");
  rule_ids.push_back(KEYWORD_RULE);
  regex_rules.push_back("[_a-zA-Z][_a-zA-Z0-9]*");
  rule_ids.push_back(IDENTIFIER);
  regex_rules.push_back("=");
  rule_ids.push_back(ASSIGN);
  regex_rules.push_back("'[^']*'");
  rule_ids.push_back(STRING);
  regex_rules.push_back(";");
  rule_ids.push_back(SEPARATOR);
  regex_rules.push_back("{");
  rule_ids.push_back(LEFT_CURLY_BRACKET);
  regex_rules.push_back("}");
  rule_ids.push_back(RIGHT_CURLY_BRACKET);
  regex_rules.push_back("[ \t\n\v\f]+");
  rule_ids.push_back(WHITE_SPACE);
  lexer_.compile(regex_rules, rule_ids);

  return 0;
}

int GrammarConfig::init_input(const char *input_text, int text_len) {
  input_ = input_text;
  text_len_ = text_len;
  start_pos_ = 0;
  return 0;
}

int GrammarConfig::next_token(Token *tok) {
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
    tok->type = EOF;
    tok->start = -1;
    tok->end = -1;
    return 1;
  }
}

const char *grammar_config_lex_type_to_str(int lex_type) {
  switch (lex_type) {
    case KEYWORD_LEX:
      return "KEYWORD_LEX";
      break;
    case KEYWORD_RULE:
      return "KEYWORD_RULE";
      break;
    case IDENTIFIER:
      return "ID";
      break;
    case ASSIGN:
      return "ASSIGN";
      break;
    case STRING:
      return "STRING";
      break;
    case SEPARATOR:
      return "SEP";
      break;
    case LEFT_CURLY_BRACKET:
      return "LEFT_CURLY_BRACKET";
      break;
    case RIGHT_CURLY_BRACKET:
      return "RIGHT_CURLY_BRACKET";
      break;
    case WHITE_SPACE:
      return "WHITE_SPACE";
      break;
    case EOF:
      return "EOF";
      break;
  }
  return "UNKNOWN";
}

int GrammarConfig::parse_cfg() {
  parse_lexical();
  parse_rule();
  return 0;
}

int GrammarConfig::parse_lexical() {
  match(KEYWORD_LEX);
  match(LEFT_CURLY_BRACKET);
  parse_lex_entry_list();
  match(RIGHT_CURLY_BRACKET);
  return 0;
}

int GrammarConfig::parse_lex_entry_list() {
  while (look_ahead() == IDENTIFIER) {
    parse_lex_entry();
  }
  return 0;
}

int GrammarConfig::parse_lex_entry() {
  match(IDENTIFIER);
  match(ASSIGN);
  match(STRING);
  return 0;
}

int GrammarConfig::parse_rule() {
  match(KEYWORD_RULE);
  match(LEFT_CURLY_BRACKET);
  parse_rule_entry_list();
  match(RIGHT_CURLY_BRACKET);
  return 0;
}

int GrammarConfig::parse_rule_entry_list() {
  while (look_ahead() == IDENTIFIER) {
    parse_rule_entry();
  }
  return 0;
}

int GrammarConfig::parse_rule_entry() {
  match(IDENTIFIER);
  match(ASSIGN);
  while (look_ahead() == IDENTIFIER) {
    match(IDENTIFIER);
  }
  match(SEPARATOR);
  return 0;
}

int GrammarConfig::match(int expect_type) {
  if (look_ahead() == expect_type) {
    std::string next_tok_text;
    if (look_ahead_tok_.start >= 0 && look_ahead_tok_.start < look_ahead_tok_.end && look_ahead_tok_.end <= text_len_) {
      next_tok_text = std::string(input_ + look_ahead_tok_.start, input_ + look_ahead_tok_.end);
    }
    std::cout << "expect " << std::setw(20) << grammar_config_lex_type_to_str(expect_type)
              << " got (" << std::setw(20) << grammar_config_lex_type_to_str(look_ahead_tok_.type)
              << ", " << std::setw(20) << next_tok_text
              << ") " << std::endl;
    get_next();
  } else {
    std::string next_tok_text;
    if (look_ahead_tok_.start >= 0 && look_ahead_tok_.start < look_ahead_tok_.end && look_ahead_tok_.end <= text_len_) {
      next_tok_text = std::string(input_ + look_ahead_tok_.start, input_ + look_ahead_tok_.end);
    }
    std::cerr << "expect " << grammar_config_lex_type_to_str(expect_type)
              << " got (" << grammar_config_lex_type_to_str(look_ahead_tok_.type)
              << ", " << next_tok_text
              << ") " << std::endl;
    assert(0);
  }
  return 0;
}

int GrammarConfig::get_next() {
  while (next_token(&look_ahead_tok_) == 0) {
    if (look_ahead_tok_.type != WHITE_SPACE) {
      break;
    }
  }
  return 0;
}

int GrammarConfig::look_ahead() {
  return look_ahead_tok_.type;
}
