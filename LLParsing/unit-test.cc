#include "ll-parser.h"
#include "grammar-config.h"

#include <iostream>
#include <fstream>

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
      right.push_back(parser.make_symbol("EPSILON", TERMINAL));
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
  {
    LLParser parser;
    {
      Symbol left = parser.make_symbol("S", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("E", NON_TERMINAL));
      parser.set_start(left);
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("E", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("E", NON_TERMINAL));
      right.push_back(parser.make_symbol("Q", NON_TERMINAL));
      right.push_back(parser.make_symbol("F", NON_TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("E", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("F", NON_TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("F", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("a", TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Q", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("+", TERMINAL));
      parser.add_rule(left, right);
    }
    {
      Symbol left = parser.make_symbol("Q", NON_TERMINAL);
      RuleRight right;
      right.push_back(parser.make_symbol("-", TERMINAL));
      parser.add_rule(left, right);
    }
    parser.compile();
  }
  {
    LLParser parser;
    std::ifstream is("test_data/grammar2.txt");
    int ret = parser.load_grammar(is);
    if (0 != ret) {
      std::cout << "grammar error\n";
    } else {
      parser.compile();
    }
    std::string text = "name1 abc ? ! ( name2 )";
    parser.parse(text);
  }
}

void unit_test_grammar_config() {
  std::ifstream is("test_data/grammar.txt", std::ifstream::binary);
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);

  std::string text;
  text.resize(length);

  is.read((char*)text.data(), length);

  GrammarConfig config;
  config.parse_config(text.data(), text.size());
  //config.init_input(text.data(), text.size());
  //Token tok;
  //while (config.next_token(&tok) == 0) {
  //  if (tok.type != WHITE_SPACE) {
  //    std::cout << "(" << tok.type << ", "
  //              << grammar_config_lex_type_to_str(tok.type)
  //              << ", \""
  //              << text.substr(tok.start, tok.end - tok.start)
  //              << "\")\n";
  //  }
  //}

}

int main() {
  unit_test_LL1();
  //unit_test_grammar_config();
  return 0;
}
