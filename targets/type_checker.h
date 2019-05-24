// $Id: type_checker.h,v 1.6 2019/05/24 13:19:57 ist186416 Exp $ -*- c++ -*-
#ifndef __M19_SEMANTICS_TYPE_CHECKER_H__
#define __M19_SEMANTICS_TYPE_CHECKER_H__

#include <string>
#include <vector>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include "targets/symbol.h"
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<m19::symbol> &_symtab;
    std::shared_ptr<m19::symbol> _function;
    basic_ast_visitor *_parent;

    // semantics analysis
    bool _inInitSection = false;
    bool _inFunArgs = false;



  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<m19::symbol> &symtab, std::shared_ptr<m19::symbol> func, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _function(func), _parent(parent) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    void processUnaryExpression(cdk::unary_expression_node * const node, int lvl);
    void processBinaryExpression(cdk::binary_expression_node * const node, int lvl);
    void process_ID_Expression(cdk::binary_expression_node * const node, int lvl);
    void process_I_Expression(cdk::binary_expression_node * const node, int lvl);
    void process_ID_ScalarExpression(cdk::binary_expression_node * const node, int lvl);
    void process_IDP_LogicalExpression(cdk::binary_expression_node * const node, int lvl);
    void do_BooleanLogicalExpression(cdk::binary_expression_node * const node, int lvl);
    template<typename T>
    void process_literal(cdk::literal_node<T> * const node, int lvl) {
    }
    

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // m19

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, function, node) { \
  try { \
    m19::type_checker checker(compiler, symtab, function, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, _function, node)

#endif
