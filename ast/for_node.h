// $Id: for_node.h,v 1.4 2019/03/21 19:00:02 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_FOR_NODE_H__
#define __M19_FOR_NODE_H__

#include <cdk/ast/expression_node.h>
// include sequence?

namespace m19 {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    cdk::sequence_node *_init;
    cdk::sequence_node *_condition;
    cdk::sequence_node *_after;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::sequence_node *init, cdk::sequence_node *condition, cdk::sequence_node *after, cdk::basic_node *block) :
        basic_node(lineno), _init(init), _condition(condition), _after(after), _block(block) {
    }

  public:
    inline cdk::sequence_node *init() {
      return _init;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *after() {
      return _after;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // m19

#endif
