// $Id: print_node.h,v 1.4 2019/05/14 14:32:25 ist186400 Exp $ -*- c++ -*-
#ifndef __M19_PRINT_NODE_H__
#define __M19_PRINT_NODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _newline;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool newline) :
        cdk::basic_node(lineno), _argument(argument), _newline(newline){
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }
    
    inline bool newline() {
      return _newline;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // m19

#endif
