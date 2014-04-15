#include<iostream>
#include "graph_algo.h"
#include "random_graph.h"

using namespace std;
using namespace graph;

int main() {
  Graph<Edge> graph = random_graph(5,15);
  for (Edge e: graph) {
    cout << e << '\n';
  }

  cout << "Weighted\n";
  Graph<Weighted_Edge> w_graph = random_weighted_graph(10,12,32);
  for (Weighted_Edge e : w_graph) {
    cout << e << '\n';
  }

  cout << "Reversed\n";
  for (Weighted_Edge e : dual(w_graph)) {
    cout << e << '\n';
  }
}
