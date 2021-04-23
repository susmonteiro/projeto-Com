#ifndef __FIR_AST_FUNCTION_DECLARATION_H__
#define __FIR_AST_FUNCTION_DECLARATION_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>

namespace fir {

  /**
   * Class for describing function declarations.
   */
  class function_declaration_node: public cdk::typed_node {
    std::string _identifier;
    int _qualifier;
    cdk::sequence_node *_arguments;

  public:
    function_declaration_node(int lineno, const std::string &identifier, int qualifier, 
                              std::shared_ptr<cdk::basic_type> functionType, cdk::sequence_node *arguments) :
        cdk::typed_node(lineno), _identifier(identifier), _qualifier(qualifier), _arguments(arguments) {
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

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // fir

#endif
