// Simple random graphs
// by Veronica Straszheim

#ifndef RANDOM_GRAPH_MAIN_H
#define RANDOM_GRAPH_MAIN_H

#include<set>
#include<utility>
#include "graph.h"
#include "edge.h"

using namespace std;

namespace graph {

  /**
     These provide simple randomized graphs, uniformly
     distrubuted. (Which in fact does a poor job simulating real-world
     graphs. Consider these toys.)
   **/

  Edge random_edge(typename Edge::node_type max_node);

  Weighted_Edge random_weighted_edge(typename Weighted_Edge::node_type max_node,
  				     typename Weighted_Edge::weight_type max_weight);

  template<class E, class C> Graph<E> random_graph(unsigned int edge_count, C rnd) {
    using node_type = typename E::node_type;
    using marker = pair<node_type, node_type>;
    Graph<E> result{};
    set<marker> already_in{};
    for(unsigned int i = 0; i < edge_count; i++) {
      for(;;) {
  	E edge = rnd();
  	marker loc{edge.source, edge.target};
  	if (already_in.find(loc) == already_in.end()) {
  	  already_in.insert(loc);
  	  result += edge;
  	  break;
  	}
      }
    }
    return result;
  }

  

  Graph<Edge> random_graph(typename Edge::node_type node_count,
  			   unsigned int edge_count);

  Graph<Weighted_Edge> random_weighted_graph(typename Weighted_Edge::node_type node_count,
  					     unsigned int edge_count,
  					     typename Weighted_Edge::weight_type max_weight);

}

#endif

// end of file
