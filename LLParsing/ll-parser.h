#ifndef LL_PARSER_H
#define LL_PARSER_H

#include <vector>
#include <string>
#include <set>
#include <map>

enum SymbolType {
  NON_TERMINAL,
  TERMINAL,
  UNDEFINE
};

struct Symbol {
  Symbol() : id(0), type(UNDEFINE) { }
  Symbol(int new_id, int new_type) : id(new_id), type(new_type) { }
  int id;
  int type;
};

typedef std::vector<Symbol> RuleRight;

class LLParser {
public:
  LLParser() : id_cnt_(0) {
    eps_id_ = id_cnt_++;
    name_to_id_[""] = eps_id_;
    id_to_name_[eps_id_] = "";
  }
  Symbol make_symbol(const std::string &name, int symbol_type);
  int add_rule(const Symbol &left, const RuleRight &right);
  int compile();
  int compute_first_set();
  void print_first_set();
private:
  std::map<int, Symbol> id_to_left_;
  std::map<int, std::vector<RuleRight> > id_to_rules_;
  std::map<std::string, int> name_to_id_;
  std::map<int, std::string> id_to_name_;
  int id_cnt_;
  int eps_id_;
  std::map<int, std::set<int> > id_to_first_set_;
};

#endif // LL_PARSER_H
