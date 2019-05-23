#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void m19::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void m19::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void m19::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}
void m19::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void m19::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;  ?
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void m19::postfix_writer::do_continue_node(m19::continue_node * const node, int lvl) {
  // FIXME
}

void m19::postfix_writer::do_break_node(m19::break_node * const node, int lvl) {
  // FIXME
}

/*void m19::postfix_writer::do_expressions_node(m19::expressions_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}*/

//---------------------------------------------------------------------------

void m19::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) { //DONE
  if (_inFunctionBody) {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void m19::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) { //DONE
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value()); // load number to the stack
  } else {
    _pf.SDOUBLE(node->value()); // double is on the DATA segment
  }
}

void m19::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) { //DONE
  int lbl1;

  /* generate the string literal */
  _pf.RODATA(); // strings are readonly DATA
  _pf.ALIGN(); // make sure the address is aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters
  if (_inFunctionBody) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void m19::postfix_writer::do_ref_node(m19::ref_node * const node, int lvl) {
  /*ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement*/ //FIXME
}


void m19::postfix_writer::do_alloc_node(m19::alloc_node * const node, int lvl) {
  /*ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement*/ //FIXME
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == basic_type::TYPE_POINTER && node->left()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT(node->right()->type()->subtype()->size());
    _pf.MUL();
    //_pf.SHTL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT(node->left()->type()->subtype()->size());
    _pf.MUL();
  }

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
  _pf.DADD();
  else
  _pf.ADD();
}

void m19::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT(node->left()->type()->subtype()->size());
    _pf.MUL();
  }

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
  _pf.DSUB();
  else
  _pf.SUB();
}
void m19::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
  _pf.I2D();

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
  _pf.DMUL();
  else
  _pf.MUL();
}
void m19::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}
void m19::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void m19::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LT();
}
void m19::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LE();
}
void m19::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GE();
}
void m19::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GT();
}
void m19::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.NE();
}
void m19::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  }
  else {
    _pf.LOCAL(symbol->offset());
  }
}

void m19::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  }
  else { // integers, pointers, and strings
    _pf.LDINT();
  }
}

void m19::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    reset_new_symbol();
    _pf.SINT(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STINT(); // store the value at address
}

//---------------------------------------------------------------------------
/*
void m19::postfix_writer::do_program_node(m19::program_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // Simple doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}
*/
//---------------------------------------------------------------------------

void m19::postfix_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void m19::postfix_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_read_node(m19::read_node * const node, int lvl) {
  /*ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  node->argument()->accept(this, lvl);
  _pf.STINT();*/
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_for_node(m19::for_node * const node, int lvl) { //DONE probably
  ASSERT_SAFE_EXPRESSIONS;
  _forIni.push(++_lbl); // after init, before body
  _forStep.push(++_lbl);// after intruction
  _forEnd.push(++_lbl);// after for

  os() << "        ;; FOR initialize" << std::endl;
  // initialize: be careful with variable declarations:
  // they are done here, but the space is occupied in the function
  _inForInit = true;// remember this for local declarations

  // initialize
  node->init()->accept(this, lvl + 2);

  os() << "        ;; FOR test" << std::endl;
  // prepare to test
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIni.top()));
  node->condition()->accept(this, lvl + 2);
  _pf.JZ(mklbl(_forEnd.top()));

  os() << "        ;; FOR block" << std::endl;
  node->block()->accept(this, lvl + 2);

  os() << "        ;; FOR step" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forStep.top()));
  node->after()->accept(this, lvl + 2);
  _pf.JMP(mklbl(_forIni.top()));

  os() << "        ;; FOR end" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forEnd.top()));

  _inForInit = false;// remember this for local declarations

  _forIni.pop();
  _forStep.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_node(m19::if_node * const node, int lvl) {  //DONE ???
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  //if double : D2I
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_else_node(m19::if_else_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_else, lbl_end;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_else = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl_end = ++_lbl));
  _pf.LABEL(mklbl(lbl_else));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl_end));
  
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_index_node(m19::index_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_block_node(m19::block_node * const node, int lvl) { //DONE probably
  _symtab.push(); // for block-local vars
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void m19::postfix_writer::do_return_node(m19::return_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_init_section_node(m19::fun_init_section_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_final_section_node(m19::fun_final_section_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_section_node(m19::fun_section_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_body_node(m19::fun_body_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_return_val_node(m19::return_val_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_call_node(m19::fun_call_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_var_decl_node(m19::var_decl_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_decl_node(m19::fun_decl_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_fun_def_node(m19::fun_def_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_id_node(m19::id_node * const node, int lvl) {
  //FIXME
}