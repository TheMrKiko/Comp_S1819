#ifndef __M19_FUN_DEF_NODE_H__
#define __M19_FUN_DEF_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/block_node.h"

namespace m19 {

  class fun_def_node: public cdk::basic_node {
    basic_type *_type;
    std::string _identifier;
    int _qualifier;
    m19::fun_body_node *_body;
    cdk::expression_node *_literal;
    cdk::sequence_node *_arguments;

  public:
    fun_def_node(int lineno, basic_type *type, const std::string &identifier, int qualifier, m19::fun_body_node *body, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(qualifier), _body(body), _literal(nullptr), _arguments(arguments) {
    }

    fun_def_node(int lineno, basic_type *type, const std::string &identifier, int qualifier, m19::fun_body_node *body, cdk::expression_node *literal, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(qualifier), _body(body), _literal(literal), _arguments(arguments) {
    }

  public:
    int qualifier() {
      return _qualifier;
    }
    basic_type *type() {
      return _type;
    }
    const std::string &identifier() const {
      return _identifier;
    }
    cdk::sequence_node *arguments() {
      return _arguments;
    }
    m19::fun_body_node *body() {
      return _body;
    }

    cdk::expression_node *literal() {
      return _literal; 
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_def_node(this, level);
    }

  };

} // m19

#endif
