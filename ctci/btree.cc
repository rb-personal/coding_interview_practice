#include <cstdio>
#include <cstdlib>
#include <vector>

// Chapter 4 -- Trees and Graphs
// Binary Tree implementation
template
<class t>
class btree_node_t
{
public:
  btree_node_t(const t value)
    : _v (value),
      _l (nullptr),
      _r (nullptr)
  {;}

  t get_v(void) { return _v };
  btree_node_t<t> *get_l(void) { return _l; }
  btree_node_t<t> *get_r(void) { return _r; }

  void set_v(t new_v) { _v = new_v; }
  void set_l(btree_node_t *new_l) { _l = new_l; }
  void set_r(btree_node_t *new_r) { _r = new_r; }

private:
  btree_node_t(void) = delete;
  btree_node_t(const btree_node_t&) = delete;

  t _v;
  btree_node_t<t> *_l;
  btree_node_t<t> *_r;
};

template
<class t>
void visit(btree_node_t<t> *node)
{
  if (node == nullptr) return;
  ; // TODO:
}

// In-order traversal of a Binary Tree
template
<class t>
void in_order_traversal(btree_node_t<t> *node)
{
  if(node == nullptr) return;
  in_order_traversal(node->get_l());
  vist(node);
  in_order_traversal(node->get_r());
}

// Pre-order traversal
template
<class t>
void pre_order_traversal(btree_node_t<t> *node)
{
  if (node == nullptr) return;
  visit(node);
  in_order_traversal(node->get_l());
  in_order_traversal(node->get_r());
}

// Post-order traversal
template
<class t>
void post_order_traversal(btree_node_t<t> *node)
{
  if (node == nullptr) return;
  in_order_traversal(node->get_l());
  in_order_traversal(node->get_r());
  visit(node);
}
