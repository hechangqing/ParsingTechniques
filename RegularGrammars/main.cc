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
  return 0;
}

int main(int argc, char **argv) {
  unit_test();
  return 0;
}
