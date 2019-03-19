// $Id: for_node.h,v 1.2 2019/03/19 22:51:36 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_FORNODE_H__
#define __M19_FORNODE_H__

#include <cdk/ast/expression_node.h>
#include "ast/expressions_node.h"


namespace m19 {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    expressions_node *_init;
    expressions_node *_condition;
    expressions_node *_after;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, expressions_node *init, expressions_node *condition, expressions_node *after, cdk::basic_node *block) :
        basic_node(lineno), _init(init), _condition(condition), _after(after), _block(block) {
    }

  public:
    inline expressions_node *init() {
      return _init;
    }
    inline expressions_node *condition() {
      return _condition;
    }
    inline expressions_node *after() {
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
