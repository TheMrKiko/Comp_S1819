#ifndef __M19_VAR_DECL_NODE_H__
#define __M19_VAR_DECL_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace m19 {

  class var_decl_node: public cdk::basic_node {
    int _qualifier;
    basic_type *_varType;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    var_decl_node(int lineno, int qualifier, basic_type* varType, const std::string &identifier, cdk::expression_node* initializer) :
        cdk::basic_node(lineno), _qualifier(qualifier), _varType(varType), _identifier(identifier), _initializer(initializer) {
    }

  public:
    int qualifier() {
      return _qualifier;
    }
    basic_type* varType() {
      return _varType;
    }
    const std::string &identifier() const {
      return _identifier;
    }
    cdk::expression_node* initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_decl_node(this, level);
    }

  };

} // m19

#endif
