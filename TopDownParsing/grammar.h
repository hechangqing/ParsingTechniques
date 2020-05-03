#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <string>
#include <sstream>

enum SymbolType {
  TERMINAL,
  NON_TERMINAL,
  INPUT_END
};

struct Symbol {
  Symbol() {}
  Symbol(int new_id, int new_type) : id(new_id), type(new_type) {}
  Symbol(const Symbol& sym) : id(sym.id), type(sym.type), index(sym.index) {}
  int id;
  int type;
  int index;
  std::string to_string() const {
    std::stringstream ss;
    ss << "Symbol(id=" << id << ", type=" << type << ", index=" << index << ")";
    return ss.str();
  }
};

struct Rule {
  Symbol left;
  std::vector<std::vector<Symbol> > right_vec;
};

class Grammar {
public:
  Symbol get_start_symbol() const {
    return start_symbol_;
  }
  void set_start_symbol(const Symbol& start_symbol) {
    start_symbol_ = start_symbol;
  }
  std::vector<std::vector<Symbol> >& get_rules(const Symbol& symbol);
  int add_rule(const Symbol& left, const std::vector<Symbol>& right);
  bool has_rules(const Symbol& symbol);
private:
  Symbol start_symbol_;
  std::map<int, Rule> id_to_rules_;
};

#endif //GRAMMAR_H
