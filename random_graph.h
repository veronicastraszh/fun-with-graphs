#include<set>
#include<utility>
//#include<functional>
#include "graph.h"
#include "edge.h"

using namespace std;

#ifndef RANDOM_GRAPH_MAIN_H
#define RANDOM_GRAPH_MAIN_H

namespace graph {

  Edge random_edge(int max_node);
  Weighted_Edge random_weighted_edge(int max_node, int max_weight);

  template<class E, class C> Graph<E> random_graph(int edge_count, C rnd) {
    Graph<E> result{};
    set<pair<int,int>> already_in{};
    for(int i = 0; i < edge_count; i++) {
      for(;;) {
	E edge = rnd();
	pair<int,int> loc{edge.source,edge.target};
	if (already_in.find(loc) == already_in.end()) {
	  already_in.insert(loc);
	  result += edge;
	  break;
	}
      }
    }
    return result;
  }

  Graph<Edge> random_graph(int node_count, int edge_count);
  Graph<Weighted_Edge> random_weighted_graph(int node_count, int edge_count, int max_weight);

}

#endif
