#include <utility>
#include <vector>
#include "graph.h"
#include "edge.h"

using namespace std;

#ifndef GRAPH_ALGO_MAIN_H
#define GRAPH_ALGO_MAIN_H

namespace graph {

  template<class E> Graph<E>
  dual(const Graph<E>& g) {
    Graph<E> result{};
    for (E e : g) {
      result += reverse_edge(e);
    }
    return result;
  }


  template<class E, template<class,class> class H, class W=long>
  pair<vector<W>,vector<int>> dijkstra(Graph<E> g, int source_node, int max_edge_cost) {
    vector<long> costs(g.node_count(), -1);
    vector<int> parents(g.node_count(), -1);
    H<W,W> heap(max_edge_cost + 1);
    vector<typename decltype(heap)::location_type> locations(g.node_count());
    costs[source_node] = 0;
    locations[source_node] = heap.insert(0, source_node);
    while(!heap.empty()) {
      for (auto node : heap.find_min()) {
	for (auto edge : g[node]) {
	  long this_cost = costs[node] + edge.weight;
	  if (costs[edge.target] == -1) {
	    // new entry
	    costs[edge.target] = this_cost;
	    parents[edge.target] = node;
	    locations[edge.target] = heap.insert(this_cost, edge.target);
	  } else if (this_cost < costs[edge.target]) {
	    // existing entry improved
	    parents[edge.target] = node;
	    locations[edge.target] = heap.decrease_key(locations[edge.target], costs[edge.target], this_cost);
	    costs[edge.target] = this_cost;
	  }
	}
      }
      heap.delete_min();
    }
    return make_pair(costs, parents);
  }
  
  template<class E, template<class,class> class H, class W=long>
  pair<vector<W>,vector<int>> dijkstra(Graph<E> g, int source_node) {
    int max_edge_cost = 0;
    for (auto e : g) {
      if (e.weight > max_edge_cost) max_edge_cost = e.weight;
    }
    return dijkstra<E,H,W>(g, source_node, max_edge_cost);
  }

}

#endif
