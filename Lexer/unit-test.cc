#include <iostream>
#include <cassert>

#include "regular-expression.h"
#include "lexer.h"

void unit_test() {
  {
    RegularExpression regex;
    Graph nfa;
    //std::string str = "((\\(ab)|c|d)\\*";
    std::string str = "a\\+";
    std::cout << str << std::endl;
    std::cout << str.size() << std::endl;
    regex.convert_regular_expression_to_NFA(str, &nfa);
  }
  {
    Lexer lexer;
    std::string text = "ABC.123";
    std::vector<std::string> regex_rules;
    std::vector<int> rule_ids;
    regex_rules.push_back("[a-zA-Z][a-zA-Z_0-9]*");
    rule_ids.push_back(1);
    regex_rules.push_back("[0-9]+");
    rule_ids.push_back(2);
    regex_rules.push_back("[0-9]+.[0-9]+");
    rule_ids.push_back(3);
    regex_rules.push_back("[=>]");
    rule_ids.push_back(4);
    //lexer.compile(regex_rules, rule_ids);

    int start_pos = 0;
    int end_pos = 0;
    //std::vector<int> ret_rules;
    //int ret = lexer.next_token(text, start_pos, &ret_rules, &end_pos);
  }
}

int main() {
  unit_test();
  return 0;
}
