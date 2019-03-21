// $Id: evaluation_node.h,v 1.2 2019/03/21 19:00:02 ist186400 Exp $
#ifndef __M19_EVALUATION_NODE_H__
#define __M19_EVALUATION_NODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  /**
   * Class for describing evaluation nodes.
   */
  class evaluation_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline evaluation_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_evaluation_node(this, level);
    }

  };

} // m19

#endif
