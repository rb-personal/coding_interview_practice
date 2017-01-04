#include <cstdio>
#include <cstdlib>
#include <vector>
#include <deque>

using namespace std;

// Graph implementation (Adjacency list)
typedef struct {
  std::vector<graph_node_t*> _adj_list;
  int value;
  bool visited;
} graph_node_t;

typedef struct {
  std::vector<graph_node_t*> _nodes;
} graph_t;

void visit(graph_node_t *node)
{
  //
}

// Depth-first search
// **NOTE** For a graph we have to keep track of
// whether we have visited a certain node -- o/w
// we will get stuck in an infinite loop
void dfs_search(graph_node_t *root)
{
  if (root == nullptr) return;
  visit(root);
  root.visited = true;
  for (auto iter = root->_nodes.begin(); iter != root->_nodes.end(); ++iter)
    if (!(*iter)->visited) search(*iter);
}

// Breadth-first search
// **NOTE** base implementation uses a queue
// recursive implementation is more tricky
void bfs_search(graph_node_t *root)
{
  std::deque<decltype(root)> roots;
  roots.push_back(root);

  while (!roots.empty()) {
    auto front = roots.front();
    roots.pop_front();
    visit(front);

    for (auto iter = front->_nodes.begin(); iter != front->_nodes.end(); iter++)
      if(!(*iter)->visited) roots.push_back(*iter);
  }
}
