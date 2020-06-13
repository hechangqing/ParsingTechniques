#include <iostream>
#include <cassert>

#include "regular-expression.h"
#include "lexer.h"

void unit_test_regular_expression() {
  {
    RegularExpression regex;
    std::string regex_str = "a";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("ba") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "ab";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("aab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);

    assert(regex.accept("ab") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a?";
    regex.compile(regex_str);

    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("") == true);
    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept(" aaa") == false);
    assert(regex.accept("aaa ") == false);
    assert(regex.accept(" aaa ") == false);
    assert(regex.accept("  aaa ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("aa") == true);
    assert(regex.accept("aaa") == true);
    assert(regex.accept("aaaa") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "a*";
    regex.compile(regex_str);

    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("aa ") == false);
    assert(regex.accept(" a") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept(" aaa") == false);
    assert(regex.accept("aaa ") == false);
    assert(regex.accept(" aaa ") == false);
    assert(regex.accept("  aaa ") == false);

    assert(regex.accept("") == true);
    assert(regex.accept("a") == true);
    assert(regex.accept("aa") == true);
    assert(regex.accept("aaa") == true);
    assert(regex.accept("aaaa") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("ba") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept(" a ") == false);

    assert(regex.accept("a") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(ab)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);

    assert(regex.accept("ab") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[ab]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-zA-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-zA-Z_]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[_a-zA-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-z_A-Z]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("[a-z]") == false);
    assert(regex.accept("[a-zA-Z]") == false);
    assert(regex.accept("[a-zA-Z_]") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("B") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a|b)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(a|b|c)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("ab") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("d") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("b") == true);
    assert(regex.accept("c") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(ab|c|d)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "((ab)|c|d)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(((ab))|(c)|(d))";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("abc") == false);
    assert(regex.accept("abcd") == false);
    assert(regex.accept(" ab") == false);
    assert(regex.accept("ab ") == false);
    assert(regex.accept("ab c") == false);
    assert(regex.accept(" ab ") == false);
    assert(regex.accept(" a ") == false);
    assert(regex.accept("a ") == false);
    assert(regex.accept("b ") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("a-") == false);
    assert(regex.accept("_ ") == false);
    assert(regex.accept("c ") == false);
    assert(regex.accept("d ") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("b") == false);

    assert(regex.accept("ab") == true);
    assert(regex.accept("c") == true);
    assert(regex.accept("d") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[a-zA-Z][a-zA-Z_0-9]*";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("_") == false);
    assert(regex.accept(" ") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept("1") == false);

    assert(regex.accept("a") == true);
    assert(regex.accept("y") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("C") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("a_") == true);
    assert(regex.accept("b0") == true);
    assert(regex.accept("bcd") == true);
    assert(regex.accept("b_cd") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[0-9]+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("_") == false);
    assert(regex.accept(" ") == false);
    assert(regex.accept("0a") == false);

    assert(regex.accept("0") == true);
    assert(regex.accept("1") == true);
    assert(regex.accept("9") == true);
    assert(regex.accept("00") == true);
    assert(regex.accept("01") == true);
    assert(regex.accept("123") == true);
    assert(regex.accept("0518") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[0-9]+.[0-9]+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("_") == false);
    assert(regex.accept(" ") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("12") == false);
    assert(regex.accept("12.") == false);
    assert(regex.accept(".") == false);
    assert(regex.accept(".3") == false);

    assert(regex.accept("0.1") == true);
    assert(regex.accept("1.345") == true);
    assert(regex.accept("20.0") == true);
    assert(regex.accept("1234567.8") == true);
    assert(regex.accept("0518.0555500000") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[ ]";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("_") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept(".") == false);
    assert(regex.accept("\n") == false);
    assert(regex.accept("\t") == false);
    assert(regex.accept("  ") == false);
    assert(regex.accept("\n\n  ") == false);

    assert(regex.accept(" ") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[_a-zA-Z][_a-zA-Z0-9]*";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept(" ") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("+") == false);
    assert(regex.accept("-") == false);
    assert(regex.accept("\\") == false);
    assert(regex.accept("*") == false);
    assert(regex.accept("_-") == false);
    assert(regex.accept("\n") == false);
    assert(regex.accept("ab.c") == false);
    assert(regex.accept("_a-b") == false);
    assert(regex.accept("0aZ") == false);
    assert(regex.accept("R_-") == false);

    assert(regex.accept("_") == true);
    assert(regex.accept("a") == true);
    assert(regex.accept("x") == true);
    assert(regex.accept("z") == true);
    assert(regex.accept("A") == true);
    assert(regex.accept("W") == true);
    assert(regex.accept("Z") == true);
    assert(regex.accept("__") == true);
    assert(regex.accept("_a") == true);
    assert(regex.accept("_x") == true);
    assert(regex.accept("_z") == true);
    assert(regex.accept("_A") == true);
    assert(regex.accept("_P") == true);
    assert(regex.accept("_Z") == true);
    assert(regex.accept("_0") == true);
    assert(regex.accept("_5") == true);
    assert(regex.accept("_9") == true);
    assert(regex.accept("ab") == true);
    assert(regex.accept("aZ") == true);
    assert(regex.accept("a0") == true);
    assert(regex.accept("m_") == true);
    assert(regex.accept("By") == true);
    assert(regex.accept("ZQ") == true);
    assert(regex.accept("U0") == true);
    assert(regex.accept("R_") == true);
    assert(regex.accept("len_of_") == true);
    assert(regex.accept("_len_of_") == true);
    assert(regex.accept("MAX_LENGTH") == true);
    assert(regex.accept("TPYE123_NO_45_") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "(0x|0X)[0-9a-fA-F]+((u|U)?(l|L)?|(l|L)?(u|U)?)";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept("1") == false);
    assert(regex.accept("345") == false);
    assert(regex.accept("0xG") == false);
    assert(regex.accept("0XZ") == false);
    assert(regex.accept("0x") == false);
    assert(regex.accept("0x1 u") == false);
    assert(regex.accept("0x1 l") == false);
    assert(regex.accept("0x1 U") == false);
    assert(regex.accept("0x1 L") == false);
    assert(regex.accept("0x1LL") == false);
    assert(regex.accept("0x1ll") == false);
    assert(regex.accept("0x1uu") == false);
    assert(regex.accept("0x1UU") == false);
    assert(regex.accept("0x1ULL") == false);
    assert(regex.accept("0x1lul") == false);

    assert(regex.accept("0x0") == true);
    assert(regex.accept("0x1") == true);
    assert(regex.accept("0x9") == true);
    assert(regex.accept("0xa") == true);
    assert(regex.accept("0xb") == true);
    assert(regex.accept("0xc") == true);
    assert(regex.accept("0xd") == true);
    assert(regex.accept("0xe") == true);
    assert(regex.accept("0xf") == true);
    assert(regex.accept("0xA") == true);
    assert(regex.accept("0xB") == true);
    assert(regex.accept("0xC") == true);
    assert(regex.accept("0xD") == true);
    assert(regex.accept("0xE") == true);
    assert(regex.accept("0xF") == true);
    assert(regex.accept("0x2f") == true);
    assert(regex.accept("0x2F") == true);
    assert(regex.accept("0x88") == true);
    assert(regex.accept("0xFF") == true);
    assert(regex.accept("0x0FF") == true);
    assert(regex.accept("0xABC") == true);
    assert(regex.accept("0xFFF") == true);
    assert(regex.accept("0xF0F") == true);
    assert(regex.accept("0x12F") == true);
    assert(regex.accept("0xAB43") == true);
    assert(regex.accept("0xAbCd") == true);
    assert(regex.accept("0X0") == true);
    assert(regex.accept("0XF") == true);
    assert(regex.accept("0XFF") == true);
    assert(regex.accept("0X88") == true);
    assert(regex.accept("0XAB43") == true);
    assert(regex.accept("0XAbCd") == true);
    assert(regex.accept("0XAbCdu") == true);
    assert(regex.accept("0XAbCdU") == true);
    assert(regex.accept("0XAbCdl") == true);
    assert(regex.accept("0XAbCdL") == true);
    assert(regex.accept("0XAbCdul") == true);
    assert(regex.accept("0XAbCduL") == true);
    assert(regex.accept("0XAbCdUl") == true);
    assert(regex.accept("0XAbCdUL") == true);
    assert(regex.accept("0XAbCdlu") == true);
    assert(regex.accept("0XAbCdlU") == true);
    assert(regex.accept("0XAbCdLu") == true);
    assert(regex.accept("0XAbCdLU") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "0[0-7]+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("0") == false);
    assert(regex.accept("1") == false);
    assert(regex.accept("123") == false);
    assert(regex.accept("08") == false);
    assert(regex.accept("0178") == false);
    assert(regex.accept("0x1") == false);
    assert(regex.accept("a") == false);
    assert(regex.accept("0a") == false);
    assert(regex.accept("0xG") == false);
    assert(regex.accept("0XZ") == false);
    assert(regex.accept("0x") == false);

    assert(regex.accept("00") == true);
    assert(regex.accept("01") == true);
    assert(regex.accept("07") == true);
    assert(regex.accept("000") == true);
    assert(regex.accept("014") == true);
    assert(regex.accept("077") == true);
    assert(regex.accept("0000") == true);
    assert(regex.accept("0001") == true);
    assert(regex.accept("0101") == true);
    assert(regex.accept("0777") == true);
    assert(regex.accept("00000") == true);
    assert(regex.accept("01001") == true);
    assert(regex.accept("01000") == true);
    assert(regex.accept("01735") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "[0-9]+";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("_") == false);
    assert(regex.accept(" ") == false);
    assert(regex.accept("0a") == false);

    assert(regex.accept("0") == true);
    assert(regex.accept("1") == true);
    assert(regex.accept("9") == true);
    assert(regex.accept("00") == true);
    assert(regex.accept("01") == true);
    assert(regex.accept("123") == true);
    assert(regex.accept("0518") == true);
  }
  {
    RegularExpression regex;
    std::string regex_str = "\'[a-zA-Z]\'";
    regex.compile(regex_str);

    assert(regex.accept("") == false);
    assert(regex.accept("\'aa\'") == false);
    assert(regex.accept("\'000\'") == false);

    assert(regex.accept("\'a\'") == true);
    assert(regex.accept("\'b\'") == true);
    assert(regex.accept("\'z\'") == true);
    assert(regex.accept("\'A\'") == true);
    assert(regex.accept("\'P\'") == true);
    assert(regex.accept("\'Z\'") == true);
  }

}

void unit_test() {
  //{
  //  RegularExpression regex;
  //  Graph nfa;
  //  //std::string str = "((\\(ab)|c|d)\\*";
  //  std::string str = "a\\+";
  //  std::cout << str << std::endl;
  //  std::cout << str.size() << std::endl;
  //  regex.convert_regular_expression_to_NFA(str, &nfa);
  //}
  //{
  //  Lexer lexer;
  //  std::string text = "3.12345 auto ABC ";
  //  std::vector<std::string> regex_rules;
  //  std::vector<int> rule_ids;
  //  regex_rules.push_back("[a-zA-Z][a-zA-Z_0-9]*");
  //  rule_ids.push_back(1);
  //  regex_rules.push_back("[0-9]+");
  //  rule_ids.push_back(2);
  //  regex_rules.push_back("[0-9]+.[0-9]+");
  //  rule_ids.push_back(3);
  //  regex_rules.push_back("[=>]");
  //  rule_ids.push_back(4);
  //  regex_rules.push_back("auto");
  //  rule_ids.push_back(5);
  //  lexer.compile(regex_rules, rule_ids);

  //  int start_pos = 0;

  //  while (start_pos < text.size() ) {
  //    std::vector<int> ret_rules;
  //    int end_pos = 0;
  //    int ret = lexer.next_token(text, start_pos, &ret_rules, &end_pos);
  //    if (ret == 0) { // accept
  //      std::cout << "(Token ";
  //      for (std::vector<int>::iterator iter = ret_rules.begin();
  //           iter != ret_rules.end(); iter++) {
  //        std::cout << *iter << " ";
  //      }
  //      std::cout << ", \"" << text.substr(start_pos, end_pos - start_pos);
  //      std::cout << "\")\n";
  //      start_pos = end_pos;
  //    } else { // not accept
  //      std::cout << "(Unknown, \"" << text[start_pos] << "\")\n";
  //      start_pos += 1;
  //    }
  //  }
  //}
  {
    Lexer lexer;
    std::string text = "3.12345 auto ABC ";
    std::vector<std::string> regex_rules;
    std::vector<int> rule_ids;
    regex_rules.push_back("[a-zA-Z][a-zA-Z_0-9]*");
    rule_ids.push_back(1);
    regex_rules.push_back("[0-9]+");
    rule_ids.push_back(2);
    regex_rules.push_back("[0-9]+.[0-9]+");
    rule_ids.push_back(3);
    regex_rules.push_back("[=>]");
    rule_ids.push_back(4);
    regex_rules.push_back("auto");
    rule_ids.push_back(5);
    regex_rules.push_back("[ ]");
    rule_ids.push_back(6);
    lexer.compile(regex_rules, rule_ids);

    int start_pos = 0;

    while (start_pos < text.size() ) {
      std::vector<int> ret_rules;
      int end_pos = 0;
      int ret = lexer.next_token(text, start_pos, &ret_rules, &end_pos);
      if (ret == 0) { // accept
        std::cout << "(Token ";
        for (std::vector<int>::iterator iter = ret_rules.begin();
             iter != ret_rules.end(); iter++) {
          std::cout << *iter << " ";
        }
        std::cout << ", \"" << text.substr(start_pos, end_pos - start_pos);
        std::cout << "\")\n";
        start_pos = end_pos;
      } else { // not accept
        std::cout << "(Unknown, \"" << text[start_pos] << "\")\n";
        start_pos += 1;
      }
    }
  }
}

void debug() {
  //{
  //  RegularExpression regex;
  //  std::string regex_str = "a";
  //  Graph a;
  //  regex.convert_regular_expression_to_NFA(regex_str, &a);
  //  std::cout << "begin " << std::string(10, '-') << std::endl;
  //  std::cout << a.to_str();
  //  std::cout << "end " << std::string(10, '-') << std::endl;

  //  Graph b;
  //  regex.convert_regular_expression_to_NFA("b", &b);
  //  std::cout << "begin " << std::string(10, '-') << std::endl;
  //  std::cout << b.to_str();
  //  std::cout << "end " << std::string(10, '-') << std::endl;

  //  Graph c;
  //  Graph::concate_fa(a, b, &c);
  //  std::cout << c.to_str();
  //}

  //{
  //  RegularExpression regex;
  //  std::string regex_str = "ab";
  //  Graph nfa;
  //  regex.convert_regular_expression_to_NFA(regex_str, &nfa);
  //  std::cout << "eps-nfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << nfa.to_str();
  //  std::cout << "eps-nfa end " << std::string(10, '-') << std::endl;

  //  Graph::eliminate_eps_arc(&nfa);

  //  std::cout << "nfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << nfa.to_str();
  //  std::cout << "nfa end " << std::string(10, '-') << std::endl;

  //  Graph dfa;
  //  Graph::convert_nfa_to_dfa(nfa, &dfa);
  //  std::cout << "dfa begin " << std::string(10, '-') << std::endl;
  //  std::cout << dfa.to_str();
  //  std::cout << "dfa end " << std::string(10, '-') << std::endl;
  //}

  {
    RegularExpression regex;
    std::string regex_str = "[ ]";
    Graph nfa;
    regex.convert_regular_expression_to_NFA(regex_str, &nfa);
    std::cout << "eps-nfa begin " << std::string(10, '-') << std::endl;
    std::cout << nfa.to_str();
    std::cout << "eps-nfa end " << std::string(10, '-') << std::endl;

    Graph::eliminate_eps_arc(&nfa);

    std::cout << "nfa begin " << std::string(10, '-') << std::endl;
    std::cout << nfa.to_str();
    std::cout << "nfa end " << std::string(10, '-') << std::endl;

    Graph dfa;
    Graph::convert_nfa_to_dfa(nfa, &dfa);
    std::cout << "dfa begin " << std::string(10, '-') << std::endl;
    std::cout << dfa.to_str();
    std::cout << "dfa end " << std::string(10, '-') << std::endl;
  }
}

int main() {
  unit_test_regular_expression();
  //unit_test();
  //debug();
  return 0;
}
