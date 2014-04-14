#include<iostream>
#include "graph_algo.h"
#include "random_graph.h"

using namespace std;
using namespace graph;

Graph<Weighted_Edge> duale(const Graph<Weighted_Edge>& g) {
  Graph<Weighted_Edge> result{};
  auto end = g.end();
  for (auto current = g.begin(); current != end; current++) {
    cout << *current << '\n';
    result += Weighted_Edge{0,0,1}; //reverse_edge(e);
  }
  return result;
}

int main() {
  // Graph<Edge> graph = random_graph(5,15);
  // for (Edge e: graph) {
  //   cout << e << '\n';
  // }

  cout << "Weighted\n";
  Graph<Weighted_Edge> w_graph = random_weighted_graph(5,15,8);
  for (Weighted_Edge e : w_graph) {
    cout << e << '\n';
  }
  // cout << "Reversed\n";
  // for (Weighted_Edge e : dual(w_graph)) {
  //   cout << e << '\n';
  // }
  cout << "Fred\n";
  duale(w_graph);
}
