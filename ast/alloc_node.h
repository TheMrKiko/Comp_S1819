#ifndef __M19_ALLOC_NODE_H__
#define __M19_ALLOC_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace m19 {

  class alloc_node: public cdk::unary_expression_node {
  public:
    alloc_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_alloc_node(this, level);
    }

  };

} // m19

#endif
