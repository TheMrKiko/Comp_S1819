#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void m19::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) { //DONE
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
} 

void m19::type_checker::do_continue_node(m19::continue_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_break_node(m19::break_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void m19::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void m19::type_checker::do_not_node(cdk::not_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}

void m19::type_checker::do_BooleanLogicalExpression(cdk::binary_expression_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT) 
    throw std::string("integer expression expected in binary expression");
  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("integer expression expected in binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_and_node(cdk::and_node * const node, int lvl) { //DONE
  do_BooleanLogicalExpression(node, lvl);  
}
void m19::type_checker::do_or_node(cdk::or_node * const node, int lvl) { //DONE
  do_BooleanLogicalExpression(node, lvl);
}
//---------------------------------------------------------------------------

void m19::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_double_node(cdk::double_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
}

void m19::type_checker::do_string_node(cdk::string_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

void m19::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) { //DONE
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT && node->argument()->type()->name() != basic_type::TYPE_DOUBLE) 
    throw std::string("wrong type in argument of unary expression");

  node->type(new basic_type(node->argument()->type()->size(), node->argument()->type()->name()));
}

void m19::type_checker::do_id_node(m19::id_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  processUnaryExpression(node, lvl);
}

void m19::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  processUnaryExpression(node, lvl);
}

void m19::type_checker::do_ref_node(m19::ref_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() == basic_type::TYPE_VOID) {
    throw std::string("wrong type in unary logical expression");
  } else {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(node->lvalue()->type()->size(), node->lvalue()->type()->name());
  }
}

void m19::type_checker::do_alloc_node(m19::alloc_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in unary logical expression");
  } else {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(0, basic_type::TYPE_VOID);
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_add_node(cdk::add_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));

  } else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    node->left()->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    node->right()->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else {
    throw std::string("wrong type in one of arguments of binary expression");

  }
}

void m19::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) { //DONE p - p doesnt check all the way
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));

  } else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER &&
    node->left()->type()->subtype()->name() == node->right()->type()->subtype()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    node->left()->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    node->right()->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else {
    throw std::string("wrong type in one of arguments of binary expression");

  }
}

void m19::type_checker::process_ID_Expression(cdk::binary_expression_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));

  } else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));

  } else {
    throw std::string("wrong type in one of arguments of binary expression");

  }
}

void m19::type_checker::process_I_Expression(cdk::binary_expression_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));

  } else {
    throw std::string("wrong type in one of arguments of binary expression");

  }
}  

void m19::type_checker::process_ID_ScalarExpression(cdk::binary_expression_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if ((node->left()->type()->name() == basic_type::TYPE_INT || node->left()->type()->name() == basic_type::TYPE_DOUBLE) && 
      (node->right()->type()->name() == basic_type::TYPE_INT || node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong type in one of arguments of binary expression");
  }
} 

void m19::type_checker::process_IDP_LogicalExpression(cdk::binary_expression_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if ((node->left()->type()->name() == basic_type::TYPE_INT  || node->left()->type()->name() == basic_type::TYPE_DOUBLE) && 
      (node->right()->type()->name() == basic_type::TYPE_INT || node->right()->type()->name() == basic_type::TYPE_DOUBLE)) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if ((node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER) ||
             (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) ||
             (node->left()->type()->name() == basic_type::TYPE_INT     && node->right()->type()->name() == basic_type::TYPE_POINTER)) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong type in one of arguments of binary expression");
  }
} 

void m19::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) { //DONE
  process_ID_Expression(node, lvl);
}
void m19::type_checker::do_div_node(cdk::div_node * const node, int lvl) { //DONE
  process_ID_Expression(node, lvl);;
}
void m19::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) { //DONE
  process_I_Expression(node, lvl);
}
void m19::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) { //DONE
  process_ID_ScalarExpression(node, lvl);
}
void m19::type_checker::do_le_node(cdk::le_node * const node, int lvl) { //DONE
  process_ID_ScalarExpression(node, lvl);
}
void m19::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) { //DONE
  process_ID_ScalarExpression(node, lvl);
}
void m19::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) { //DONE
  process_ID_ScalarExpression(node, lvl);
}
void m19::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) { //DONE
  process_IDP_LogicalExpression(node, lvl);
}
void m19::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) { //DONE
  process_IDP_LogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_variable_node(cdk::variable_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else if (node->name() == "@") {
    if (_function) 
      node->type(_function->type());
    else
      throw std::string("wrong use of @ (not in function)");      
  } else {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void m19::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(node->lvalue()->type());
}

void m19::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) { //DONE basically
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);

  basic_type *ltype = node->lvalue()->type();
  basic_type *rtype = node->rvalue()->type();

  cdk::integer_node *lit;

  if (node->lvalue()->type()->name() == basic_type::TYPE_INT) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    } else
      throw std::string("wrong assignment to integer");
  } 

  else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE) { 
    if (node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE || node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else
      throw std::string("wrong assignment to real");
  }

  else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_STRING) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
    /*} else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) { //NAO!?
      node->type(new basic_type(4, basic_type::TYPE_STRING));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_STRING));*/
    } else
      throw std::string("wrong assignment to string");
  } 
  
  else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER) {

    /*verifies pointer level*/
    while (ltype->subtype() != nullptr && rtype->subtype() != nullptr) {
      ltype = ltype->subtype();
      rtype = rtype->subtype();
    }
    if (ltype->name() != rtype->name()) {
      throw std::string("wrong types in assignment");
    }

    //FIXME tipos não vão fundo

    if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER &&
        node->lvalue()->type()->subtype()->name() == node->rvalue()->type()->subtype()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type()->_subtype = new basic_type(node->lvalue()->type()->subtype()->size(), 
                                              node->lvalue()->type()->subtype()->name());
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER &&
        node->rvalue()->type()->subtype()->name() == basic_type::TYPE_VOID) {
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type()->_subtype = new basic_type(node->lvalue()->type()->subtype()->size(), 
                                              node->lvalue()->type()->subtype()->name());
      node->rvalue()->type()->_subtype = new basic_type(node->lvalue()->type()->subtype()->size(), 
                                                        node->lvalue()->type()->subtype()->name());
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      lit = dynamic_cast<cdk::integer_node*>(node->rvalue());
      if (lit != nullptr && lit->value() == 0) {
        node->type(new basic_type(4, basic_type::TYPE_POINTER));
        node->type()->_subtype = new basic_type(node->lvalue()->type()->subtype()->size(), 
                                                node->lvalue()->type()->subtype()->name());
      } else
        throw std::string("wrong assignment to pointer");
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) { //necessary?
      node->type(new basic_type(4, basic_type::TYPE_ERROR));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_ERROR));
    } else 
      throw std::string("wrong assignment to pointer");
  }
  
  else {
    throw std::string("wrong types in assignment");
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_evaluation_node(m19::evaluation_node * const node, int lvl) { //DONE
  node->argument()->accept(this, lvl + 2);
}

void m19::type_checker::do_print_node(m19::print_node * const node, int lvl) { //DONE
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_VOID)
    throw std::string("wrong type in print argument");
}

//---------------------------------------------------------------------------

void m19::type_checker::do_read_node(m19::read_node * const node, int lvl) { //DONE
  node->type(new basic_type(0, basic_type::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void m19::type_checker::do_for_node(m19::for_node * const node, int lvl) { //DONE
  /*node->init()->accept(this, lvl + 2);
  node->condition()->accept(this, lvl + 2);
  node->after()->accept(this, lvl + 2);
  node->block()->accept(this, lvl + 2);*/
}

//---------------------------------------------------------------------------

void m19::type_checker::do_if_node(m19::if_node * const node, int lvl) { //DONE
  node->condition()->accept(this, lvl + 2);
  if (node->condition()->type()->name() != basic_type::TYPE_INT && 
      node->condition()->type()->name() != basic_type::TYPE_DOUBLE) throw std::string("expected integer condition");
  node->block()->accept(this, lvl + 2);
}

void m19::type_checker::do_if_else_node(m19::if_else_node * const node, int lvl) { //DONE
  node->condition()->accept(this, lvl + 2);
  if (node->condition()->type()->name() != basic_type::TYPE_INT && 
      node->condition()->type()->name() != basic_type::TYPE_DOUBLE) throw std::string("expected integer condition");
  node->thenblock()->accept(this, lvl + 2);
  node->elseblock()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_index_node(m19::index_node * const node, int lvl) { //DONE
  ASSERT_UNSPEC;
  node->base()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);
  if (node->base()->type()->name() == basic_type::TYPE_POINTER &&
    node->index()->type()->name() == basic_type::TYPE_INT)
    node->type(node->base()->type()->subtype());
  else if (node->index()->type()->name() == basic_type::TYPE_POINTER &&
    node->base()->type()->name() == basic_type::TYPE_INT)
    node->type(node->index()->type()->subtype());
  else 
    throw std::string("wrong types in index");
}

void m19::type_checker::do_block_node(m19::block_node * const node, int lvl) { //DONE
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
}

void m19::type_checker::do_return_node(m19::return_node * const node, int lvl) { //DONE
  if (!_function) throw std::string("return outside of function");
}

void m19::type_checker::do_fun_init_section_node(m19::fun_init_section_node * const node, int lvl) { //DONE
  node->block()->accept(this, lvl);
}

void m19::type_checker::do_fun_final_section_node(m19::fun_final_section_node * const node, int lvl) { //DONE
  node->block()->accept(this, lvl);
}

void m19::type_checker::do_fun_section_node(m19::fun_section_node * const node, int lvl) { //DONE
  if(node->condition()) node->condition()->accept(this, lvl);
  //node->block()->accept(this, lvl);  
}

void m19::type_checker::do_fun_body_node(m19::fun_body_node * const node, int lvl) { //DONE
  /*if (node->initial_section()) node->initial_section()->accept(this, lvl);
  if (node->sections())     node->sections()->accept(this, lvl);
  if (node->final_section())   node->final_section()->accept(this, lvl);*/
}

void m19::type_checker::do_return_val_node(m19::return_val_node * const node, int lvl) {
  //APAGAR 
}

void m19::type_checker::do_fun_call_node(m19::fun_call_node * const node, int lvl) { //verificar argumentos??
  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) {
    if (id == "@" && _function)
      symbol = _function;
    else
      throw std::string("symbol '" + id + "' is undeclared.");
  }

  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  if (node->arguments()) {
    if (node->arguments()->size() != _function->args()->size())
      throw std::string("bad arguments");

    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {

      cdk::typed_node *valarg = dynamic_cast<cdk::typed_node*>(node->arguments()->node(ix));
      std::shared_ptr<m19::symbol> funarg = symbol->args()->at(ix);

      valarg->accept(this, lvl); // the function symbol is at the top of the stack

      basic_type *vartype = funarg->type();
      basic_type *initype = valarg->type(); 

      cdk::integer_node *lit;

      while (vartype->subtype() != nullptr && initype->subtype() != nullptr) {
        vartype = vartype->subtype();
        initype = initype->subtype();
      }

      if (vartype->name() == basic_type::TYPE_INT) {
        if (initype->name() != basic_type::TYPE_INT) throw std::string(
            "wrong type for initializer (integer expected).");
      } else if (vartype->name() == basic_type::TYPE_DOUBLE) {
        if (initype->name() != basic_type::TYPE_INT
            && initype->name() != basic_type::TYPE_DOUBLE) throw std::string(
            "wrong type for initializer (integer or double expected).");
      } else if (vartype->name() == basic_type::TYPE_STRING) {
        if (initype->name() != basic_type::TYPE_STRING) throw std::string(
            "wrong type for initializer (string expected).");
      } else if (vartype->name() == basic_type::TYPE_POINTER) {
        if (initype->name() == basic_type::TYPE_INT) {
          lit = dynamic_cast<cdk::integer_node*>(valarg);
          if (lit == nullptr || lit->value() != 0) {
            throw std::string("wrong type for initializer (pointer expected).");        
          }
        } else if (initype->name() != basic_type::TYPE_POINTER) throw std::string(
            "wrong type for initializer (pointer expected).");
      } else {
        throw std::string("unknown type for initializer.");
      }
      
    }
  }
}

void m19::type_checker::do_var_decl_node(m19::var_decl_node * const node, int lvl) { //DONE
  if (node->initializer() != nullptr) {  //falta ver se ta inArgs
    node->initializer()->accept(this, lvl + 2);

    basic_type *vartype = node->varType();
    basic_type *initype = node->initializer()->type(); 

    cdk::integer_node *lit;

    while (vartype->subtype() != nullptr && initype->subtype() != nullptr) {
      vartype = vartype->subtype();
      initype = initype->subtype();
    }

    if (vartype->name() == basic_type::TYPE_INT) {
      if (initype->name() != basic_type::TYPE_INT) throw std::string(
          "wrong type for initializer (integer expected).");
    } else if (vartype->name() == basic_type::TYPE_DOUBLE) {
      if (initype->name() != basic_type::TYPE_INT
          && initype->name() != basic_type::TYPE_DOUBLE) throw std::string(
          "wrong type for initializer (integer or double expected).");
    } else if (vartype->name() == basic_type::TYPE_STRING) {
      if (initype->name() != basic_type::TYPE_STRING) throw std::string(
          "wrong type for initializer (string expected).");
    } else if (vartype->name() == basic_type::TYPE_POINTER) {
      if (initype->name() == basic_type::TYPE_INT) {
        lit = dynamic_cast<cdk::integer_node*>(node->initializer());
        if (lit == nullptr || lit->value() != 0) {
          throw std::string("wrong type for initializer (pointer expected).");        
        }
      } else if (initype->name() != basic_type::TYPE_POINTER) throw std::string(
          "wrong type for initializer (pointer expected).");
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = std::make_shared < m19::symbol > (
  node->qualifier(), // qualifiers: public, forward, "private" (i.e., none)
  node->varType(), // type (type id + type size)
  id, // identifier
  (bool)node->initializer(), // initialized?
  false); // is it a function?
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }

  if (_inFunArgs)
    _function->args()->insert(_function->args()->end(), symbol);
}

void m19::type_checker::do_fun_decl_node(m19::fun_decl_node * const node, int lvl) { //verificar argumentos?? e lit
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "m19")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  std::shared_ptr<m19::symbol> function = std::make_shared < m19::symbol
      > (node->qualifier(), node->type(), id, false, true, true);

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (previous) {//faltaverargs
    if (previous->qualifier() != node->qualifier() || previous->type() != node->type()) {
      throw std::string("conflicting declaration for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }

  _function = function;
  _inFunArgs = true;

  if (node->arguments()) {
    _symtab.push();
    node->arguments()->accept(this, lvl + 2);
    _symtab.pop();
  }

  _inFunArgs = false;

  _function = nullptr;

}

void m19::type_checker::do_fun_def_node(m19::fun_def_node * const node, int lvl) { //verificar argumentos?? e literal; verificar qualificadores
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "m19")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  std::shared_ptr<m19::symbol> function = std::make_shared < m19::symbol
      > (node->qualifier(), node->type(), id, false, true);
  function->set_offset(-node->type()->size());

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward() 
        && (previous->qualifier() == node->qualifier())
        && (previous->type()->name() == node->type()->name())) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }

  if (node->literal() != nullptr) {
    if (node->type()->name() == basic_type::TYPE_INT) {
      cdk::integer_node *lit = dynamic_cast<cdk::integer_node*>(node->literal());
      if (lit == nullptr) throw std::string("wrong type for literal");
    } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
      cdk::double_node *lit = dynamic_cast<cdk::double_node*>(node->literal());
      if (lit == nullptr) throw std::string("wrong type for literal");
    } else if (node->type()->name() == basic_type::TYPE_POINTER) {
      cdk::integer_node *lit = dynamic_cast<cdk::integer_node*>(node->literal());
      if (lit == nullptr) throw std::string("wrong type for literal");
      else if (lit->value() != 0) throw std::string("pointer literal must be 0 (zero)");
    } else if (node->type()->name() == basic_type::TYPE_STRING) {
      cdk::string_node *lit = dynamic_cast<cdk::string_node*>(node->literal());
      if (lit == nullptr) throw std::string("wrong type for literal");
    } else if (node->type()->name() == basic_type::TYPE_VOID) {
      throw std::string("void type function with initial value");
    }
  }
  //_function = function;
  //_inFunArgs = true;

  //_symtab.push();
  //node->arguments()->accept(this, lvl + 2);
  //node->body()->accept(this, lvl + 2);
  //_symtab.pop();

  //_inFunArgs = false;
  //_function = nullptr;
}

