#include<iostream>
#include "graph_algo.h"
#include "random_graph.h"
#include "heaps.h"

using namespace std;
using namespace graph;
using namespace graph::heaps;

template<class E>
void print_graph(Graph<E> g) {
  for (auto edge : g) {
    cout << edge << '\n';
  }
}

int main() {
  Graph<Weighted_Edge> g = random_weighted_graph(20,50,10);
  print_graph(g);
  cout << '\n';
  auto results = dijkstra<Weighted_Edge, dial_heap>(g, 0);
  for (int i = 0; i < results.first.size(); i++) {
    cout << "node " << i << " cost " << results.first[i] << " parent " << results.second[i] << '\n';
  }
}

