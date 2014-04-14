#include<cstdlib>
#include "random_graph.h"

namespace graph {

  Edge random_edge(int max_node) {
    int source = rand() % max_node;
    int target = rand() % max_node;
    while (target == source) target = rand() % max_node;
    return Edge{source, target};
  }

  Weighted_Edge random_weighted_edge(int max_node, int max_weight) {
    int source = rand() % max_node;
    int target = rand() % max_node;
    while (target == source) target = rand() % max_node;
    return Weighted_Edge{source, target, rand() % max_weight + 1};
  }

  Graph<Edge> random_graph(int node_count, int edge_count) {
    auto rnd = [&]() { return random_edge(node_count); };
    return random_graph<Edge, decltype(rnd)>(edge_count, rnd);
  }

  Graph<Weighted_Edge> random_weighted_graph(int node_count, int edge_count, int max_weight) {
    auto rnd = [&]() { return random_weighted_edge(node_count, max_weight); };
    return random_graph<Weighted_Edge, decltype(rnd)>(edge_count, rnd);
  }

}
