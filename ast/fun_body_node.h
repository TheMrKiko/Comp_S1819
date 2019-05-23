#ifndef __M19_FUN_BODY_NODE_H__
#define __M19_FUN_BODY_NODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  class fun_body_node: public cdk::basic_node {

    m19::fun_init_section_node*  _initial_section;
    cdk::sequence_node*          _sections;
    m19::fun_final_section_node* _final_section;

  public:
    inline fun_body_node(int lineno, m19::fun_init_section_node* initial_section, cdk::sequence_node* sections, m19::fun_final_section_node* final_section) :
        cdk::basic_node(lineno), _initial_section(initial_section), _sections(sections), _final_section(final_section) {
    }

  public:
    inline m19::fun_init_section_node* initial_section() {
      return _initial_section;
    }
    inline cdk::sequence_node* sections() {
      return _sections;
    }
    inline m19::fun_final_section_node* final_section() {
      return _final_section;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_body_node(this, level);
    }

  };

} // m19

#endif
