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
  Graph<Weighted_Edge> g = random_weighted_graph(8, 25, 25);
  print_graph(g);
  cout << '\n';
  auto results = dijkstra<Weighted_Edge, radix_heap>(g, 0);
  for (unsigned int i = 0; i < results.first.size(); i++) {
    cout << "node " << i << " cost " << results.first[i] << " parent " << results.second[i] << '\n';
  }
}

void test_dfw() {
  Graph<Edge> g = random_graph(16, 22);
  print_graph(g);
  auto pre = [](int n) { cout << "pre " << n << '\n'; };
  auto post = [](int n) { cout << "post " << n << '\n'; };
  auto edge = [](Edge e) { cout << "edge " << e << '\n'; };
  dfw_all(g, pre, post, edge);
}

void test_scc() {
  Graph<Edge> g;
  g += {0,1};
  g += {1,2};
  g += {1,3};
  g += {1,4};
  g += {2,0};
  g += {3,0};
  g += {3,5};
  g += {3,7};
  g += {4,5};
  g += {5,6};
  g += {6,4};
  g += {7,5};
  print_graph(g);
  vector<vector<typename Graph<Edge>::node_type>> result = scc(g);
  for (auto v : result) {
    cout << "component";
    for (auto n : v) {
      cout << ' ' << n;
    }
    cout << '\n';
  }
}

int main() {
  //test_dfw();
  test_scc();
  //test_dijkstra();
}

