#include "grammar-config.h"

#include <cassert>
#include <iostream>
#include <iomanip>

int GrammarConfig::parse_config(const char *config_text, int text_len) {
  init_input(config_text, text_len);
  get_next();
  parse_cfg();
  for (int i = 0; i < rules_.size(); i++) {
    for (int j = 0; j < rules_[i].size(); j++) {
      std::cout << rules_[i][j] << " ";
    }
    std::cout << "\n";
  }
  for (std::map<std::string, int>::iterator iter = name_to_symbol_type_.begin(); iter != name_to_symbol_type_.end(); iter++) {
    std::cout << iter->first << " " << (iter->second == NON_TERMINAL ? "non_terminal": "terminal") << "\n";
  }
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
    tok->type = EOF_TAG;
    tok->start = -1;
    tok->end = -1;
    return 1;
  }
}

int GrammarConfig::get_token_text(const Token &tok, std::string *str) {
  assert(str != NULL);
  if (tok.start >= 0 && tok.start < tok.end && tok.end <= text_len_) {
    *str = std::string(input_ + tok.start, input_ + tok.end);
    return 0;
  } else {
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
    case EOF_TAG:
      return "EOF";
      break;
  }
  return "UNKNOWN";
}

int GrammarConfig::parse_cfg() {
  int ret = parse_lexical();
  if (0 != ret) {
    return ret;
  }
  ret = parse_rule();
  if (0 != ret) {
    return ret;
  }
  return 0;
}

int GrammarConfig::parse_lexical() {
  int ret = match(KEYWORD_LEX);
  if (0 != ret) {
    return ret;
  }
  ret = match(LEFT_CURLY_BRACKET);
  if (0 != ret) {
    return ret;
  }
  ret = parse_lex_entry_list();
  if (0 != ret) {
    return ret;
  }
  ret = match(RIGHT_CURLY_BRACKET);
  if (0 != ret) {
    return ret;
  }
  return 0;
}

int GrammarConfig::parse_lex_entry_list() {
  while (look_ahead() == IDENTIFIER) {
    int ret = parse_lex_entry();
    if (0 != ret) {
      return ret;
    }
  }
  return 0;
}

int GrammarConfig::parse_lex_entry() {
  int ret = match(IDENTIFIER);
  if (0 != ret) {
    return ret;
  }
  std::string lex_name;
  assert(0 == get_token_text(cur_tok_, &lex_name));
  if (name_to_symbol_type_.find(lex_name) == name_to_symbol_type_.end()) {
    name_to_symbol_type_[lex_name] = TERMINAL;
  } else {
    std::cerr << "duplicate lexical id: \"" << lex_name
              << "\", please check the grammar\n";
    return 1;
  }
  ret = match(ASSIGN);
  if (0 != ret) {
    return ret;
  }
  ret = match(STRING);
  if (0 != ret) {
    return ret;
  }
  return 0;
}

int GrammarConfig::parse_rule() {
  int ret = match(KEYWORD_RULE);
  if (0 != ret) {
    return ret;
  }
  ret = match(LEFT_CURLY_BRACKET);
  if (0 != ret) {
    return ret;
  }
  ret = parse_rule_entry_list();
  if (0 != ret) {
    return ret;
  }
  ret = match(RIGHT_CURLY_BRACKET);
  if (0 != ret) {
    return ret;
  }
  return 0;
}

int GrammarConfig::parse_rule_entry_list() {
  while (look_ahead() == IDENTIFIER) {
    int ret = parse_rule_entry();
    if (0 != ret) {
      return ret;
    }
  }
  return 0;
}

int GrammarConfig::parse_rule_entry() {
  std::vector<std::string> rule_string_vec;
  int ret = match(IDENTIFIER);
  if (0 != ret) {
    return ret;
  }
  std::string non_terminal_str;
  assert(0 == get_token_text(cur_tok_, &non_terminal_str));
  std::map<std::string, int>::iterator iter = name_to_symbol_type_.find(non_terminal_str);
  if (iter != name_to_symbol_type_.end()) {
    if (NON_TERMINAL != iter->second) {
      assert(TERMINAL == iter->second);
      std::cerr << "non_terminal name \"" << non_terminal_str
                << "\" conficts with lex name, please check grammar.\n";
      return 1;
    }
  } else {
    name_to_symbol_type_[non_terminal_str] = NON_TERMINAL;
  }
  rule_string_vec.push_back(non_terminal_str);
  ret = match(ASSIGN);
  if (0 != ret) {
    return ret;
  }
  while (look_ahead() == IDENTIFIER) {
    int ret = match(IDENTIFIER);
    if (0 != ret) {
      return ret;
    }
    std::string symbol_str;
    assert(0 == get_token_text(cur_tok_, &symbol_str));
    rule_string_vec.push_back(symbol_str);
  }
  ret = match(SEPARATOR);
  if (0 != ret) {
    return ret;
  }
  rules_.push_back(rule_string_vec);
  return 0;
}

int GrammarConfig::match(int expect_type) {
  if (look_ahead() == expect_type) {
    cur_tok_ = look_ahead_tok_;
    std::string next_tok_text;
    if (look_ahead_tok_.start >= 0 && look_ahead_tok_.start < look_ahead_tok_.end && look_ahead_tok_.end <= text_len_) {
      next_tok_text = std::string(input_ + look_ahead_tok_.start, input_ + look_ahead_tok_.end);
    }
    std::cout << "expect " << std::setw(20) << grammar_config_lex_type_to_str(expect_type)
              << " got (" << std::setw(20) << grammar_config_lex_type_to_str(look_ahead_tok_.type)
              << ", " << std::setw(20) << next_tok_text
              << ") " << std::endl;
    get_next();
    return 0;
  } else {
    std::string next_tok_text;
    if (look_ahead_tok_.start >= 0 && look_ahead_tok_.start < look_ahead_tok_.end && look_ahead_tok_.end <= text_len_) {
      next_tok_text = std::string(input_ + look_ahead_tok_.start, input_ + look_ahead_tok_.end);
    }
    std::cerr << "expect " << grammar_config_lex_type_to_str(expect_type)
              << " got (" << grammar_config_lex_type_to_str(look_ahead_tok_.type)
              << ", " << next_tok_text
              << ") " << std::endl;
    return 1;
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
