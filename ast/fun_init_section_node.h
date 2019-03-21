#ifndef __M19_INIT_SECTION_NODE_H__
#define __M19_INIT_SECTION_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"


namespace m19 {

  class fun_init_section_node: public cdk::basic_node {
    m19::block_node *_block;

  public:
    fun_init_section_node(int lineno, m19::block_node *block) :
        cdk::basic_node(lineno), _block(block) {
    }

  public:
    m19::block_node *block() {
      return _block;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_init_section_node(this, level);
    }

  };

} // m19

#endif
