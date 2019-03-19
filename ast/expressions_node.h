#ifndef __M19_EXPRESSIONSNODE_H__
#define __M19_EXPRESSIONSNODE_H__

#include <vector>
#include <cdk/ast/expression_node.h>

namespace m19 {

  /**
   * Class for describing 'expressions' nodes.
   */
  class expressions_node: public cdk::basic_node {

    typedef std::vector<cdk::basic_node*> expressions_type;
    expressions_type _nodes;

  public:
    /*inline*/ expressions_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    /*inline*/ expressions_node(int lineno, cdk::basic_node *item, expressions_node *expressions = nullptr) :
        basic_node(lineno) {
    if (expressions != nullptr)
        _nodes = expressions->nodes();
    _nodes.push_back(item);
    }

  public:

    /*inline*/ ~expressions_node() {
      for (auto node : _nodes)
        delete node;
      _nodes.clear();
    }

  public:
    basic_node *node(size_t i) {
      return _nodes[i];
    }
    expressions_type &nodes() {
      return _nodes;
    }
    size_t size() {
      return _nodes.size();
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_expressions_node(this, level);
    }

  };

}

#endif
