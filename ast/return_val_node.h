#ifndef __M19_RETURN_VAL_NODE_H__
#define __M19_RETURN_VAL_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class return_val_node: public cdk::expression_node {
    cdk::expression_node* _value;

  public:
    return_val_node(int lineno, cdk::expression_node* value) :
        cdk::expression_node(lineno), _value(value) {
    }

  public:
    cdk::expression_node* value() {
      return _value;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_val_node(this, level);
    }

  };

} // m19

#endif
