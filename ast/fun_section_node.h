#ifndef __M19_SECTION_NODE_H__
#define __M19_SECTION_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"


namespace m19 {

  class fun_section_node: public cdk::basic_node {
    m19::block_node *_block;
    bool _exclusive;
    cdk::expression_node *_condition;

  public:
    fun_section_node(int lineno, m19::block_node *block, bool exclusive = false, cdk::expression_node *condition = nullptr) :
        cdk::basic_node(lineno), _block(block), _exclusive(exclusive), _condition(condition) {
    }

  public:
    m19::block_node *block() {
      return _block;
    }

    cdk::expression_node *condition() {
      return _condition;
    }

    bool exclusive() {
      return _exclusive;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_section_node(this, level);
    }

  };

} // m19

#endif
