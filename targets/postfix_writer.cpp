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

void m19::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) { //DONE
  //ASSERT_SAFE_EXPRESSIONS;  ?
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void m19::postfix_writer::do_continue_node(m19::continue_node * const node, int lvl) { //DONE
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forStep.top())); // jump to next cycle
  } else
    error(node->lineno(), "'continue' outside 'for'");
}

void m19::postfix_writer::do_break_node(m19::break_node * const node, int lvl) { //DONE
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forEnd.top())); // jump to for end
  } else
    error(node->lineno(), "'break' outside 'for'");
}

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

void m19::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void m19::postfix_writer::do_id_node(m19::id_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  //do nothing
}

void m19::postfix_writer::do_ref_node(m19::ref_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl); // it's enough!
}

void m19::postfix_writer::do_alloc_node(m19::alloc_node * const node, int lvl) { //DONE careful with local offset??
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(node->type()->subtype()->size());
  _pf.MUL();
  _pf.ALLOC(); // allocate
  _pf.SP();
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
  if (id == "@") {
    _pf.LOCAL(-4);
  }
  else if (symbol->global()) {
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

void m19::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void m19::postfix_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {  //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8);
  } else if (node->argument()->type()->name() == basic_type::TYPE_INT
          || node->argument()->type()->name() == basic_type::TYPE_POINTER
          || node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4);
  } else if (node->argument()->type()->name() != basic_type::TYPE_VOID) {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void m19::postfix_writer::do_print_node(m19::print_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if (node->newline()) _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_read_node(m19::read_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  if (node->type()->name() == basic_type::TYPE_INT) {
    _functions_to_declare.insert("readd"); //TAKE THIS OUT ?
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else if (node->type()->name() == basic_type::TYPE_INT) {
    _functions_to_declare.insert("readi"); //THIS TOO
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else {
    std::cerr << "FATAL: " << node->lineno() << ": cannot read type" << std::endl;
    return;
  }
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
  if (node->init()) node->init()->accept(this, lvl + 2);

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

void m19::postfix_writer::do_index_node(m19::index_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

void m19::postfix_writer::do_block_node(m19::block_node * const node, int lvl) { //DONE probably
  bool inInitSection = _inInitSection;
  _inInitSection = false;

  if (!inInitSection) _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  if (!inInitSection) _symtab.pop();  
}

void m19::postfix_writer::do_return_node(m19::return_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;  
  if (!_inFinalSection) {
    _pf.JMP(mklbl(_finalSectionLbl));
  } else {
    _pf.JMP(mklbl(_functionEndLbl));
  }
}

void m19::postfix_writer::do_fun_init_section_node(m19::fun_init_section_node * const node, int lvl) { //DONE
  _inInitSection = true;
  node->block()->accept(this, lvl);
  _inInitSection = false; //to be sure
}

void m19::postfix_writer::do_fun_final_section_node(m19::fun_final_section_node * const node, int lvl) { //DONE
  _inFinalSection = true;
  node->block()->accept(this, lvl);
  _inFinalSection = false;
}

void m19::postfix_writer::do_fun_section_node(m19::fun_section_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  int end;
  if (node->condition()) 
    node->condition()->accept(this, lvl);
  else 
    _pf.INT(1);
  _pf.JZ(mklbl(end = ++_lbl));
  node->block()->accept(this, lvl + 2);
  if (node->exclusive())
    _pf.JMP(mklbl(_finalSectionLbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(end));
}

void m19::postfix_writer::do_fun_body_node(m19::fun_body_node * const node, int lvl) { //DONE
  //ASSERT_SAFE_EXPRESSIONS;  
  _finalSectionLbl = ++_lbl;
  _functionEndLbl = ++_lbl;
  if (node->initial_section()) node->initial_section()->accept(this, lvl);
  if (node->sections()) node->sections()->accept(this, lvl);
  _pf.ALIGN();
  _pf.LABEL(mklbl(_finalSectionLbl));
  if (node->final_section()) node->final_section()->accept(this, lvl);
  _pf.LABEL(mklbl(_functionEndLbl));
}

void m19::postfix_writer::do_return_val_node(m19::return_val_node * const node, int lvl) {
  //APAGAR
}

void m19::postfix_writer::do_fun_call_node(m19::fun_call_node * const node, int lvl) {
  //FIXME
}

void m19::postfix_writer::do_var_decl_node(m19::var_decl_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;

  std::string id = node->identifier();
  // type size?
  int offset = 0, typesize = node->varType()->size();// in bytes
  //std::cout << "ARG: " << id << ", " << typesize << std::endl;
  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
    offset = 0; // global variable
  }

  std::shared_ptr<m19::symbol> symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (node->qualifier() == 2) {
    _pf.EXTERN(id);
  }

  else if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->varType()->name() == basic_type::TYPE_INT || node->varType()->name() == basic_type::TYPE_STRING
          || node->varType()->name() == basic_type::TYPE_POINTER) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->varType()->name() == basic_type::TYPE_DOUBLE) {
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } 
  
  else {
    if (!_function) { //necessario?
      if (node->initializer() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        if (node->qualifier() == 1) _pf.GLOBAL(id, _pf.OBJ());
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {

        if (node->varType()->name() == basic_type::TYPE_INT || node->varType()->name() == basic_type::TYPE_DOUBLE
            || node->varType()->name() == basic_type::TYPE_POINTER || node->varType()->name() == basic_type::TYPE_STRING) {

          _pf.DATA();
          _pf.ALIGN();
          if (node->qualifier() == 1) _pf.GLOBAL(id, _pf.OBJ());          
          _pf.LABEL(id);

          if (node->varType()->name() == basic_type::TYPE_INT) {
            node->initializer()->accept(this, lvl);
          } else if (node->varType()->name() == basic_type::TYPE_POINTER) {
            node->initializer()->accept(this, lvl);
          } else if (node->varType()->name() == basic_type::TYPE_DOUBLE) {
            if (node->initializer()->type()->name() == basic_type::TYPE_DOUBLE) {
              node->initializer()->accept(this, lvl);
            } else if (node->initializer()->type()->name() == basic_type::TYPE_INT) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->initializer());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
              _errors = true;
            }
          } else if (node->varType()->name() == basic_type::TYPE_STRING) {
            node->initializer()->accept(this, lvl);
          }
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
          _errors = true;
        }

      }

    }
  }
}

void m19::postfix_writer::do_fun_decl_node(m19::fun_decl_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot declare function in body or in args");
    return;
  }

  if (!new_symbol()) return;

  std::shared_ptr<m19::symbol> function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();
}

void m19::postfix_writer::do_fun_def_node(m19::fun_def_node * const node, int lvl) { //DONE
  ASSERT_SAFE_EXPRESSIONS;
  
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  // remember symbol so that args and body know
  _function = new_symbol();
  _functions_to_declare.erase(_function->name());  // just in case
  reset_new_symbol();

  _offset = 8; // prepare for arguments (4: remember to account for return address)
  _symtab.push(); // scope of args
  if (node->arguments()) {
    _inFunctionArgs = true; //FIXME really needed?
    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
      cdk::basic_node *arg = node->arguments()->node(ix);
      if (arg == nullptr) break; // this means an empty sequence of arguments
      arg->accept(this, 0); // the function symbol is at the top of the stack
    }
    _inFunctionArgs = false; //FIXME really needed?
  }

  _pf.TEXT();
  _pf.ALIGN();
  if (node->qualifier() == 1) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  // the following flag is a slight hack: it won't work with nested functions
  _inFunctionBody = true;
  // prepare for local variables but remember the return value (first below fp)
  _offset = -_function->type()->size();
  os() << "        ;; before body " << std::endl;
  node->body()->accept(this, lvl + 2);
  os() << "        ;; after body " << std::endl;
  _inFunctionBody = false;
  _symtab.pop(); // scope of arguments

  _pf.LOCAL(-4);
  if (_function->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
    _pf.STFVAL64();
  } else if (_function->type()->name() != basic_type::TYPE_VOID) {
    _pf.LDINT();
    _pf.STFVAL32();
  }

  _pf.LEAVE();
  _pf.RET();

  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");

  if (node->identifier() == "m19") {
    // declare external functions
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
  _function = nullptr;
}
/*

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}
*/