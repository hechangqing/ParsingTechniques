#include "ll-parser.h"

#include <iostream>
#include <cassert>

LLParser::Symbol LLParser::make_symbol(const std::string &name, int symbol_type) {
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

int LLParser::get_rule_right(int non_terminal_id, int terminal_id, RuleRight *rule_right) {
  assert(rule_right != NULL);
  std::map<int, std::map<int, std::set<int> > >::iterator iter_table = parse_table_.find(non_terminal_id);
  if (iter_table == parse_table_.end()) {
    return 1; // no rule found
  } else {
    std::map<int, std::set<int> > &table_line = iter_table->second;
    std::map<int, std::set<int> >::iterator iter_table_entry = table_line.find(terminal_id);
    if (iter_table_entry == table_line.end()) {
      return 1; // no rule found
    } else {
      std::set<int> &rule_right_ids = iter_table_entry->second;
      assert(rule_right_ids.size() == 1); // LL(1)
      int rule_right_idx = *(rule_right_ids.begin());
      const std::vector<RuleRight> &rule_right_vec = id_to_rules_.at(non_terminal_id);
      assert(rule_right_idx < rule_right_vec.size());
      *rule_right = rule_right_vec.at(rule_right_idx);
      return 0;
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
    if (lex_defs[i].first == "skip") {
      skip_token_type_ = this_symbol.id;
      assert(skip_token_type_ != INVALID_ID);
    }
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
      tok->type = LEX_TYPE_UNKNOWN;
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

int LLParser::get_next_token(LexToken *tok) {
  int ret = next_token(tok);
  while (ret == 0) {
    if (skip_token_type_ != INVALID_ID && tok->type == skip_token_type_) {
      // skip this token
      ret = next_token(tok);
    } else {
      return 0;
    }
  }
  return ret;
}

int LLParser::parse(const std::string &input_text) {
  input_text_ = input_text;
  input_ = input_text_.data();
  text_len_ = input_text_.size();
  start_pos_ = 0;

  analysis_stack_.clear();
  prediction_stack_.clear();

  prediction_stack_.push_back(start_symbol_);

  while (get_next_token(&look_ahead_tok_) == 0) {
    if (look_ahead_tok_.type == LEX_TYPE_UNKNOWN) {
      std::string token_text;
      get_token_text(look_ahead_tok_, &token_text);
      std::cerr << "unexpect lex " << token_text << std::endl;
      return 1;
    }

    if (prediction_stack_.empty()) {
      break;
    } else {
      Symbol top;
      do {
        top = prediction_stack_.back();
        if (top.type == NON_TERMINAL) {
          prediction_stack_.pop_back();
          analysis_stack_.push_back(top);
          RuleRight rule_right;
          int ret = get_rule_right(top.id, look_ahead_tok_.type, &rule_right);
          if (ret == 0) {
            for (int i = rule_right.size() - 1; i >= 0; i--) {
              if (rule_right[i].id != eps_id_) {
                prediction_stack_.push_back(rule_right[i]);
              }
            }
          } else {
            std::cerr << " no rule find " << std::endl;
            break;
          }
        }
      } while (top.type == NON_TERMINAL && !prediction_stack_.empty());
    }
    if (prediction_stack_.empty()) {
      break;
    } else if (prediction_stack_.back().type == NON_TERMINAL) {
      break;
    } else if (prediction_stack_.back().type == TERMINAL) {
      if (prediction_stack_.back().id != look_ahead_tok_.type) {
        std::cerr << " expect " << id_to_name(prediction_stack_.back().id)
                  << " but meet " << id_to_name(look_ahead_tok_.type)
                  << std::endl;
      } else {
        analysis_stack_.push_back(prediction_stack_.back());
        prediction_stack_.pop_back();
      }
    } else {
      assert(0);
    }
    std::string token_text;
    std::string token_type_str;
    get_token_text(look_ahead_tok_, &token_text);
    if (look_ahead_tok_.type != LEX_TYPE_UNKNOWN) {
      assert(id_to_name_.find(look_ahead_tok_.type) != id_to_name_.end());
      token_type_str = id_to_name_[look_ahead_tok_.type];
    }
    std::cout << "Token(" << look_ahead_tok_.type << ", "
              << token_type_str << ", "
              << token_text << ")\n";
  }

  if (prediction_stack_.empty() && look_ahead_tok_.type == END_MARK) {
    for (int i = 0; i < analysis_stack_.size(); i++) {
      Symbol &this_symbol = analysis_stack_[i];
      std::cout << id_to_name(this_symbol.id) << " ";
    }
    std::cout << std::endl;
    return 0;
  } else {
    return 1;
  }
}

int LLParser::get_rule_right_indices(int non_terminal_id, int terminal_id, std::vector<int> *rule_right_indices) {
  assert(rule_right_indices != NULL);
  std::map<int, std::map<int, std::set<int> > >::iterator iter_table = parse_table_.find(non_terminal_id);
  if (iter_table == parse_table_.end()) {
    return 1; // no rule found
  } else {
    std::map<int, std::set<int> > &table_line = iter_table->second;
    std::map<int, std::set<int> >::iterator iter_table_entry = table_line.find(terminal_id);
    if (iter_table_entry == table_line.end()) {
      return 1; // no rule found
    } else {
      std::set<int> &rule_right_ids = iter_table_entry->second;
      if (rule_right_ids.size() == 0) {
        return 1; // no rule found
      }
      rule_right_indices->clear();
      for (std::set<int>::iterator iter = rule_right_ids.begin(); iter != rule_right_ids.end(); iter++) {
        rule_right_indices->push_back(*iter);
      }
      return 0;
    }
  }
}

int LLParser::get_rule_right_with_id(int non_terminal_id, int rule_right_idx, RuleRight *rule_right) {
  assert(rule_right != NULL);
  const std::vector<RuleRight> &rule_right_vec = id_to_rules_.at(non_terminal_id);
  assert(rule_right_idx < rule_right_vec.size());
  *rule_right = rule_right_vec.at(rule_right_idx);
  return 0;
}

int LLParser::backtracking_parse(const std::string &input_text) {
  input_text_ = input_text;
  input_ = input_text_.data();
  text_len_ = input_text_.size();
  start_pos_ = 0;

  analysis_stack_.clear();
  prediction_stack_.clear();

  prediction_stack_.push_back(Symbol(END_MARK, TERMINAL));
  prediction_stack_.push_back(start_symbol_);

  tokens_.clear();
  while (get_next_token(&look_ahead_tok_) == 0) {
    if (look_ahead_tok_.type == LEX_TYPE_UNKNOWN) {
      std::string token_text;
      get_token_text(look_ahead_tok_, &token_text);
      std::cerr << "unexpect lex " << token_text << std::endl;
      return 1;
    }
    tokens_.push_back(look_ahead_tok_);
  }
  {
    LexToken tok;
    tok.type = END_MARK;
    tok.start = 0;
    tok.end = 0;
    tokens_.push_back(tok);
  }

  assert(tokens_.size() >= 1);
  int token_idx = 0;
  look_ahead_tok_ = tokens_[token_idx];
  while (!prediction_stack_.empty()) {
    Symbol top;
    do {
      top = prediction_stack_.back();
      if (top.type == NON_TERMINAL) {
        //std::cout << "top " << print_symbol(top) << std::endl;
        //std::cout << "LL(1) token " << print_token(look_ahead_tok_) << std::endl;

        int ret = get_rule_right_indices(top.id, look_ahead_tok_.type, &top.rule_indices);
        if (ret != 0) {
          std::cerr << " no rule find " << std::endl;
          break;
        }

        top.rule_idx = 0;
        analysis_stack_.push_back(top);
        assert(top.rule_indices.size() > 0);
        RuleRight rule_right;
        ret = get_rule_right_with_id(top.id, top.rule_indices[top.rule_idx], &rule_right);
        assert(ret == 0);
        prediction_stack_.pop_back();
        for (int i = rule_right.size() - 1; i >= 0; i--) {
          if (rule_right[i].id != eps_id_) {
            prediction_stack_.push_back(rule_right[i]);
          }
        }
      }
    } while (top.type == NON_TERMINAL && !prediction_stack_.empty());
    //    for (int i = 0; i < prediction_stack_.size(); i++) {
    //      std::cout << print_symbol(prediction_stack_[i]) << std::endl;
    //    }
    if (prediction_stack_.empty()) {
      break;
    } else if (prediction_stack_.back().type == NON_TERMINAL) {
      break;
    } else if (prediction_stack_.back().type == TERMINAL) {
      if (prediction_stack_.back().id != look_ahead_tok_.type) {
        // backtrack
        bool backtrack = true;
        while (backtrack) {
          while (!analysis_stack_.empty() && analysis_stack_.back().type == TERMINAL) {
            prediction_stack_.push_back(analysis_stack_.back());
            analysis_stack_.pop_back();
            token_idx -= 1;
            assert(token_idx >= 0);
            look_ahead_tok_ = tokens_[token_idx];
          }
          if (analysis_stack_.empty()) {
            backtrack = false;
            break;
          }
          assert(analysis_stack_.back().type == NON_TERMINAL);
          Symbol analysis_stack_top = analysis_stack_.back();
          if (analysis_stack_top.rule_idx + 1 < analysis_stack_top.rule_indices.size()) {
            RuleRight orig_rule_right;
            int ret = get_rule_right_with_id(analysis_stack_top.id,
                                             analysis_stack_top.rule_indices[analysis_stack_top.rule_idx],
                                             &orig_rule_right);
            assert(ret == 0);
            for (int i = 0; i < orig_rule_right.size(); i++) {
              Symbol &this_top = prediction_stack_.back();
              assert(this_top.id == orig_rule_right[i].id);
              prediction_stack_.pop_back();
            }
            analysis_stack_.back().rule_idx += 1;
            assert(analysis_stack_top.rule_idx < analysis_stack_top.rule_indices.size());
            RuleRight new_rule_right;
            ret = get_rule_right_with_id(analysis_stack_top.id,
                                         analysis_stack_top.rule_indices[analysis_stack_.back().rule_idx],
                                             &new_rule_right);
            assert(ret == 0);
            for (int i = new_rule_right.size() - 1; i >= 0; i--) {
              prediction_stack_.push_back(new_rule_right[i]);
            }
            backtrack = false;
          } else {
            RuleRight orig_rule_right;
            int ret = get_rule_right_with_id(analysis_stack_top.id,
                                             analysis_stack_top.rule_indices[analysis_stack_top.rule_idx],
                                             &orig_rule_right);
            assert(ret == 0);
            for (int i = 0; i < orig_rule_right.size(); i++) {
              Symbol &this_top = prediction_stack_.back();
              assert(this_top.id == orig_rule_right[i].id);
              prediction_stack_.pop_back();
            }
            analysis_stack_.back().rule_idx = 0;
            prediction_stack_.push_back(analysis_stack_.back());
            analysis_stack_.pop_back();
          }
        }
        if (analysis_stack_.empty()) {
          break;
        }
        //for (int i = 0; i < prediction_stack_.size(); i++) {
        //  std::cout << print_symbol(prediction_stack_[i]) << std::endl;
        //}
        //std::cout << print_token(look_ahead_tok_) << std::endl;
      } else {
        analysis_stack_.push_back(prediction_stack_.back());
        analysis_stack_.back().tok = look_ahead_tok_;
        prediction_stack_.pop_back();
        if (token_idx + 1 < tokens_.size()) {
          token_idx += 1;
          look_ahead_tok_ = tokens_[token_idx];
        } else {
          break;
        }
      }
    } else {
      assert(0);
    }
    //std::cout << print_token(look_ahead_tok_) << std::endl;
  }

  if (prediction_stack_.empty() && look_ahead_tok_.type == END_MARK) {
    //for (int i = 0; i < analysis_stack_.size(); i++) {
    //  Symbol &this_symbol = analysis_stack_[i];
    //  std::cout << print_symbol(this_symbol);
    //  if (this_symbol.type == TERMINAL) {
    //    std::cout << print_token(this_symbol.tok) << " ";
    //  } else {
    //    std::cout << " ";
    //  }
    //}
    //std::cout << std::endl;
    int start_idx = 0;
    parse_tree_ = generate_parse_tree(&start_idx);
    return 0;
  } else {
    return 1;
  }
}

std::shared_ptr<AST> LLParser::generate_parse_tree(int *start_idx) {
  assert(*start_idx < analysis_stack_.size());
  if (analysis_stack_[*start_idx].type == NON_TERMINAL) {
    Symbol& top_symbol = analysis_stack_[*start_idx];
    RuleRight rule_right;
    int ret = get_rule_right_with_id(top_symbol.id,
                                     top_symbol.rule_indices[top_symbol.rule_idx],
                                     &rule_right);
    assert(ret == 0);
    std::shared_ptr<AST> root(new AST());
    root->text = id_to_name(top_symbol.id);
    *start_idx += 1;
    for (int i = 0; i < rule_right.size(); i++) {
      root->children.push_back(generate_parse_tree(start_idx));
    }
    return root;
  } else if (analysis_stack_[*start_idx].type == TERMINAL) {
    std::shared_ptr<AST> node(new AST());
    int ret = get_token_text(analysis_stack_[*start_idx].tok, &(node->text));
    *start_idx += 1;
    return node;
  } else {
    assert(0);
    return std::shared_ptr<AST>();
  }
}

std::shared_ptr<AST> LLParser::get_parse_tree() {
  return parse_tree_;
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

std::string LLParser::print_token(const LexToken &tok) {
    std::string token_text;
    std::string token_type_str = "UNKNOWN_TYPE";
    get_token_text(tok, &token_text);
    if (tok.type != LEX_TYPE_UNKNOWN && id_to_name_.find(tok.type) != id_to_name_.end()) {
      assert(id_to_name_.find(tok.type) != id_to_name_.end());
      token_type_str = id_to_name_[tok.type];
    }
    return "Token(" + std::to_string(tok.type) + ", "
              + token_type_str + ", "
              + token_text + ")";
}

std::string LLParser::print_symbol(const Symbol &symbol) {
  std::string result;
  std::string symbol_str = "NOT_FOUND";
  if (id_to_name_.find(symbol.id) != id_to_name_.end()) {
    symbol_str = id_to_name_[symbol.id];
  }
  result += "Symbol(" + std::to_string(symbol.id) + ", " + symbol_str + ", " + (symbol.type == NON_TERMINAL ? "NON_TERMINAL" : "TERMINAL") + ", " + std::to_string(symbol.rule_idx) + ")";
  return result;
}
