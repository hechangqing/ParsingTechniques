#include "ll-parser.h"

void unit_test_LL1() {
  {
    LLParser parser;
    {
      Symbol left = parser.make_symbol("Session", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("Facts", NON_TERMINAL));
      right.push_back(parser.make_symbol("Question", NON_TERMINAL));
      parser.set_start(left);
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Session", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("(", TERMINAL));
      right.push_back(parser.make_symbol("Session", NON_TERMINAL));
      right.push_back(parser.make_symbol(")", TERMINAL));
      right.push_back(parser.make_symbol("Session", NON_TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Facts", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("Fact", NON_TERMINAL));
      right.push_back(parser.make_symbol("Facts", NON_TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Facts", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("", TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Fact", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("!", TERMINAL));
      right.push_back(parser.make_symbol("STRING", TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Question", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("?", TERMINAL));
      right.push_back(parser.make_symbol("STRING", TERMINAL));
      parser.add_rule(left, right);
    }
    parser.compile();
  }
}

int main() {
  unit_test_LL1();
  return 0;
}
