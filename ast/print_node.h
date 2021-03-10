#ifndef __FIR_AST_PRINT_NODE_H__
#define __FIR_AST_PRINT_NODE_H__

#include <cdk/ast/sequence_node.h>

namespace fir {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::sequence_node *_arguments;

  public:
    inline print_node(int lineno, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _arguments(arguments) {
    }

  public:
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // fir

#endif
