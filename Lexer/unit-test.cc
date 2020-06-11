#include <iostream>
#include <cassert>

#include "regular-expression.h"
#include "lexer.h"

void unit_test_regular_expression() {
  {
    RegularExpression regex;
    std::string regex_str = "a";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("ba") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "ab";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);

    assert(regex.accept("ab") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a?";
    regex.compile(regex_str);

    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("") == true);
    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept(" aaa") == false);
    assert(regex.accept("aaa ") == false);
    assert(regex.accept(" aaa ") == false);
    assert(regex.accept("  aaa ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("aa") == true);
    assert(regex.accept("aaa") == true);
    assert(regex.accept("aaaa") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a*";
    regex.compile(regex_str);

    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept(" aaa") == false);
    assert(regex.accept("aaa ") == false);
    assert(regex.accept(" aaa ") == false);
    assert(regex.accept("  aaa ") == false);

    assert(regex.accept("") == true);
    assert(regex.accept("a") == true);
    assert(regex.accept("aa") == true);
    assert(regex.accept("aaa") == true);
    assert(regex.accept("aaaa") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("ba") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(ab)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);

    assert(regex.accept("ab") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[ab]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-zA-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-zA-Z_]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[_a-zA-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-z_A-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a|b)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a|b|c)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("d") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(ab|c|d)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "((ab)|c|d)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(((ab))|(c)|(d))";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
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

  //{
  //  RegularExpression regex;
  //  std::string regex_str = "ab";
  //  Graph nfa;
  //  regex.convert_regular_expression_to_NFA(regex_str, &nfa);
  //  std::cout << "eps-nfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << nfa.to_str();
  //  std::cout << "eps-nfa end " << std::string(10, '-') << std::endl;

  //  Graph::eliminate_eps_arc(&nfa);

  //  std::cout << "nfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << nfa.to_str();
  //  std::cout << "nfa end " << std::string(10, '-') << std::endl;

  //  Graph dfa;
  //  Graph::convert_nfa_to_dfa(nfa, &dfa);
  //  std::cout << "dfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << dfa.to_str();
  //  std::cout << "dfa end " << std::string(10, '-') << std::endl;
  //}

  {
    RegularExpression regex;
    std::string regex_str = "(a|b)";
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
