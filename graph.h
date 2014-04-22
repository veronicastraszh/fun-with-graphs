// A datatype for graphs
// by Veronica Straszheim

#ifndef GRAPH_MAIN_H
#define GRAPH_MAIN_H 1

#include<vector>
#include<algorithm>

using namespace std;


namespace graph {

  template<class E> class Graph_Iterator;
  
  /**
     A Graph.

     The template parameter E should be some edge type, from
     edges.h. For a basic graph, any type with members named source
     and target will do.

     Edges are added to a graph using the += operator.

     Edges are looked up using the [] operator, where the node id is provided.

     A graph supports iteration, which provides all edges.
   **/
  
  template<class E>
  class Graph {
  public:
    using edge_type = E;
    using size_type = typename vector<edge_type>::size_type;
    using node_type = typename E::node_type;
    
    node_type node_count() const { return static_cast<node_type>(edges.size()); }
    size_type edge_count() const { return edges_n; }
    
    const vector<edge_type>& operator[](node_type node) { return edges[node]; }
    const vector<edge_type>& operator[](node_type node) const { return edges[node]; }
    
    Graph_Iterator<E> begin() const { return Graph_Iterator<E>{*this, 0}; }
    Graph_Iterator<E> end() const {
      return Graph_Iterator<E>{*this, static_cast<node_type>(edges.size()), 0};
    }
    
    Graph& operator+=(edge_type edge);
    
  private:
    size_type edges_n {0};
    vector<vector<edge_type>> edges;
  };
  
  template<class E>
  Graph<E>& Graph<E>::operator+=(E edge) {
    node_type maxVertex = max(edge.source, edge.target);
    if (maxVertex >= edges.size()) {
      edges.resize(maxVertex+1);
    }
    edges[edge.source].push_back(edge);
    edges_n += 1;
    return *this;
  }
  
  
  // A graph iterator
  
  template<class E>
  class Graph_Iterator {
  public:
    using node_type = typename E::node_type;

    Graph_Iterator(const Graph<E>& g, node_type n) : graph{g}, node{n}, offset{0} { correct_node(); };
    Graph_Iterator(const Graph<E>& g, node_type n, node_type o) : graph{g}, node{n}, offset{o} {}; // no check
  
    const E& operator*() { return graph[node][offset]; }
    Graph_Iterator operator++(int) { auto old = *this; ++offset; correct_node(); return old; }
    Graph_Iterator& operator++() { ++offset; correct_node(); return *this; }
    
    bool operator==(const Graph_Iterator<E>& o) { return node == o.node && offset == o.offset; }
    bool operator!=(const Graph_Iterator<E>& o) { return !(*this).operator==(o); }

  private:
    const Graph<E>& graph;
    node_type node;
    node_type offset;

    void correct_node() {
      while(node < graph.node_count() && offset == graph[node].size()) {
	offset = 0;
	node++;
      }
    }
  };
  
}
      
#endif

// end of file

