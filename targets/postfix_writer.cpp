#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "ast/all.h"  // all.h is automatically generated
#include "fir_parser.tab.h"

//---------------------------------------------------------------------------

void fir::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void fir::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}

void fir::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void fir::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value());
  } else {
    _pf.SINT(node->value());
  }
}

void fir::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value());
  } else {
    _pf.SDOUBLE(node->value());
  }
}

void fir::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (_function) {
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == cdk::TYPE_POINTER && node->left()->type()->name() == cdk::TYPE_INT) {
    // TODO check thiiiis
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DADD();
  else
    _pf.ADD();
}

void fir::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DSUB();
  else
    _pf.SUB();
}

void fir::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DMUL();
  else
    _pf.MUL();
}

void fir::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}

void fir::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void fir::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  // TODO DMCP?
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}

void fir::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}

void fir::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}

void fir::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}

void fir::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}

void fir::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  auto symbol = _symtab.find(id);

  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  } else {
    _pf.LDINT();
  }
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl); // determine the new value

  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == cdk::TYPE_INT) _pf.I2D();
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  if (new_symbol() == nullptr) node->lvalue()->accept(this, lvl); // TODO not necessary? 

  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}


//---------------------------------------------------------------------------

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.TRASH(node->argument()->type()->size());
}

void fir::postfix_writer::do_print_node(fir::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));

    child->accept(this, lvl); // expression to print
    if (child->is_typed(cdk::TYPE_INT)) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // trash int
    } else if (child->is_typed(cdk::TYPE_DOUBLE)) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // trash double
    } else if (child->is_typed(cdk::TYPE_STRING)) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // trash char pointer
    } else {
      error(node->lineno(), "cannot print expression of unknown type");
    }
  }

  if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node * const node, int lvl) {
  /* ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  node->argument()->accept(this, lvl);
  _pf.STINT(); */
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _whileIni.push(++_lbl);
  _whileEnd.push(++_lbl);
  
  _pf.LABEL(mklbl(_whileIni.top()));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(_whileEnd.top()));

  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(_whileIni.top()));

  _pf.LABEL(mklbl(_whileEnd.top()));
  if (node->finally_block()) node->finally_block()->accept(this, lvl + 2);

  _whileIni.pop();
  _whileEnd.pop();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sizeof_node(fir::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->argument()->type()->size());
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  
  std::stack<int> aux;
  
  for (int i = 0; i < node->level() - 1; i++) {
    if (_whileIni.size() != 0) {
      aux.push(_whileEnd.top());
      _whileEnd.pop();
    } else {
      error(node->lineno(), "'leave' outside while");
    }
  }

  _pf.JMP(mklbl(_whileEnd.top())); // jump to while end

  for (int i = 0; i < node->level() - 1; i++) {
    _whileEnd.push(aux.top());
    aux.pop();
  }

  // TODO sketchy. Better way?
}

void fir::postfix_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  std::stack<int> aux;

  for (int i = 0; i < node->level() - 1; i++) {
    if (_whileIni.size() != 0) {
      aux.push(_whileIni.top());
      _whileIni.pop();
    } else {
      error(node->lineno(), "'leave' outside while");
    }
  }

  _pf.JMP(mklbl(_whileIni.top())); // jump to while end

  for (int i = 0; i < node->level() - 1; i++) {
    _whileIni.push(aux.top());
    aux.pop();
  }

  // TODO sketchy. Better way?
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_return_node(fir::return_node * const node, int lvl) {
  _pf.JMP(_currentBodyRetLabel);
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_block_node(fir::block_node * const node, int lvl) {
  if (!_prologue) _symtab.push(); // TODO what if inside while
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  if (!_prologue) _symtab.pop();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_declaration_node(fir::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();

  int offset = 0, typesize = node->type()->size(); // in bytes

  if (_inFunctionBody) {

    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {

    offset = _offset;
    _offset += typesize;
  } else {

    offset = 0; // global variable
  }

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (node->initializer()->is_typed(cdk::TYPE_INT))
          _pf.I2D();
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        error(node->lineno(), "cannot initialize");
      }
    }
  } else {
    if (!_function) {
      if (node->initializer() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {
        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);

          if (node->is_typed(cdk::TYPE_INT)) {
            node->initializer()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_POINTER)) {
            node->initializer()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            if (node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
              node->initializer()->accept(this, lvl);
            } else if (node->initializer()->is_typed(cdk::TYPE_INT)) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node*>(node->initializer());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
            }
          }
        } else if (node->is_typed(cdk::TYPE_STRING)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->initializer()->accept(this, lvl);
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
        }
      }
    }
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_function_declaration_node(fir::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody || _inFunctionArgs) { // FIXME really needed?
    error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  if (!new_symbol()) return;

  auto function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();
}

void fir::postfix_writer::do_function_definition_node(fir::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if (_inFunctionBody || _inFunctionArgs) { //FIXME really needed?
    error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  // remember symbol so that args and body know
  _function = new_symbol();
  _functions_to_declare.erase(_function->name()); // just in case
  reset_new_symbol();

  _currentBodyRetLabel = mklbl(++_lbl);

  _offset = 8;

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
  if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());


  int typesize = node->type()->size(); // in bytes


  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab, _function);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize() + typesize); // total stack size reserved for local variables

  _offset = 0; // prepare for local variable

  _inFunctionBody = true;

  // declare return variable
  _offset -= typesize;
  _function->set_offset(_offset);
  // initialize return variable
  if (node->return_value()) {
      node->return_value()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.LOCAL(_function->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (node->return_value()->is_typed(cdk::TYPE_INT))
          _pf.I2D();
        _pf.LOCAL(_function->offset());
        _pf.STDOUBLE();
      } else {
        error(node->lineno(), "cannot initialize return value");
      }
    } 

  if (node->prologue()) {
    _prologue = true;
    os() << "        ;; prologue " << std::endl;
    node->prologue()->accept(this, lvl + 4); // block has its own scope
    _prologue = false;
  }
  if (node->block()) {
    os() << "        ;; block " << std::endl;
    node->block()->accept(this, lvl + 4); // block has its own scope
  }
  _pf.LABEL(_currentBodyRetLabel);
  
  if (node->epilogue()) {
    _currentBodyRetLabel = mklbl(++_lbl);
    os() << "        ;; epilogue " << std::endl;
    node->epilogue()->accept(this, lvl + 4); // block has its own scope
    _pf.LABEL(_currentBodyRetLabel);
  }
  
  if (node->prologue()) {
    _symtab.pop();  // exit prologue scope
  }
  
  _inFunctionBody = false;
  _returnSeen = false;  // TODO needed?


  // TODO should this be done somewhere else?
  // return_value
  if (_function->type()->name() != cdk::TYPE_VOID) {
      _pf.LOCAL(_function->offset());

    if (_function->type()->name() == cdk::TYPE_INT || _function->type()->name() == cdk::TYPE_STRING
        || _function->type()->name() == cdk::TYPE_POINTER) {
      _pf.LDINT();
      _pf.STFVAL32();
    } else if (_function->type()->name() == cdk::TYPE_DOUBLE) {
      _pf.LDDOUBLE();
      _pf.STFVAL64();
    } else {
      error(node->lineno(), ": should not happen: unknown return type");
    }
  }

  _returnSeen = true;

  _pf.LEAVE();
  _pf.RET();

  _symtab.pop(); // scope of arguments

  if (node->identifier() == "fir") {
    // declare external functions
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
}

void fir::postfix_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto symbol = _symtab.find(node->identifier());

  size_t argsSize = 0;
  if (node->arguments()) {
    if (node->arguments()->size() > 0) {
      for (int ax = node->arguments()->size() - 1; ax >= 0; ax--) {
        cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax));
        arg->accept(this, lvl + 2);
        if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) {
          _pf.I2D();
        }
        argsSize += symbol->argument_size(ax);
      }
    }
  }
  _pf.CALL(node->identifier());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) {
    _pf.LDFVAL32();
  } else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else if (symbol->is_typed(cdk::TYPE_VOID)) {
    // EMPTY
  } else {
    error(node->lineno(), "wrong type");
  }
}

void fir::postfix_writer::do_null_node(fir::null_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}

void fir::postfix_writer::do_address_of_node(fir::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl + 2);
}

void fir::postfix_writer::do_index_node(fir::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->base()) {
    node->base()->accept(this, lvl);
  } else {
    if (_function) {
      _pf.LOCV(-_function->type()->size()); // TODO check this
    } else {
      std::cerr << "FATAL: " << node->lineno() << ": trying to use return value outside function" << std::endl;
    }
  }
  node->index()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ADD(); // add pointer and index
}

void fir::postfix_writer::do_stack_alloc_node(fir::stack_alloc_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(3); // TODO change this
  _pf.SHTL();
  _pf.ALLOC(); // allocate
  _pf.SP(); // put base pointer in stack
}

void fir::postfix_writer::do_prologue_node(fir::prologue_node *const node, int lvl) {
  _symtab.push(); // for block-local vars
  node->block()->accept(this, lvl + 2);
}

void fir::postfix_writer::do_identity_node(fir::identity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
