#ifndef __M19_ID_NODE_H__
#define __M19_ID_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace m19 {

  class id_node: public cdk::unary_expression_node {
  public:
    id_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_id_node(this, level);
    }

  };

} // m19

#endif
