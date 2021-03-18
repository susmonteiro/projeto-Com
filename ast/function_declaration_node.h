#ifndef __FIR_AST_FUNCTION_DECLARATION_H__
#define __FIR_AST_FUNCTION_DECLARATION_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/literal_node.h>

namespace fir {

  //!
  //! Class for describing function declarations.
  //! <pre>
  //! declaration: type qualifier id '(' args ')'
  //!            {
  //!              new fir::function::Declaration(LINE, $1, $2, $3, $5);
  //!            }
  //! </pre>
  //!
  class function_declaration_node: public cdk::typed_node {
    std::string _identifier;
    int _qualifier;
    cdk::sequence_node *_arguments;
    cdk::expression_node *_return_value;

  public:
    function_declaration_node(int lineno, const std::string &identifier, int qualifier, std::shared_ptr<cdk::basic_type> functionType,
                              cdk::sequence_node *arguments, cdk::expression_node *return_value) :
        cdk::typed_node(lineno), _identifier(identifier), _qualifier(qualifier), _arguments(arguments), _return_value(return_value) {
      type(functionType);
    }

  public:
    const std::string& identifier() const {
      return _identifier;
    }
    int qualifier() {
      return _qualifier;
    }

    cdk::sequence_node* arguments() {
      return _arguments;
    }
    cdk::expression_node* return_value() {
      return _return_value;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // fir

#endif
