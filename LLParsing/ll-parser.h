#ifndef LL_PARSER_H
#define LL_PARSER_H

#include "grammar-config.h"
#include "common-types.h"
#include "../Lexer/lexer.h"

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Symbol {
  Symbol() : id(0), type(UNDEFINE), rule_idx(0) { }
  Symbol(int new_id, int new_type) : id(new_id), type(new_type), rule_idx(0) { }
  int id;
  int type;
  int rule_idx;
  std::vector<int> rule_indices;
};

typedef std::vector<Symbol> RuleRight;

class LLParser {
  enum {
    INVALID_ID = -2,
    LEX_TYPE_UNKNOWN = -1,
    END_MARK = 0,
    ID_START = 1
  };
  struct LexToken {
    LexToken() : type(END_MARK), start(0), end(0) { }
    int type;
    int start;
    int end;
  };
public:
  LLParser() : id_cnt_(ID_START) {
    eps_id_ = id_cnt_++;
    name_to_id_["EPSILON"] = eps_id_;
    id_to_name_[eps_id_] = "EPSILON";
    skip_token_type_ = INVALID_ID;
  }
  Symbol make_symbol(const std::string &name, int symbol_type);
  int load_grammar(std::istream &is);
  int add_rule(const Symbol &left, const RuleRight &right);
  int set_start(const Symbol &start_symbol);
  int compile();
  int parse(const std::string &input_text);
  int backtracking_parse(const std::string &input_text);
  int compute_first_set();
  int first(RuleRight::iterator beg, RuleRight::iterator end, std::set<int> *out_set);
  int compute_follow_set();
  int produce_parse_table();
  void print_first_set();
  void print_follow_set();
  void print_parse_table();
  const std::string &id_to_name(int id) const;
  std::string rule_right_to_str(int rule_non_terminal, int rule_right_idx) const;
  int next_token(LexToken *tok);
  int get_next_token(LexToken *tok);
  int get_token_text(const LexToken &tok, std::string *str);
  int get_rule_right(int non_terminal_id, int terminal_id, RuleRight *rule_right);
  int get_rule_right_with_id(int non_terminal_id, int rule_right_idx, RuleRight *rule_right);
  int get_rule_right_indices(int non_terminal_id, int terminal_id, std::vector<int> *rule_right_indices);
private:
  std::map<int, Symbol> id_to_left_;
  std::map<int, std::vector<RuleRight> > id_to_rules_;
  std::map<std::string, int> name_to_id_;
  std::map<int, std::string> id_to_name_;
  int id_cnt_;
  int eps_id_;
  std::map<int, std::set<int> > id_to_first_set_;
  std::map<int, std::set<int> > id_to_follow_set_;
  std::map<int, std::map<int, std::set<int> > > parse_table_;
  Symbol start_symbol_;
  GrammarConfig grammar_config_;
  std::string grammar_text_;

  std::vector<Symbol> analysis_stack_;
  std::vector<Symbol> prediction_stack_;

  Lexer lexer_;
  std::string input_text_;
  const char *input_;
  int start_pos_;
  int text_len_;
  LexToken look_ahead_tok_;
  LexToken cur_tok_;
  int skip_token_type_;

  std::vector<LexToken> tokens_;
};

#endif // LL_PARSER_H
