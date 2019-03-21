#ifndef __M19_FUN_CALL_H__
#define __M19_FUN_CALL_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/nil_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class fun_call_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    fun_call_node(int lineno, const std::string &identifier) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)) {
    }

    fun_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }

  public:
    const std::string &identifier() {
      return _identifier;
    }
    cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_fun_call_node(this, level);
    }

  };

} // m19

#endif
