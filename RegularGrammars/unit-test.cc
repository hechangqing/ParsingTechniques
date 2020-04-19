#include "graph.h"
#include "grammar.h"
#include "regular-expression.h"

#include <iostream>
#include <cassert>

void unit_test_regular_expression_match() {
  {
    TextMatch tm;
    tm.compile("ab");

    assert(tm.match("ab") == true);

    assert(tm.match("") == false);
    assert(tm.match("a") == false);
    assert(tm.match("ac") == false);
    assert(tm.match("abc") == false);
  }
  {
    TextMatch tm;
    tm.compile("a*");

    assert(tm.match("") == true);
    assert(tm.match("a") == true);
    assert(tm.match("aa") == true);
    assert(tm.match("aaa") == true);

    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
  }
  {
    TextMatch tm;
    tm.compile("a+");

    assert(tm.match("a") == true);
    assert(tm.match("aa") == true);
    assert(tm.match("aaa") == true);

    assert(tm.match("") == false);
    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
  }
  {
    TextMatch tm;
    tm.compile("a?");

    assert(tm.match("") == true);
    assert(tm.match("a") == true);

    assert(tm.match("aa") == false);
    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("aab") == false);
  }
  {
    TextMatch tm;
    tm.compile("(a)");

    assert(tm.match("a") == true);

    assert(tm.match("") == false);
    assert(tm.match("aa") == false);
    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("aab") == false);
  }
  {
    TextMatch tm;
    tm.compile("(ab)");

    assert(tm.match("ab") == true);

    assert(tm.match("") == false);
    assert(tm.match("a") == false);
    assert(tm.match("b") == false);
    assert(tm.match("aa") == false);
    assert(tm.match("aab") == false);
  }
  {
    TextMatch tm;
    tm.compile("(abc)");

    assert(tm.match("abc") == true);

    assert(tm.match("") == false);
    assert(tm.match("a") == false);
    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("ac") == false);
    assert(tm.match("bc") == false);
    assert(tm.match("abd") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("[a]");

    assert(tm.match("a") == true);

    assert(tm.match("") == false);
    assert(tm.match("b") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("[ab]");

    assert(tm.match("a") == true);
    assert(tm.match("b") == true);

    assert(tm.match("") == false);
    assert(tm.match("c") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("[abc]");

    assert(tm.match("a") == true);
    assert(tm.match("b") == true);
    assert(tm.match("c") == true);

    assert(tm.match("") == false);
    assert(tm.match("d") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("(a|b)");

    assert(tm.match("a") == true);
    assert(tm.match("b") == true);

    assert(tm.match("") == false);
    assert(tm.match("c") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("(a|b|c)");

    assert(tm.match("a") == true);
    assert(tm.match("b") == true);
    assert(tm.match("c") == true);

    assert(tm.match("") == false);
    assert(tm.match("d") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("abcd") == false);
  }
  {
    TextMatch tm;
    tm.compile("(ab)*(p|q)+");

    assert(tm.match("p") == true);
    assert(tm.match("q") == true);
    assert(tm.match("pp") == true);
    assert(tm.match("qq") == true);
    assert(tm.match("pq") == true);
    assert(tm.match("qp") == true);
    assert(tm.match("abp") == true);
    assert(tm.match("abq") == true);
    assert(tm.match("abppq") == true);
    assert(tm.match("abpqq") == true);
    assert(tm.match("ababppq") == true);
    assert(tm.match("ababababp") == true);

    assert(tm.match("") == false);
    assert(tm.match("c") == false);
    assert(tm.match("aq") == false);
    assert(tm.match("ab") == false);
    assert(tm.match("pa") == false);
    assert(tm.match("pab") == false);
    assert(tm.match("abab") == false);
    assert(tm.match("ababc") == false);
    assert(tm.match("ababa") == false);
  }
  {
    TextMatch tm;
    tm.compile("(f|$) *[0123456789]+((.[0123456789][0123456789])|(.--))?");

    assert(tm.match("f34") == true);
    assert(tm.match("$ 3") == true);
    assert(tm.match("$ 790") == true);
    assert(tm.match("$ 35.43") == true);
    assert(tm.match("$ 35.--") == true);
    assert(tm.match("$ 0.--") == true);
    assert(tm.match("$ 134.35") == true);

    assert(tm.match("") == false);
    assert(tm.match("f") == false);
    assert(tm.match("f.4") == false);
    assert(tm.match("$3.4") == false);
    assert(tm.match("$3a") == false);
    assert(tm.match("$30.3") == false);
    assert(tm.match("0") == false);
  }
}

int main() {
  unit_test_regular_expression_match();
  return 0;
}
