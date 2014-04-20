#include <utility>
#include <vector>
#include <stack>
//#include <unordered_set>
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

  class enum Visit_Type { pre_visit, post_visit };

  template<class T>
  struct Visit {
    T data;
    Visit_Type type;
  };

  template<class T>
  class Do_Nothing {
    void operator()(T nothing) { }
  };

  template<class E, class Pre, class Post=Do_Nothing, class Edge=Do_Nothing>
  void depth_first(Graph<E> g, int start_node, Pre pre, Post post=Do_Nothing{}, Edge edge=Do_Nothing{}) {
    stack<Visit<int>> visits;
    stack.push({start_node, pre_visit});
    while(!stack.empty()) {
      Visit visit = stack.top(); stack.pop();
      switch(visit.type) {
      case pre_visit:
	pre(visit.data);
	stack.push({visit.data, post_visit});
	for (auto e : g[visit.data]) {
	  edge(e);
	  stack.push({e.target, pre_visit});
	}
	break;
      case post_visit:
	post(visit.data);
	break;
      }
    }
  }

  template<class E, class Pre, class Post=Do_Nothing, class Edge=Do_Nothing>
  void depth_first_all(Graph<E> g, Pre pre, Post post=Do_Nothing{}, Edge edge=Do_Nothing{}) {
    vector<bool> visited{g.node_count(), false};
    for(;;) {
      int next_node = -1;
      for (int i = 0; i < visited.size(); i++) {
	if (!visited[i]) {
	  next_node = i;
	  break;
	}
      }
      if (next_node == -1) break;
      auto new_pre = [&pre](int n){ visited[n] = true; pre(n); };
      depth_first(g, next_node, new_pre, post, edge);
    }
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
