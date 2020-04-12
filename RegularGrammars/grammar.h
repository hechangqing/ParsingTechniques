#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <cassert>
#include <sstream>

enum SymbolType {
  NON_TERMINAL,
  TERMINAL
};

struct Symbol {
  Symbol() {}
  Symbol(int n_type, int n_id) : type(n_type), id(n_id) {}
  int type;
  int id;
  std::string to_string() const {
    std::stringstream ss;
    if (type == NON_TERMINAL) {
      ss << "SYMBOL(NON_TERMINAL, " << id << ")";
    } else if (type == TERMINAL) {
      ss << "SYMBOL(TERMINAL, " << char(id) << ")";
    } else {
      assert(0);
    }
    return ss.str();
  }
};

struct Rule {
  Symbol left;
  std::vector<Symbol> right;
  std::string to_string() const {
    std::stringstream ss;
    ss << left.to_string() << " -> ";
    for (size_t i = 0; i < right.size(); i++) {
      ss << right[i].to_string() << " ";
    }
    return ss.str();
  }
};

class Grammar {
public:
  Grammar() : id_(0) { }
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
  int add_id() {
    int ret = id_;
    id_++;
    return ret;
  }
  std::string to_string() const {
    std::stringstream ss;
    for (size_t i = 0; i < rules_.size(); i++) {
      ss << rules_[i].to_string() << "\n";
    }
    return ss.str();
  }
private:
  std::vector<Rule> rules_;
  Symbol start_symbol_;
  int id_;
};

#endif
