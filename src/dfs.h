#ifndef PREGOLYA_DFS_H
#define PREGOLYA_DFS_H

#include "graph.h"

namespace Pregolya {

  template<typename GraphType>
  struct DFSVisitor
  {
    typedef typename graph_traits<GraphType>::vertex_type vertex_type;
    typedef typename graph_traits<GraphType>::edge_type edge_type;

    void vertex(const vertex_type *v) {}
    void edge(const edge_type *e) {}
    void backtrack(const vertex_type *v) {}
    void closure(const edge_type *edge) {}
  };

  enum DFSColor { White, Gray, Black };

  template<typename VertexType, typename DFSVisitorType>
  void dfs_visit(const VertexType *v, DFSVisitorType &visitor, std::vector<DFSColor> &visited)
  {
    typedef VertexType vertex_type;
    typedef typename vertex_traits<VertexType>::graph_type graph_type;
    typedef typename graph_traits<graph_type>::edge_type edge_type;

    visited[v->index()] = Gray;
    visitor.vertex(v);
      
    for (const edge_type *e : v->edges()) {
      const vertex_type *w = e->other(v);

      if (visited[w->index()] != White) {
        if (visited[w->index()] == Black)
          visitor.closure(e);
        continue;
      }

      visitor.edge(e);

      dfs_visit(w, visitor, visited);
    }
    
    visitor.backtrack(v);
    
    visited[v->index()] = Black;
  }

  template<typename GraphType, typename DFSVisitorType>
  void depth_first_search(const GraphType *g, DFSVisitorType &visitor)
  {
    std::vector<DFSColor> visited(g->numVertices(), White);
    for (const Vertex *v : g->vertices())
      if (visited[v->index()] != White)
        dfs_visit(v, visitor, visited);
  }

}

#endif
