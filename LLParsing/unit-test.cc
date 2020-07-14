#include "ll-parser.h"
#include "grammar-config.h"

#include <iostream>
#include <fstream>
#include <cassert>

void unit_test_LL_json(bool print_example_text) {
  LLParser parser;
  std::ifstream is ("test_data/grammar_json.txt");
  int ret = parser.load_grammar(is);
  if (0 != ret) {
    std::cout << "grammar error\n";
  }
  parser.compile();

  std::vector<std::string> pos_examples = {
    "true",
    "false",
    "null",
    "1.23",
    "-3.57e10",
    "\"\"",
    "\"abc\"",
    "{  \"name\"  	   	 : \"harry\" \n \n }"
  };

  for (int i = 0; i < pos_examples.size(); i++) {
    std::string &pos_example = pos_examples[i];
    if (print_example_text) {
      std::cout << "TEST INPUT: " << pos_example << std::endl;
    }
    assert(0 == parser.parse(pos_example));
    assert(0 == parser.backtracking_parse(pos_example));
  }

  std::vector<std::string> neg_examples = {
    "true } ",
    "{ \"name\" : \"harry\" , }"
  };

  for (int i = 0; i < neg_examples.size(); i++) {
    std::string &neg_example = neg_examples[i];
    if (print_example_text) {
      std::cout << "TEST INPUT: " << neg_example << std::endl;
    }
    assert(0 != parser.parse(neg_example));
    assert(0 != parser.backtracking_parse(neg_example));
  }
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
  unit_test_LL_json(true);
  return 0;
}
