#include <iostream>
#include <cassert>

#include "regular-expression.h"

void unit_test() {
  {
    RegularExpression regex;
    Graph nfa;
    std::string str = "((\\(ab)|c|d)";
    std::cout << str << std::endl;
    std::cout << str.size() << std::endl;
    regex.convert_regular_expression_to_NFA(str, &nfa);
  }
}

int main() {
  unit_test();
  return 0;
}
