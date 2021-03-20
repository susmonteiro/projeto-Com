#ifndef __FIR_AST_EPILOGUE_node_H__
#define __FIR_AST_EPILOGUE_node_H__

#include <cdk/ast/basic_node.h>
#include "block_node.h"

namespace fir {

  /**
   * Class for describing block nodes.
   */
  class epilogue_node: public cdk::basic_node {
    fir::block_node *_block;

  public:
    inline epilogue_node(int lineno, block_node *block) :
        cdk::basic_node(lineno), _block(block) {
    }

  public:
    inline fir::block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_epilogue_node(this, level);
    }

  };

} // fir

#endif