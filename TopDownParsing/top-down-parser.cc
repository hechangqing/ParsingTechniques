#include "top-down-parser.h"

#include <cassert>
#include <iostream>

int BackTrackingParser::compile(const Grammar &grammar) {
  grammar_ = grammar;
  return 0;
}

int BackTrackingParser::parse(const std::string &input) {
  Symbol start_symbol = grammar_.get_start_symbol();
  start_symbol.index = -1;
  prediction_stack_.push(start_symbol);
  int input_idx = 0;
  while (!prediction_stack_.empty()) {
    Symbol next_symbol = prediction_stack_.top();
    if (next_symbol.type == NON_TERMINAL) {
      std::vector<std::vector<Symbol> > &right_vec = grammar_.get_rules(next_symbol);
      if (next_symbol.index + 1 < right_vec.size()) {
        next_symbol.index += 1;
        analysis_stack_.push(next_symbol);
        prediction_stack_.pop();
        assert(right_vec.size() >= 1);
        for (int i = right_vec[next_symbol.index].size() - 1; i >= 0; i--) {
          Symbol this_sym = right_vec[next_symbol.index][i];
          this_sym.index = -1;
          prediction_stack_.push(this_sym);
        }
      } else {
        assert(0);
      }
    } else if (next_symbol.type == TERMINAL) {
      if (next_symbol.id == input[input_idx]) {
        analysis_stack_.push(next_symbol);
        prediction_stack_.pop();
        input_idx += 1;
      } else { // backtracking
        bool need_backtracking = true;
        while (need_backtracking) {
          while (analysis_stack_.top().type == TERMINAL) {
            Symbol backtrack_sym = analysis_stack_.top();
            prediction_stack_.push(backtrack_sym);
            analysis_stack_.pop();
            input_idx -= 1;
          }
          if (analysis_stack_.top().type == NON_TERMINAL) {
            Symbol back_track_sym = analysis_stack_.top();
            int this_right_index = back_track_sym.index;
            std::vector<std::vector<Symbol> > &right_vec = grammar_.get_rules(back_track_sym);
            assert(this_right_index < right_vec.size());
            for (int i = 0; i < right_vec[this_right_index].size(); i++) {
              Symbol right_sym = right_vec[this_right_index][i];
              Symbol this_pop = prediction_stack_.top();
              prediction_stack_.pop();
              assert(this_pop.id == right_sym.id && this_pop.type == right_sym.type);
            }
            prediction_stack_.push(analysis_stack_.top());
            analysis_stack_.pop();
            if (this_right_index == right_vec.size() - 1) {
              need_backtracking = true;
            } else {
              need_backtracking = false;
            }
          } else {
            assert(0);
          }
        } // while need backtracking
        // clear right rule index of the symbols after top on prediction stack
        clear_index(&prediction_stack_);
      }
    } else {
      assert(0);
    }
    //
    print_instantaneous_description(input_idx);
  }
  return 0;
}

void BackTrackingParser::clear_index(std::stack<Symbol> *stk) {
  std::vector<Symbol> vec;
  while (!(stk->empty())) {
    vec.push_back(stk->top());
    stk->pop();
  }
  for (int i = 1; i < vec.size(); i++) {
    vec[i].index = -1;
  }
  for (int i = vec.size() - 1; i >= 0; i--) {
    stk->push(vec[i]);
  }
}

static void print_stack(const std::stack<Symbol> &stack, bool inverse) {
  std::stack<Symbol> analysis_stack = stack;
  std::vector<Symbol> analysis_vec;
  while (!analysis_stack.empty()) {
    analysis_vec.push_back(analysis_stack.top());
    analysis_stack.pop();
  }
  if (!inverse) {
    for (int i = analysis_vec.size() - 1; i >= 0; i--) {
      std::cout << analysis_vec[i].to_string() << " ";
    }
  } else {
    for (int i = 0; i < analysis_vec.size(); i++) {
      std::cout << analysis_vec[i].to_string() << " ";
    }
  }
}

int BackTrackingParser::print_instantaneous_description(int input_idx) {
  std::cout << input_idx << std::endl;
  print_stack(analysis_stack_, false);
  std::cout << std::endl;
  print_stack(prediction_stack_, true);
  std::cout << std::endl;
}
