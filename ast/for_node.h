// $Id: for_node.h,v 1.1 2019/03/19 12:49:31 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_FORNODE_H__
#define __M19_FORNODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    cdk::expression_node *_init;
    cdk::expression_node *_condition;
    cdk::expression_node *_after;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::expression_node *init, cdk::expression_node *condition, cdk::expression_node *after, cdk::basic_node *block) :
        basic_node(lineno), _init(init), _condition(condition), _after(after), _block(block) {
    }

  public:
    inline cdk::expression_node *init() {
      return _init;
    }
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::expression_node *after() {
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
