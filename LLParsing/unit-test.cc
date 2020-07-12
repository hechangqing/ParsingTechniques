#include "ll-parser.h"
#include "grammar-config.h"

#include <iostream>
#include <fstream>
#include <cassert>

void unit_test_LL_json() {
  LLParser parser;
  std::ifstream is ("test_data/grammar_json.txt");
  int ret = parser.load_grammar(is);
  if (0 != ret) {
    std::cout << "grammar error\n";
  }
  parser.compile();

  assert(0 == parser.parse("true"));
  assert(0 == parser.parse("false"));
  assert(0 == parser.parse("null"));

  assert(0 == parser.parse("1.23"));
  assert(0 == parser.parse("-3.57e10"));

  assert(0 == parser.parse("\"\""));
  assert(0 == parser.parse("\"abc\""));

  assert(0 == parser.parse("{        \"name\"     	 	   	:   \"harry\" \n\n }"));

  assert(0 != parser.parse("true }"));
  assert(0 != parser.parse("{ \"name\" : \"harry\" , }"));
}

void unit_test_LL1() {
  {
    LLParser parser;
    std::ifstream is("test_data/grammar2.txt");
    int ret = parser.load_grammar(is);
    if (0 != ret) {
      std::cout << "grammar error\n";
    } else {
      parser.compile();
    }
    std::string text = "! somefact ! twofact ? somequestion";
    std::cout << text << "\n";
    assert(0 == parser.parse(text));
    assert(0 == parser.parse("! fact ? question"));
    assert(0 == parser.parse("(! fact ? question) ? twoquestion"));

    assert(1 == parser.parse("("));
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
  //unit_test_LL1();
  //unit_test_grammar_config();
  unit_test_LL_json();
  return 0;
}
