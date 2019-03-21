#ifndef __M19_REF_NODE_H__
#define __M19_REF_NODE_H__

#include <cdk/ast/unary_expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class ref_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;

  public:
    ref_node(int lineno, cdk::lvalue_node *argument) :
        cdk::expression_node(lineno), _lvalue(argument) {
    }

  public:
    cdk::lvalue_node *lvalue() {
      return _lvalue;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_ref_node(this, level);
    }

  };

} // m19

#endif
