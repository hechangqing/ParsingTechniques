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

int LLParser::compile() {
  compute_first_set();
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
}

void LLParser::print_first_set() {
  std::cout << "CURRENT FIRSETS OF ALL TERMINALS:\n";
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
