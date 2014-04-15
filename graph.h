#include<vector>
#include<algorithm>


#include <iostream>


using namespace std;

// The main data structure for a graph

#ifndef GRAPH_MAIN_H
#define GRAPH_MAIN_H 1

namespace graph {

  template<class E> class Graph_Iterator;
  
  // A graph
  
  template<class E> class Graph {
  public:
    using edge_type = E;
    
    int node_count() const { return static_cast<int>(edges.size()); }
    int edge_count() const { return edges_n; }
    
    const vector<edge_type>& operator[](int node) { return edges[node]; }
    const vector<edge_type>& operator[](int node) const { return edges[node]; }
    
    Graph_Iterator<E> begin() const { return Graph_Iterator<E>{this, 0}; }
    Graph_Iterator<E> end() const { return Graph_Iterator<E>{this, static_cast<int>(edges.size())}; }
    
    Graph& operator+=(edge_type edge);
    
  private:
    int edges_n {0};
    vector<vector<edge_type>> edges;
  };
  
  template<class E>
    Graph<E>& Graph<E>::operator+=(E edge) {
    int maxVertex = max(edge.source, edge.target);
    if (maxVertex >= edges.size()) {
      edges.resize(maxVertex+1);
    }
    edges[edge.source].push_back(edge);
    edges_n += 1;
    return *this;
  }
  
  
  // A graph iterator
  
  template<class E> class Graph_Iterator {
  public:
    Graph_Iterator(const Graph<E>* g, int n) : graph{g}, node{n}, offset{0} { correct_node(); };
  
    const E& operator*() { return (*graph)[node][offset]; }
    Graph_Iterator operator++(int) { auto old = *this; ++offset; correct_node(); return old; }
    Graph_Iterator& operator++() { ++offset; correct_node(); return *this; }
    
    bool operator==(const Graph_Iterator<E>& o) { return graph == o.graph && node == o.node && offset == o.offset; }
    bool operator!=(const Graph_Iterator<E>& o) { return !(*this).operator==(o); }
  private:
    const Graph<E>* graph;
    int node;
    int offset;

    void correct_node() {
      while(node < graph->node_count() && offset == (*graph)[node].size()) {
	offset = 0;
	node++;
      }
    }
  };
  
}
      
#endif
