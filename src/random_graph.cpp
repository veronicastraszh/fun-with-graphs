// Simple random graphs
// by Veronica Straszheim

#include<cstdlib>
#include "random_graph.h"

namespace graph {
    
    Edge random_edge(Edge::node_type max_node) {
        typename Edge::node_type source = rand() % max_node;
        typename Edge::node_type target = rand() % max_node;
        while (target == source) target = rand() % max_node;
        return Edge{source, target};
    }
    
    Weighted_Edge random_weighted_edge(typename Weighted_Edge::node_type max_node,
                                       typename Weighted_Edge::weight_type max_weight) {
        typename Weighted_Edge::node_type source = rand() % max_node;
        typename Weighted_Edge::node_type target = rand() % max_node;
        while (target == source) target = rand() % max_node;
        return Weighted_Edge{source, target, rand() % max_weight + 1};
    }
    
    Graph<Edge> random_graph(typename Edge::node_type node_count,
                             unsigned int edge_count) {
        auto rnd = [&]() { return random_edge(node_count); };
        return random_graph<Edge, decltype(rnd)>(edge_count, rnd);
    }
    
    Graph<Weighted_Edge> random_weighted_graph(typename Weighted_Edge::node_type node_count,
                                               unsigned int edge_count,
                                               typename Weighted_Edge::weight_type max_weight) {
        auto rnd = [&]() { return random_weighted_edge(node_count, max_weight); };
        return random_graph<Weighted_Edge, decltype(rnd)>(edge_count, rnd);
    }

}

// end of file
