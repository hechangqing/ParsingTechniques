#include "ll-parser.h"
#include "grammar-config.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

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

class Calculator {
public:
  Calculator() {
    std::ifstream is ("test_data/grammar_calculator.txt");
    int ret = parser_.load_grammar(is);
    if (0 != ret) {
      std::cout << "grammar error\n";
    }
    parser_.compile();
  }
  void print_tree(AST *node, int indent = 0) {
    std::cout << std::string(indent, ' ') << node->text << "\n";
    for (int i = 0; i < node->children.size(); i++) {
      print_tree(node->children[i].get(), indent + 2);
    }
  }
  std::shared_ptr<AST> transform_tree(std::shared_ptr<AST> tree, AST *father = NULL) {
    if (tree->text == "additive_expression" ||
        tree->text == "multiplicative_expression") {
      if (tree->children.size() == 3) {
        for (int i = 0; i < tree->children.size() - 1; i++) {
          transform_tree(tree->children[i], NULL);
        }
        transform_tree(tree->children.back(), tree.get());
      } else {
        transform_tree(tree->children.back(), NULL);
      }
      if (father) {
        father->children.pop_back();
        for (int i = 0; i < tree->children.size(); i++) {
          father->children.push_back(tree->children[i]);
        }
      }
    } else {
      for (int i = 0; i < tree->children.size(); i++) {
        transform_tree(tree->children[i], NULL);
      }
    }
    return tree;
  }

  float eval_tree(AST *tree) {
    assert(tree != NULL);
    float result = 0.0;
    if (tree->text == "additive_expression" || tree->text == "multiplicative_expression") {
      assert(tree->children.size() >= 1);
      result = eval_tree(tree->children[0].get());
      int op = OP_UNKNOWN;
      for (int i = 1; i < tree->children.size(); i++) {
        if (i % 2 == 1) {
          if (tree->children[i]->text == "+") {
            op = OP_ADD;
          } else if (tree->children[i]->text == "-") {
            op = OP_SUB;
          } else if (tree->children[i]->text == "*") {
            op = OP_MUL;
          } else if (tree->children[i]->text == "/") {
            op = OP_DIV;
          } else {
            std::cerr << "unknown operator " << tree->children[i]->text << std::endl;
            return 0;
          }
        } else {
          assert(op != OP_UNKNOWN);
          if (OP_ADD == op) {
            result += eval_tree(tree->children[i].get());
          } else if (OP_SUB == op) {
            result -= eval_tree(tree->children[i].get());
          } else if (OP_MUL == op) {
            result *= eval_tree(tree->children[i].get());
          } else if (OP_DIV == op) {
            float sub_result = eval_tree(tree->children[i].get());
            if (sub_result != 0) {
              result /= sub_result;
            } else {
              std::cerr << "div zero error " << std::endl;
              return 0;
            }
          }
        }
      }
    } else if (tree->text == "primary_expression") {
      if (tree->children.size() == 1) {
        return atof(tree->children[0]->text.c_str());
      } else if (tree->children.size() == 3) {
        return eval_tree(tree->children[1].get());
      }
    } else {
      std::cerr << "cannot eval node " << tree->text << std::endl;
      return 0;
    }
  }
  int eval(const std::string &expression, float *result) {
    int ret = parser_.backtracking_parse(expression);
    if (ret != 0) {
      std::cerr << "syntax error\n";
      return ret;
    }
    std::shared_ptr<AST> parse_tree = parser_.get_parse_tree();
    //print_tree(parse_tree.get());
    //std::cout << std::string(10, '-') << std::endl;
    transform_tree(parse_tree);
    //print_tree(parse_tree.get());
    //std::cout << std::string(10, '-') << std::endl;
    *result = eval_tree(parse_tree.get());
    return ret;
  }
private:
  enum Op {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_UNKNOWN
  };
private:
  LLParser parser_;
};

bool check_calculator_result(Calculator &calculator,
                            const std::string &expression,
                            int expect_ret,
                            float expect_value,
                            bool print) {
  float val = 0;
  int ret = calculator.eval(expression, &val);
  if (print) {
    std::cout << "TEST INPUT " << expression << " ret " << ret << " val " << val << std::endl;
  }
  if (ret == expect_ret && fabs(val - expect_value) < 1e-4) {
    return true;
  } else {
    assert(0);
    return false;
  }
}

void unit_test_LL_calculator(bool print_example_text) {
  Calculator calculator;
  std::vector<std::pair<std::string, float> > pos_examples = {
    {"1", 1},
    {"30 + 540", 570},
    {"2 + 30*15", 452},
    {"5 + 7 * 4 + 3", 36},
    {"(5+3) * 4", 32},
    {"5 + 5 + 3 * 2 * 4", 34},
    {"(5+1) * 4 + 3 * 4", 36},
    {"5 + (5+3)*2", 21},
    {"5 - 5 - 5 - 5", -10},
    {"5 - 5 - 5 - (5 - 5 - 5 - (5 - 5 - 5) - 5 - 5 - 5  ) - 5 - 5 - 5", -5},
    {"5 / 5 / 5 / (5 / 5 / 5 / (5 / 5 - 5) - 5 / 5 / 5  ) - 5 / 5 - 5", -6.8},
    {"3.14 + 4.2 * 7.3 / 5.4 - 6.89 + 3.0 - 2.1 * 4.03* 5.3 + (2.7 * 4.3) / (5.1 - 3.2 -3.1)", -49.60112},
    {"3.14 - 4.2 * 7.3 / 5.4 - 6.89 + 3.0 - 2.1 * 4.03* 5.3 + (2.7 * 4.3) / (5.1 - 3.2 -3.1)", -60.95667}
  };

  for (int i = 0; i < pos_examples.size(); i++) {
    std::pair<std::string, float> &pos_example = pos_examples[i];
    check_calculator_result(calculator, pos_example.first, 0, pos_example.second, print_example_text);
  }

  std::vector<std::string> neg_examples = {
    "1 - ",
    "30 ( + 540",
    "30 * + 1",
    "+ 1",
  };

  for (int i = 0; i < neg_examples.size(); i++) {
    std::string &neg_example = neg_examples[i];
    float val = 0;
    int ret = calculator.eval(neg_example, &val);
    std::cout << "TEST INPUT " << neg_example << " ret " << ret << " val " << val << std::endl;
    assert(0 != ret);
  }


}

int main() {
  //unit_test_LL1();
  //unit_test_grammar_config();
  //unit_test_LL_json(true);
  unit_test_LL_calculator(true);
  return 0;
}
