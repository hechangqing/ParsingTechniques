#ifndef AST_H
#define AST_H

#include <memory>
#include <string>

struct AST {
  std::string text;
  std::vector<std::shared_ptr<AST> > children;
};

#endif
