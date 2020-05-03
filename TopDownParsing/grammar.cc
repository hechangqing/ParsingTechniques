#include "grammar.h"

#include <cassert>

bool Grammar::has_rules(const Symbol& symbol) {
  std::map<int, Rule>::iterator iter = id_to_rules_.find(symbol.id);
  if (iter != id_to_rules_.end()) {
    assert(symbol.type == iter->second.left.type);
    return true;
  } else {
    return false;
  }
}

std::vector<std::vector<Symbol> >& Grammar::get_rules(const Symbol& symbol) {
  assert(has_rules(symbol));
  return id_to_rules_[symbol.id].right_vec;
}

int Grammar::add_rule(const Symbol& left, const std::vector<Symbol>& right) {
  assert(left.type == NON_TERMINAL);
  std::map<int, Rule>::iterator iter = id_to_rules_.find(left.id);
  if (iter != id_to_rules_.end()) {
    assert(iter->second.left.id == left.id);
    assert(iter->second.left.type == left.type);
    iter->second.right_vec.push_back(right);
  } else {
    Rule rule;
    rule.left = left;
    rule.right_vec.push_back(right);
    id_to_rules_[left.id] = rule;
  }
  return 0;
}

