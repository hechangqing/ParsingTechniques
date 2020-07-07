#include "ll-parser.h"

#include <iostream>
#include <cassert>

Symbol LLParser::make_symbol(const std::string &name, int symbol_type) {
  std::map<std::string, int>::iterator iter = name_to_id_.find(name);
  if (iter != name_to_id_.end()) {
    return Symbol(iter->second, symbol_type);
  } else {
    int new_id = id_cnt_++;
    name_to_id_[name] = new_id;
    id_to_name_[new_id] = name;
    return Symbol(name_to_id_[name], symbol_type);
  }
}

int LLParser::add_rule(const Symbol &left, const RuleRight &right) {
  id_to_left_[left.id] = left;
  id_to_rules_[left.id].push_back(right);
  return 0;
}

int LLParser::set_start(const Symbol &start_symbol) {
  start_symbol_ = start_symbol;
  return 0;
}

int LLParser::compile() {
  compute_first_set();
  compute_follow_set();
  produce_parse_table();
  print_parse_table();
  return 0;
}

int LLParser::compute_first_set() {
  for (std::map<int, Symbol>::iterator iter = id_to_left_.begin(); iter != id_to_left_.end(); iter++) {
    id_to_first_set_[iter->first] = std::set<int>();
  }
  bool new_addition = true;
  while (new_addition) {
    new_addition = false;
    for (std::map<int, std::vector<RuleRight> >::iterator iter = id_to_rules_.begin();
         iter != id_to_rules_.end(); iter++) {
      int non_terminal_id = iter->first;
      std::set<int> &target_set = id_to_first_set_[non_terminal_id];
      std::vector<RuleRight> &rule_right_vec = iter->second;
      for (int i = 0; i < rule_right_vec.size(); i++) {
        RuleRight &this_right = rule_right_vec[i];
        int this_eps_cnt = 0;
        for (RuleRight::iterator sym_iter = this_right.begin();
             sym_iter != this_right.end(); sym_iter++) {
          if (sym_iter->type == NON_TERMINAL) {
            std::set<int> &ui = id_to_first_set_[sym_iter->id];
            for (std::set<int>::iterator tmp_iter = ui.begin(); tmp_iter != ui.end(); tmp_iter++) {
              if (target_set.find(*tmp_iter) == target_set.end() && *tmp_iter != eps_id_) {
                target_set.insert(*tmp_iter);
                new_addition = true;
              }
            }
            if (ui.find(eps_id_) == ui.end()) {
              break;
            } else {
              this_eps_cnt++;
            }
          } else {
            if (target_set.find(sym_iter->id) == target_set.end()) {
              target_set.insert(sym_iter->id);
              new_addition = true;
            }
            break;
          }
        }
        if (this_eps_cnt == this_right.size()) {
          target_set.insert(eps_id_);
        }
      }
    }
    print_first_set();
  }
  return 0;
}

void LLParser::print_first_set() {
  std::cout << "CURRENT FIRSETS OF ALL NON_TERMINALS:\n";
  for (std::map<int, std::set<int> >::iterator iter = id_to_first_set_.begin(); iter != id_to_first_set_.end(); iter++) {
    assert(id_to_name_.find(iter->first) != id_to_name_.end());
    const std::string &name = id_to_name_[iter->first];
    std::cout << "FIRST(" << name << ") : ";
    for (std::set<int>::iterator set_iter = iter->second.begin(); set_iter != iter->second.end(); set_iter++) {
      assert(id_to_name_.find(*set_iter) != id_to_name_.end());
      const std::string &terminal_name = id_to_name_[*set_iter];
      std::cout << "\"" << terminal_name << "\", ";
    }
    std::cout << std::endl;
  }
}

void LLParser::print_follow_set() {
  std::cout << "CURRENT FOLLOW SETS OF ALL NON_TERMINALS:\n";
  for (std::map<int, std::set<int> >::iterator iter = id_to_follow_set_.begin(); iter != id_to_follow_set_.end(); iter++) {
    assert(id_to_name_.find(iter->first) != id_to_name_.end());
    const std::string &name = id_to_name_[iter->first];
    std::cout << "FOLLOW(" << name << ") : ";
    for (std::set<int>::iterator set_iter = iter->second.begin(); set_iter != iter->second.end(); set_iter++) {
      if (*set_iter != END_MARK) {
        assert(id_to_name_.find(*set_iter) != id_to_name_.end());
        const std::string &terminal_name = id_to_name_[*set_iter];
        std::cout << "\"" << terminal_name << "\", ";
      } else {
        std::cout << "#, ";
      }
    }
    std::cout << std::endl;
  }
}

int LLParser::first(RuleRight::iterator beg, RuleRight::iterator end, std::set<int> *out_set) {
  assert(out_set != NULL);
  out_set->clear();
  while (beg != end) {
    if (beg->type == NON_TERMINAL) {
      for (std::set<int>::iterator sym_iter = id_to_first_set_[beg->id].begin(); sym_iter != id_to_first_set_[beg->id].end(); sym_iter++) {
        if (*sym_iter != eps_id_) {
          out_set->insert(*sym_iter);
        }
      }
      if (id_to_first_set_[beg->id].find(eps_id_) == id_to_first_set_[beg->id].end()) {
        break;
      }
    } else {
      assert(beg->type == TERMINAL);
      out_set->insert(beg->id);
      break;
    }
    beg++;
  }
  if (beg == end) {
    out_set->insert(eps_id_);
  }
  return 0;
}

int LLParser::compute_follow_set() {
  for (std::map<int, Symbol>::iterator iter = id_to_left_.begin(); iter != id_to_left_.end(); iter++) {
    id_to_follow_set_[iter->first] = std::set<int>();
  }
  assert(start_symbol_.type == NON_TERMINAL);
  id_to_follow_set_[start_symbol_.id].insert(END_MARK);
  bool new_addition = true;
  while (new_addition) {
    new_addition = false;
    for (std::map<int, std::vector<RuleRight> >::iterator iter = id_to_rules_.begin(); iter != id_to_rules_.end(); iter++) {
      std::vector<RuleRight> &rule_right_vec = iter->second;
      for (std::vector<RuleRight>::iterator rule_right_iter = rule_right_vec.begin(); rule_right_iter != rule_right_vec.end(); rule_right_iter++) {
        RuleRight &this_rule_right = *rule_right_iter;
        for (RuleRight::iterator sym_iter = this_rule_right.begin(); sym_iter != this_rule_right.end(); sym_iter++) {
          Symbol &this_symbol = *sym_iter;
          if (this_symbol.type == NON_TERMINAL) {
            std::set<int> &target_set = id_to_follow_set_[this_symbol.id];
            std::set<int> first_set;
            first(sym_iter + 1, this_rule_right.end(), &first_set);
            for (std::set<int>::iterator tmp_iter = first_set.begin(); tmp_iter != first_set.end(); tmp_iter++) {
              if (target_set.find(*tmp_iter) == target_set.end() && *tmp_iter != eps_id_) {
                target_set.insert(*tmp_iter);
                new_addition = true;
              }
            }
            if (first_set.find(eps_id_) != first_set.end()) {
              for (std::set<int>::iterator tmp_iter = id_to_follow_set_[iter->first].begin(); tmp_iter != id_to_follow_set_[iter->first].end(); tmp_iter++) {
                if (target_set.find(*tmp_iter) == target_set.end()) {
                  target_set.insert(*tmp_iter);
                  new_addition = true;
                }
              }
            }
          }
        }
      }
    }
    print_follow_set();
  }
  return 0;
}

int LLParser::produce_parse_table() {
  for (std::map<int, std::vector<RuleRight> >::iterator iter = id_to_rules_.begin(); iter != id_to_rules_.end(); iter++) {
    int this_non_terminal_id = iter->first;
    std::vector<RuleRight> &rule_right_vec = iter->second;
    for (std::vector<RuleRight>::iterator rule_right_iter = rule_right_vec.begin(); rule_right_iter != rule_right_vec.end(); rule_right_iter++) {
      RuleRight &rule_right = *rule_right_iter;
      int rule_right_idx = rule_right_iter - rule_right_vec.begin();
      std::set<int> first_set;
      first(rule_right.begin(), rule_right.end(), &first_set);
      for (std::set<int>::iterator set_iter = first_set.begin(); set_iter != first_set.end(); set_iter++) {
        if (*set_iter != eps_id_) {
          parse_table_[this_non_terminal_id][*set_iter].insert(rule_right_idx);
        }
      }
      if (first_set.find(eps_id_) != first_set.end()) {
        for (std::set<int>::iterator tmp_iter = id_to_follow_set_[this_non_terminal_id].begin(); tmp_iter != id_to_follow_set_[this_non_terminal_id].end(); tmp_iter++) {
          parse_table_[this_non_terminal_id][*tmp_iter].insert(rule_right_idx);
        }
      }
    }
  }
  return 0;
}

void LLParser::print_parse_table() {
  for (std::map<int, std::map<int, std::set<int> > >::iterator iter_table = parse_table_.begin(); iter_table != parse_table_.end(); iter_table++) {
    int non_terminal_id = iter_table->first;
    std::map<int, std::set<int> > &table_line = iter_table->second;
    for (std::map<int, std::set<int> >::iterator iter_table_entry = table_line.begin(); iter_table_entry != table_line.end(); iter_table_entry++) {
      int terminal_id = iter_table_entry->first;
      std::set<int> &rule_right_ids = iter_table_entry->second;
      for (std::set<int>::iterator tmp_iter = rule_right_ids.begin(); tmp_iter != rule_right_ids.end(); tmp_iter++) {
        std::cout << "[ " << id_to_name(non_terminal_id) << ", " << id_to_name(terminal_id) << "] : "
                  << rule_right_to_str(non_terminal_id, *tmp_iter)
                  << "\n";
      }
    }
  }
}

const std::string &LLParser::id_to_name(int id) const {
  assert(id_to_name_.find(id) != id_to_name_.end());
  return id_to_name_.at(id);
}

std::string LLParser::rule_right_to_str(int rule_non_terminal, int rule_right_idx) const {
  const std::vector<RuleRight> &rule_right_vec = id_to_rules_.at(rule_non_terminal);
  assert(rule_right_idx < rule_right_vec.size());
  const RuleRight &rule_right = rule_right_vec.at(rule_right_idx);
  std::string out;
  for (RuleRight::const_iterator iter = rule_right.begin(); iter != rule_right.end(); iter++) {
    if (iter->type == NON_TERMINAL) {
      out += id_to_name(iter->id);
    } else {
      assert(iter->type == TERMINAL);
      out += "\"" + id_to_name(iter->id) + "\"";
    }
    out += " ";
  }
  return out;
}

int LLParser::load_grammar(std::istream &is) {
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);

  grammar_text_.resize(length);
  is.read((char*)grammar_text_.data(), length);
  int ret = grammar_config_.parse_config(grammar_text_.data(), grammar_text_.size());
  if (0 != ret) {
    return ret;
  }

  std::vector<std::pair<std::string, std::string> > &lex_defs = grammar_config_.get_lex_defs();
  std::vector<std::string> regex_rules;
  std::vector<int> rule_ids;
  for (int i = 0; i < lex_defs.size(); i++) {
    std::string &this_regex = lex_defs[i].second;
    Symbol this_symbol = make_symbol(lex_defs[i].first, TERMINAL);
    assert(TERMINAL == grammar_config_.name_to_type(lex_defs[i].first));
    regex_rules.push_back(this_regex);
    rule_ids.push_back(this_symbol.id);
    std::cout << " regex " << regex_rules.back() << " rule_id " << rule_ids.back() << std::endl;
  }
  lexer_.compile(regex_rules, rule_ids);

  std::vector<std::vector<std::string> > &rules = grammar_config_.get_rules();
  for (int i = 0; i < rules.size(); i++) {
    std::vector<std::string> &this_rule = rules[i];
    assert(this_rule.size() >= 2);
    Symbol left = make_symbol(this_rule[0], grammar_config_.name_to_type(this_rule[0]));
    RuleRight right;
    for (int j = 1; j < this_rule.size(); j++) {
      right.push_back(make_symbol(this_rule[j], grammar_config_.name_to_type(this_rule[j])));
    }
    add_rule(left, right);
  }

  std::string start_symbol_str = grammar_config_.get_start_symbol();
  Symbol start_symbol = make_symbol(start_symbol_str, grammar_config_.name_to_type(start_symbol_str));
  set_start(start_symbol);
  return 0;
}

int LLParser::next_token(LexToken *tok) {
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
    tok->type = END_MARK;
    tok->start = -1;
    tok->end = -1;
    return 1;
  }
}

int LLParser::parse(const std::string &input_text) {
  input_text_ = input_text;
  input_ = input_text_.data();
  text_len_ = input_text_.size();
  start_pos_ = 0;

  while (next_token(&look_ahead_tok_) == 0) {
    std::string token_text;
    get_token_text(look_ahead_tok_, &token_text);
    std::cout << "Token(" << look_ahead_tok_.type << ", "
              << token_text << ")\n";
  }
  return 0;
}

int LLParser::get_token_text(const LexToken &tok, std::string *str) {
  assert(str != NULL);
  if (tok.start >= 0 && tok.start < tok.end && tok.end <= text_len_) {
    *str = std::string(input_ + tok.start, input_ + tok.end);
    return 0;
  } else {
    return 1;
  }
}
