#include "graph.h"
#include "grammar.h"
#include "regular-expression.h"

#include <iostream>
#include <cassert>

int unit_test() {
  {
    Symbol S(NON_TERMINAL, 0);
    Symbol A(NON_TERMINAL, 1);
    Symbol B(NON_TERMINAL, 2);
    Symbol a(TERMINAL, 100);
    Symbol b(TERMINAL, 101);
    // S -> A
    Rule rule1;
    rule1.left = S;
    rule1.right.push_back(A);
    // S -> a B
    Rule rule2;
    rule2.left = S;
    rule2.right.push_back(a);
    rule2.right.push_back(B);
    // A -> a A
    Rule rule3;
    rule3.left = A;
    rule3.right.push_back(a);
    rule3.right.push_back(A);
    // A -> <eps>
    Rule rule4;
    rule4.left = A;
    // B -> b B
    Rule rule5;
    rule5.left = B;
    rule5.right.push_back(b);
    rule5.right.push_back(B);
    // B -> b
    Rule rule6;
    rule6.left = B;
    rule6.right.push_back(b);

    Grammar grammar;
    grammar.set_start(S);
    grammar.add_rule(rule1);
    grammar.add_rule(rule2);
    grammar.add_rule(rule3);
    grammar.add_rule(rule4);
    grammar.add_rule(rule5);
    grammar.add_rule(rule6);

    RegularExpression regular_expression;
    Graph nfa;
    Graph dfa;
    regular_expression.convert_regular_grammar_to_NFA(grammar, &nfa);
    std::cout << "nfa " << std::endl;
    std::cout << nfa.to_string() << std::endl;
    std::cout << "-----------------" << std::endl;
    regular_expression.convert_NFA_to_DFA(nfa, &dfa);
    std::cout << "dfa " << std::endl;
    std::cout << dfa.to_string() << std::endl;
    std::cout << "-----------------" << std::endl;
  }

  {
    RegularExpression regular_expression;
    Grammar regular_grammar;
    std::string re_str = "(ab)*(p|q)+";
    regular_expression.convert_regular_expression_to_regular_grammar(re_str, &regular_grammar);
    std::cout << "regular_expression " << re_str << "\n";
    std::cout << "regular_grammar " << "\n";
    std::cout << "-----------------" << std::endl;
    std::cout << regular_grammar.to_string() << std::endl;
    std::cout << "-----------------" << std::endl;
  }

  {
    FastTextSearch ts;
    ts.compile("ababc");
    int i = ts.search("aabababca");
    std::cout << i << std::endl;
  }

  {
    RegularExpression regular_expression;
    Grammar regular_grammar;
    //std::string re_str = "[0123456789][0123456789]*";
    //std::string re_str = "[01]";
    //std::string re_str = "[01]*";
    std::string re_str = "[01][01]*";
    regular_expression.convert_regular_expression_to_regular_grammar(re_str, &regular_grammar);
    std::cout << "regular_expression " << re_str << "\n";
    std::cout << "regular_grammar " << "\n";
    std::cout << "-----------------" << std::endl;
    std::cout << regular_grammar.to_string() << std::endl;
    std::cout << "-----------------" << std::endl;
  }

  return 0;
}

int grep(const std::string& regular_expression, const std::string& text) {
  FastTextSearch ts;
  ts.compile(regular_expression);
  int i = ts.search(text);
  return i;
}

int main(int argc, char **argv) {
  unit_test();
  //if (argc == 3) {
  //  std::string regex_str = argv[1];
  //  std::string text = argv[2];
  //  int ix = grep(regex_str, text);
  //  std::cout << ix << std::endl;
  //}
  return 0;
}
