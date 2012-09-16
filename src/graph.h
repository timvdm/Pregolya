#ifndef PREGOLYA_GRAPH_H
#define PREGOLYA_GRAPH_H

#include <vector>
#include <cassert>

namespace Pregolya {

  typedef std::size_t Size;
  typedef std::size_t Index;

  template<typename GraphType>
  struct graph_traits
  {
    typedef GraphType graph_type;
    typedef typename GraphType::vertex_type vertex_type;
    typedef typename GraphType::edge_type edge_type;
    typedef typename GraphType::vertices_type vertices_type;
    typedef typename GraphType::edges_type edges_type;
    typedef typename GraphType::vertex_neighbors_type vertex_neighbors_type;
    typedef typename GraphType::vertex_edges_type vertex_edges_type;
    typedef typename GraphType::mutator mutator_type;
  };

  template<typename VertexType>
  struct vertex_traits
  {
    typedef typename VertexType::graph_type graph_type;
  };

  template<typename EdgeType>
  struct edge_traits
  {
    typedef typename EdgeType::graph_type graph_type;
  };

  class Graph;
  class Edge;

  class Vertex
  {
    public:
      typedef Graph graph_type;

      Index index() const
      {
        return m_index;
      }

      Graph* graph() const
      {
        return m_graph;      
      }

      Size degree() const
      {
        return m_edges.size();
      }

      const std::vector<const Edge*>& edges() const
      {
        return m_edges;
      }

      const std::vector<const Vertex*> neighbors() const;

    private:
      friend class GraphMutator;

      Vertex(Graph *graph, Index index) : m_graph(graph), m_index(index)
      {
      }

      graph_type *m_graph;
      std::vector<const Edge*> m_edges;
      Index m_index;
  };


  class Edge
  {
    public:
      typedef Graph graph_type;
      
      Graph* graph() const
      {
        return m_graph;
      }

      Index index() const
      {
        return m_index;
      }

      const Vertex* source() const
      {
        return m_source;
      }

      const Vertex* target() const
      {
        return m_target;
      }

      const Vertex* other(const Vertex *v) const
      {
        return v == m_source ? m_target : m_source;
      }

    private:
      friend class GraphMutator;

      Edge(Graph *graph, Index index) : m_graph(graph), m_index(index)
      {
      }

      Graph *m_graph;
      Index m_index;
      const Vertex *m_source;
      const Vertex *m_target;
  };

  class GraphMutator;

  class Graph
  {
    public:
      typedef Vertex vertex_type;
      typedef Edge edge_type;
      typedef std::vector<Vertex*> vertices_type;
      typedef std::vector<Edge*> edges_type;
      typedef std::vector<const Vertex*> vertex_neighbors_type;
      typedef std::vector<const Edge*> vertex_edges_type;
      typedef GraphMutator mutator_type;

      ~Graph()
      {
        for (const Vertex *v : m_vertices)
          delete v;
        for (const Edge *e : m_edges)
          delete e;
      }

      Size numVertices() const
      {
        return m_vertices.size();
      }

      Size numEdges() const
      {
        return m_edges.size();
      }

      const std::vector<Vertex*> vertices() const
      {
        return m_vertices;
      }

      const std::vector<Edge*> edges() const
      {
        return m_edges;
      }

      bool isAdjacent(const Vertex *v, const Vertex *w) const
      {
        std::vector<const Vertex*> nbrs(v->neighbors());
        for (const Vertex *nbr : nbrs)
          if (nbr == w)
            return true;
        return false;      
      }

   private:
      friend class GraphMutator;

      std::vector<Vertex*> m_vertices;
      std::vector<Edge*> m_edges;
  };

  class GraphMutator
  {
    public:
      GraphMutator(Graph *graph) : m_graph(graph)
      {
      }

      void construct(Size n, const std::vector<std::pair<Index, Index>> &E)
      {
        for (Size i = 0; i < n; ++i)
          addVertex();
        for (const std::pair<Index, Index> &p : E) {
          assert(p.first < n && p.second < n);
          addEdge(m_graph->vertices()[p.first], m_graph->vertices()[p.second]);
        }
      }

      Vertex* addVertex()
      {
        m_graph->m_vertices.push_back(new Vertex(m_graph, m_graph->m_vertices.size()));
        return m_graph->m_vertices.back();
      }

      Edge* addEdge(const Vertex *source, const Vertex *target)
      {
        m_graph->m_edges.push_back(new Edge(m_graph, m_graph->m_edges.size()));
        Edge *e = m_graph->m_edges.back();
        e->m_source = source;
        e->m_target = target;
        const_cast<Vertex*>(source)->m_edges.push_back(e);
        const_cast<Vertex*>(target)->m_edges.push_back(e);
        return e;
      }

    private:
      Graph *m_graph;  
  };


  const std::vector<const Vertex*> Vertex::neighbors() const
  {
    std::vector<const Vertex*> nbrs;
    for (std::size_t i = 0; i < m_edges.size(); ++i)
      nbrs.push_back(m_edges[i]->other(this));
    return nbrs;
  }



}

#endif
