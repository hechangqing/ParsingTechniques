#include <iostream>
#include <cassert>
#include <vector>

#include "grammar.h"
#include "top-down-parser.h"

void unit_test() {
  {
    Grammar grammar;
    Symbol S(1000, NON_TERMINAL);
    Symbol A(1001, NON_TERMINAL);
    Symbol B(1002, NON_TERMINAL);
    Symbol C(1003, NON_TERMINAL);
    Symbol D(1004, NON_TERMINAL);
    Symbol a('a', TERMINAL);
    Symbol b('b', TERMINAL);
    Symbol c('c', TERMINAL);
    grammar.set_start_symbol(S);
    {
      std::vector<Symbol> right;
      right.push_back(D);
      right.push_back(C);
      grammar.add_rule(S, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(A);
      right.push_back(B);
      grammar.add_rule(S, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(a);
      grammar.add_rule(A, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(a);
      right.push_back(A);
      grammar.add_rule(A, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(b);
      right.push_back(c);
      grammar.add_rule(B, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(b);
      right.push_back(B);
      right.push_back(c);
      grammar.add_rule(B, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(c);
      grammar.add_rule(C, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(c);
      right.push_back(C);
      grammar.add_rule(C, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(a);
      right.push_back(b);
      grammar.add_rule(D, right);
    }
    {
      std::vector<Symbol> right;
      right.push_back(a);
      right.push_back(D);
      right.push_back(b);
      grammar.add_rule(D, right);
    }
    BackTrackingParser backtracking_paraser;
    backtracking_paraser.compile(grammar);
    backtracking_paraser.parse("aabc");
  }
}

int main() {
  unit_test();
  return 0;
}
