#ifndef __FIR_AST_FUNCTION_DEFINITION_H__
#define __FIR_AST_FUNCTION_DEFINITION_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include "block_node.h"

namespace fir {

  /**
   * Class for describing function definitions.
   */
  class function_definition_node: public cdk::typed_node {
    std::string _identifier;
    int _qualifier;
    cdk::sequence_node *_arguments;
    cdk::expression_node *_return_value;
    fir::prologue_node *_prologue;
    fir::block_node *_block;
    fir::block_node *_epilogue;

  public:
    function_definition_node(int lineno, const std::string &identifier, int qualifier, std::shared_ptr<cdk::basic_type> functionType,
                              cdk::sequence_node *arguments, cdk::expression_node *return_value,
                              fir::prologue_node *prologue, fir::block_node *block, fir::block_node *epilogue) :
        cdk::typed_node(lineno), _identifier(identifier), _qualifier(qualifier), _arguments(arguments), 
        _return_value(return_value), _prologue(prologue), _block(block), _epilogue(epilogue) {
      type(functionType);
    }

    function_definition_node(int lineno, const std::string &identifier, int qualifier, std::shared_ptr<cdk::basic_type> functionType,
                              cdk::sequence_node *arguments,
                              fir::prologue_node *prologue, fir::block_node *block, fir::block_node *epilogue) :
        cdk::typed_node(lineno), _identifier(identifier), _qualifier(qualifier), _arguments(arguments), 
        _prologue(prologue), _block(block), _epilogue(epilogue) {
      type(functionType);
      _return_value = (functionType->name() == cdk::TYPE_INT) ? (new cdk::integer_node(lineno, 0)) : nullptr;
    }

  public:
    const std::string& identifier() const {
      return _identifier;
    }
    
    int qualifier() {
      return _qualifier;
    }

    cdk::typed_node *argument(size_t ax) {
      return dynamic_cast<cdk::typed_node *>(_arguments->node(ax));
    }

    cdk::sequence_node* arguments() {
      return _arguments;
    }
    cdk::expression_node* return_value() {
      return _return_value;
    }

    fir::prologue_node* prologue() {
        return _prologue;
    }

    fir::block_node* block() {
        return _block;
    }

    fir::block_node* epilogue() {
        return _epilogue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // fir

#endif
