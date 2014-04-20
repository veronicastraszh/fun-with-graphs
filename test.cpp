#include<iostream>
#include "graph_algo.h"
#include "random_graph.h"
#include "heaps.h"

using namespace std;
using namespace graph;
using namespace graph::heaps;

template<class E>
void print_graph(const Graph<E>& g) {
  for (auto edge : g) {
    cout << edge << '\n';
  }
}

void test_dijkstra() {
  Graph<Weighted_Edge> g = random_weighted_graph(100,500,25);
  print_graph(g);
  cout << '\n';
  auto results = dijkstra<Weighted_Edge, dial_heap>(g, 0);
  for (int i = 0; i < results.first.size(); i++) {
    cout << "node " << i << " cost " << results.first[i] << " parent " << results.second[i] << '\n';
  }
}

void test_dfw() {
  Graph<Edge> g = random_graph(15, 15);
  print_graph(g);
  auto pre = [](int n) { cout << "visit " << n << '\n'; };
  dfw_all(g, pre);
}

void test_scc() {
  Graph<Edge> g = random_graph(15, 15);
  print_graph(g);
  vector<vector<int>> result = scc(g);
  for (auto v : result) {
    cout << "component";
    for (auto n : v) {
      cout << ' ' << n;
    }
    cout << '\n';
  }
}

int main() {
  test_scc();
}

