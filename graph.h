#include<vector>
#include<algorithm>

using namespace std;

// The main data structure for a graph

#ifndef GRAPH_MAIN_H
#define GRAPH_MAIN_H 1


template<class E> class Graph_Iterator;;

// A graph

template<class E>
class Graph {
public:
  using edge_type = E;

  int node_count() const { return edges.size(); }
  int edge_count() const { return edges_n; }

  const vector<edge_type>& operator[](int node) { return edges[node]; }
  const vector<edge_type>& operator[](int node) const { return edges[node]; }

  Graph_Iterator<E> begin() const { return Graph_Iterator<E>{this, 0}; }
  Graph_Iterator<E> end() const { return Graph_Iterator<E>{this, edges.size()}; }

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

template<class E>
class Graph_Iterator {
public:
  Graph_Iterator(const Graph<E>* g, int n) : graph{g}, at_end{false} { enter_node(n); }

  const E& operator*() { return *current; }
  Graph_Iterator operator++(int) { auto old = *this; advance(); return old; }
  Graph_Iterator& operator++() { advance(); return *this; }
  void advance();

  bool operator==(const Graph_Iterator<E>& o);
  bool operator!=(const Graph_Iterator<E>& o);
private:
  const Graph<E>* graph;
  int cur_node;
  bool at_end;
  typename vector<E>::const_iterator current;
  typename vector<E>::const_iterator end;

  void enter_node(int node);
};

template<class E>
bool Graph_Iterator<E>::operator==(const Graph_Iterator<E>& o) {
  return graph == o.graph
    && cur_node == o.cur_node
    && at_end == o.at_end
    && current == o.current;
}

template<class E>
bool Graph_Iterator<E>::operator!=(const Graph_Iterator<E>& o) {
  return !(*this).operator==(o);
}

template<class E>
void Graph_Iterator<E>::advance() {
  current++;
  while (current == end && !at_end) {
    enter_node(cur_node+1);
  }
}

template<class E>
void Graph_Iterator<E>::enter_node(int node) {
  if (node < graph->node_count()) {
    const vector<E> edges = (*graph)[node];
    current = edges.begin();
    end = edges.end();
  } else {
    current = end = typename vector<E>::const_iterator{};
    at_end = true;
  }
  cur_node = node;
}

#endif
