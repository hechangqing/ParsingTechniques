// grammar
// S -> D C | A B
// A -> a | a A
// B -> b c | b B c
// D -> a b | a D b
// C -> c | c C

#include <iostream>
#include <string>
#include <vector>

class Parser {
public:
  Parser() : is_speculate_(0) { }

  void set_input(const char *text, int text_length) {
    text_ = text;
    text_length_ = text_length;
    read_idx_ = 0;
  }

  std::string generator_error_msg(const std::string &expecting) {
    std::string str = "expecting " + expecting;
    if (read_idx_ < text_length_) {
      return str + " found " + text_[read_idx_];
    } else {
      return str + ", but no more input";
    }
  }

  std::string generator_error_msg(char expecting) {
    return generator_error_msg(std::to_string(expecting));
  }

  void match(char c) {
    if (read_idx_ < text_length_ && text_[read_idx_] == c) {
      read_idx_++;
    } else {
      throw generator_error_msg(c);
    }
  }

  bool speculate_S_alt1() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      D();
      C();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  bool speculate_S_alt2() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      A();
      B();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  void print_rules() {
    for (int i = 0; i < rules_.size(); i++) {
      std::cout << rules_[i] << "\n";
    }
  }

  void S() {
    if (speculate_S_alt1()) {
      if (!is_speculate_) {
        rules_.push_back("S -> DC");
      }
      D();
      C();
      match('#');
    } else if (speculate_S_alt2()) {
      if (!is_speculate_) {
        rules_.push_back("S -> AB");
      }
      A();
      B();
      match('#');
    } else {
      throw generator_error_msg("S");
    }
    print_rules();
  }

  bool speculate_A_alt1() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('a');
      B();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  bool speculate_A_alt2() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('a');
      A();
      B();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  void A() {
    if (speculate_A_alt1()) {
      if (!is_speculate_) {
        rules_.push_back("A -> a");
      }
      match('a');
    } else if (speculate_A_alt2()) {
      if (!is_speculate_) {
        rules_.push_back("A -> aA");
      }
      match('a');
      A();
    } else {
      throw generator_error_msg("A");
    }
  }

  bool speculate_B_alt1() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('b');
      match('c');
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  bool speculate_B_alt2() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('b');
      B();
      match('c');
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  void B() {
    if (speculate_B_alt1()) {
      if (!is_speculate_) {
        rules_.push_back("B -> bc");
      }
      match('b');
      match('c');
    } else if (speculate_B_alt2()) {
      if (!is_speculate_) {
        rules_.push_back("B -> bBc");
      }
      match('b');
      B();
      match('c');
    } else {
      throw generator_error_msg("B");
    }
  }

  bool speculate_D_alt1() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('a');
      match('b');
      C();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  bool speculate_D_alt2() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('a');
      D();
      match('b');
      C();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  void D() {
    if (speculate_D_alt1()) {
      if (!is_speculate_) {
        rules_.push_back("D -> ab");
      }
      match('a');
      match('b');
    } else if (speculate_D_alt2()) {
      if (!is_speculate_) {
        rules_.push_back("D -> aDb");
      }
      match('a');
      D();
      match('b');
    } else {
      throw generator_error_msg("D");
    }
  }

  bool speculate_C_alt1() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('c');
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  bool speculate_C_alt2() {
    is_speculate_ += 1;;
    bool sucess = true;
    int read_idx = read_idx_;
    try {
      match('c');
      C();
      match('#');
    } catch (std::string &e) {
      sucess = false;
    }
    read_idx_ = read_idx;
    is_speculate_ -= 1;;
    return sucess;
  }

  void C() {
    if (speculate_C_alt1()) {
      if (!is_speculate_) {
        rules_.push_back("C -> c");
      }
      match('c');
    } else if (speculate_C_alt2()) {
      if (!is_speculate_) {
        rules_.push_back("C -> cC");
      }
      match('c');
      C();
    } else {
      throw generator_error_msg("D");
    }
  }

private:
  const char *text_;
  int text_length_;
  int read_idx_;
  std::vector<std::string> rules_;
  int is_speculate_;
};

int main() {
  {
    Parser p;
    std::string str = "aabc#";
    p.set_input(str.data(), str.size());
    try {
      p.S();
    } catch (std::string &e) {
      std::cout << e << std::endl;
    }
  }
  return 0;
}
