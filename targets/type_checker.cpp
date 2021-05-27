#include <cdk/types/typename_type.h>
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>
#include "fir_parser.tab.h"


#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->argument()->accept(this, lvl + 2);

  if (!node->argument()->is_typed(cdk::TYPE_INT))
    throw std::string("wrong type in unary logical expression");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  processIntegerBinaryExpression(node, lvl);
}

void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  processIntegerBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE)); 
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_identity_node(fir::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_INT))
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE))
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  else
    throw std::string("wrong type in argument of unary expression");
}

  void fir::type_checker::processIntegerBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

    node->right()->accept(this, lvl + 2);
    if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }

void fir::type_checker::processPIDBinaryExpression(cdk::binary_operation_node *const node, int lvl) {

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(node->left()->type());
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void fir::type_checker::processIDBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  
  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void fir::type_checker::processScalarLogicExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE)) 
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE)) 
    throw std::string("wrong type in right argument of binary expression");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::processGeneralLogicExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE) && !node->left()->is_typed(cdk::TYPE_POINTER)) 
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT) && !node->right()->is_typed(cdk::TYPE_DOUBLE) && !node->right()->is_typed(cdk::TYPE_POINTER)) 
    throw std::string("wrong type in right argument of binary expression");

  if (node->left()->type() != node->right()->type()) {
    throw std::string("same type expected on both sides of equality operator");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::checkPointer(std::shared_ptr<cdk::basic_type> lptr, std::shared_ptr<cdk::basic_type> rptr){
  std::shared_ptr<cdk::basic_type> ltype, rtype;

  ltype = cdk::reference_type::cast(lptr)->referenced();
  rtype = cdk::reference_type::cast(rptr)->referenced();

  while (ltype->name() == cdk::TYPE_POINTER && rtype->name() == cdk::TYPE_POINTER) {
    ltype = cdk::reference_type::cast(ltype)->referenced();
    rtype = cdk::reference_type::cast(rtype)->referenced();
  }

  if (ltype->name() != rtype->name()) {
    throw std::string("incompatible pointer types");
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  // only possible in add
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    node->type(node->right()->type());
  } else {
    processPIDBinaryExpression(node, lvl);
  }
}

void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  // only possible in sub
  if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    std::shared_ptr<cdk::basic_type> ltype = cdk::reference_type::cast(node->left()->type())->referenced();
    std::shared_ptr<cdk::basic_type> rtype = cdk::reference_type::cast(node->right()->type())->referenced();
    if (ltype->name() != rtype->name())
      throw std::string("can only subtract pointers of same type");
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    processPIDBinaryExpression(node, lvl);
  }
}

void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}

void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processIDBinaryExpression(node, lvl);
}

void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processIntegerBinaryExpression(node, lvl);
}

void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processScalarLogicExpression(node, lvl);
}

void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processScalarLogicExpression(node, lvl);
}

void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processScalarLogicExpression(node, lvl);
}

void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processScalarLogicExpression(node, lvl);
}

void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processGeneralLogicExpression(node, lvl);
}

void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processGeneralLogicExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 4);
  _lvalue_type = node->lvalue()->type();
  
  node->rvalue()->accept(this, lvl + 4);

  if (node->lvalue()->is_typed(cdk::TYPE_INT) && node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {
    if (node->rvalue()->is_typed(cdk::TYPE_DOUBLE) || node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to real");
    }
  } else if (node->lvalue()->is_typed(cdk::TYPE_STRING) && node->rvalue()->is_typed(cdk::TYPE_STRING)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {

    if (node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
      checkPointer(node->lvalue()->type(), node->rvalue()->type());
      node->type(node->rvalue()->type());
    } else {
      throw std::string("wrong assignment to pointer");
    }
  } else {
    throw std::string("wrong types in assignment");
  }

  _lvalue_type = nullptr; // remove last value so that it is not used again
}

//---------------------------------------------------------------------------

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void fir::type_checker::do_print_node(fir::print_node *const node, int lvl) {
  _lvalue_type = cdk::primitive_type::create(4, cdk::TYPE_INT);
  node->arguments()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
  if (!node->type()) {
    if (_lvalue_type != nullptr) {
      node->type(_lvalue_type);
    } else {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    }
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_sizeof_node(fir::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_declaration_node(fir::variable_declaration_node *const node, int lvl) {

  _lvalue_type = node->type();

  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT))
        throw std::string("wrong type for initializer (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE))
        throw std::string("wrong type for initializer (integer or double expected).");
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->initializer()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      checkPointer(node->type(), node->initializer()->type());

      if (!node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        throw std::string("wrong type for initializer (pointer expected).");
      }
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifier();
  auto symbol = fir::make_symbol(node->qualifier(), node->type(), id, (bool)node->initializer(), false);

  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
  _lvalue_type = nullptr; // remove last value so that it is not used again
}

//---------------------------------------------------------------------------

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  auto function = fir::make_symbol(node->qualifier(), node->type(), id, false, true, true);

  if (node->arguments()) {
    std::vector <std::shared_ptr<cdk::basic_type>> argtypes;
    for (size_t ax = 0; ax < node->arguments()->size(); ax++)
      argtypes.push_back(node->argument(ax)->type());
    function->set_argument_types(argtypes);
  }

  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward() &&
        ((previous->qualifier() == tPRIVATE && !(node->qualifier() == tPRIVATE)) ||
         (!(previous->qualifier() == tPRIVATE) && node->qualifier() == tPRIVATE))
    ) {
      throw std::string("conflicting definition for '" + function->name() + "'");
    } else {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void fir::type_checker::do_function_definition_node(fir::function_definition_node *const node, int lvl) {
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  _inBlockReturnType = nullptr;

  auto function = fir::make_symbol(node->qualifier(), node->type(), id, (bool)node->return_value(), true);

  if (node->arguments()) {
    std::vector <std::shared_ptr<cdk::basic_type>> argtypes;
    for (size_t ax = 0; ax < node->arguments()->size(); ax++)
      argtypes.push_back(node->argument(ax)->type());
    function->set_argument_types(argtypes);
  }

  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward() &&
        ((previous->qualifier() == tPRIVATE && !(node->qualifier() == tPRIVATE)) ||
         (!(previous->qualifier() == tPRIVATE) && node->qualifier() == tPRIVATE))
    ) {
      throw std::string("conflicting definition for '" + function->name() + "'");
    } else {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }

  if (node->type()->name() == cdk::TYPE_VOID && node->return_value()) {
    throw std::string("cannot specify return_value for a void function");
  } 

  if (node->return_value()) {
    node->return_value()->accept(this, lvl + 2);

    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->return_value()->is_typed(cdk::TYPE_INT))
        throw std::string("wrong type for return_value (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (!node->return_value()->is_typed(cdk::TYPE_INT) && !node->return_value()->is_typed(cdk::TYPE_DOUBLE))
        throw std::string("wrong type for return_value (integer or double expected).");
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->return_value()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for return_value (string expected).");
      }
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      std::cout << "yes its null" << std::endl;
      if (!node->return_value()->is_typed(cdk::TYPE_POINTER)) {
        throw std::string("wrong type for return_value (pointer expected).");
      }
    } else {
      throw std::string("unknown type for return_value.");
    }
  }

  if (!node->prologue() && !node->block() && !node->epilogue()) {
    throw std::string("at least one of the parts is needed");
  }
}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->identifier();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");
  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());
  
  if (node->arguments()) {
    if (node->arguments()->size() == symbol->number_of_arguments()) {
      node->arguments()->accept(this, lvl + 4);
      for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
        if (node->argument(ax)->type() == symbol->argument_type(ax)) {
          if (node->argument(ax)->is_typed(cdk::TYPE_POINTER))
            checkPointer(node->argument(ax)->type(), symbol->argument_type(ax));
          continue;
        }
        if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && node->argument(ax)->is_typed(cdk::TYPE_INT)) continue;
        throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " of '" + id + "'.");
      }
    } else {
      throw std::string(
          "number of arguments in call (" + std::to_string(node->arguments()->size()) + ") must match declaration ("
              + std::to_string(symbol->number_of_arguments()) + ").");
    }
  }
}

void fir::type_checker::do_null_node(fir::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, nullptr));
}

void fir::type_checker::do_address_of_node(fir::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

void fir::type_checker::do_index_node(fir::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  std::shared_ptr<cdk::reference_type> btype;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    btype = cdk::reference_type::cast(node->base()->type());
    if (!node->base()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected in index left-value");
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected in left-value index");

  node->type(btype->referenced());
}

void fir::type_checker::do_stack_alloc_node(fir::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in allocation expression");
  }

  if (_lvalue_type == nullptr) {
    throw std::string("unexpected stack alloc");
  } else if (_lvalue_type->name() != cdk::TYPE_POINTER) {
    node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
  } else {
    node->type(_lvalue_type);
  }
}

void fir::type_checker::do_prologue_node(fir::prologue_node *const node, int lvl) {
  // EMPTY
}
