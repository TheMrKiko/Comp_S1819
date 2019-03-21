// $Id: read_node.h,v 1.2 2019/03/21 19:00:03 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_READ_NODE_H__
#define __M19_READ_NODE_H__

#include <cdk/ast/lvalue_node.h>

namespace m19 {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::basic_node {
    cdk::lvalue_node *_argument;

  public:
    inline read_node(int lineno, cdk::lvalue_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // m19

#endif
