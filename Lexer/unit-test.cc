#include <iostream>
#include <cassert>

#include "regular-expression.h"

void unit_test() {
  {
    RegularExpression regex;
    Graph nfa;
    regex.convert_regular_expression_to_NFA("((ab)|c|d)", &nfa);
  }
}

int main() {
  unit_test();
  return 0;
}
