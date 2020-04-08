#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <cassert>

enum SymbolType {
  NON_TERMINAL,
  TERMINAL
};

struct Symbol {
  Symbol() {}
  Symbol(int n_type, int n_id) : type(n_type), id(n_id) {}
  int type;
  int id;
};

struct Rule {
  Symbol left;
  std::vector<Symbol> right;
};

class Grammar {
public:
  int num_rules() const {
    return rules_.size();
  }
  const Rule &get_rule(int i) const {
    assert(i >= 0 && i < rules_.size());
    return rules_[i];
  }
  Symbol get_start() const {
    return start_symbol_;
  }
  int add_rule(const Rule &rule) {
    assert(rule.left.type == NON_TERMINAL);
    rules_.push_back(rule);
    return 0;
  }
  int set_start(const Symbol &s) {
    start_symbol_ = s;
    assert(start_symbol_.type == NON_TERMINAL);
    return 0;
  }
private:
  std::vector<Rule> rules_;
  Symbol start_symbol_;
};

#endif
