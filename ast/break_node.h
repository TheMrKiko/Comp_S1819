#ifndef __M19_BREAK_NODE_H__
#define __M19_BREAK_NODE_H__

#include <cdk/ast/basic_node.h>

namespace m19 {

  class break_node: public cdk::basic_node {
    int _level;

  public:
    break_node(int lineno, int level = 1) :
        cdk::basic_node(lineno), _level(level) {
    }

  public:
    int level() const {
      return _level;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // m19

#endif
