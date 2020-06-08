#include <iostream>
#include <cassert>

#include "regular-expression.h"
#include "lexer.h"

void unit_test_regular_expression() {
  {
    RegularExpression regex;
    std::string regex_str = "a";
    regex.compile(regex_str);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == false);
  }
}

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

void debug() {
  //{
  //  RegularExpression regex;
  //  std::string regex_str = "a";
  //  Graph a;
  //  regex.convert_regular_expression_to_NFA(regex_str, &a);
  //  std::cout << "begin " << std::string(10, '-') << std::endl;
  //  std::cout << a.to_str();
  //  std::cout << "end " << std::string(10, '-') << std::endl;

  //  Graph b;
  //  regex.convert_regular_expression_to_NFA("b", &b);
  //  std::cout << "begin " << std::string(10, '-') << std::endl;
  //  std::cout << b.to_str();
  //  std::cout << "end " << std::string(10, '-') << std::endl;

  //  Graph c;
  //  Graph::concate_fa(a, b, &c);
  //  std::cout << c.to_str();
  //}

  {
    RegularExpression regex;
    std::string regex_str = "ab";
    Graph nfa;
    regex.convert_regular_expression_to_NFA(regex_str, &nfa);
    std::cout << "eps-nfa begin " << std::string(10, '-') << std::endl;
    std::cout << nfa.to_str();
    std::cout << "eps-nfa end " << std::string(10, '-') << std::endl;

    Graph::eliminate_eps_arc(&nfa);

    std::cout << "nfa begin " << std::string(10, '-') << std::endl;
    std::cout << nfa.to_str();
    std::cout << "nfa end " << std::string(10, '-') << std::endl;

    Graph dfa;
    Graph::convert_nfa_to_dfa(nfa, &dfa);
    std::cout << "dfa begin " << std::string(10, '-') << std::endl;
    std::cout << dfa.to_str();
    std::cout << "dfa end " << std::string(10, '-') << std::endl;
  }
}

int main() {
  unit_test_regular_expression();
  //unit_test();
  //debug();
  return 0;
}
