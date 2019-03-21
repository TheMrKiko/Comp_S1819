// $Id: program_node.h,v 1.2 2019/03/21 19:00:03 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_PROGRAM_NODE_H__
#define __M19_PROGRAM_NODE_H__

#include <cdk/ast/basic_node.h>

namespace m19 {

  /**
   * Class for describing program nodes.
   */
  class program_node: public cdk::basic_node {
    cdk::basic_node *_statements;

  public:
    inline program_node(int lineno, cdk::basic_node *statements) :
        cdk::basic_node(lineno), _statements(statements) {
    }

  public:
    inline cdk::basic_node *statements() {
      return _statements;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_program_node(this, level);
    }

  };

} // m19

#endif
