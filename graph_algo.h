#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
//#include <functional>
//#include <unordered_set>
#include "graph.h"
#include "edge.h"

using namespace std;

#ifndef GRAPH_ALGO_MAIN_H
#define GRAPH_ALGO_MAIN_H

namespace graph {

  /**
     COMPUTE DUAL TO A GRAPH
   **/

  template<class E> Graph<E>
  dual(const Graph<E>& g) {
    Graph<E> result{};
    for (E e : g) {
      result += reverse_edge(e);
    }
    return result;
  }


  /**
     DEPTH FIRST WALK
   **/

  enum class Visit_Type { pre, post };

  struct Visit {
    int node;
    Visit_Type type;
  };

  template<class T>
  class Do_Nothing {
  public:
    void operator()(T& nothing) { }
  };

  template<class E,
	   class Pre=Do_Nothing<int>,
	   class Post=Do_Nothing<int>,
	   class Edge=Do_Nothing<E>>
  void dfw(const Graph<E>& g,
	   int start_node,
	   vector<bool>& visited, // modified in place
	   Pre pre=Do_Nothing<int>{},
	   Post post=Do_Nothing<int>{},
	   Edge edge=Do_Nothing<E>{}) {
    stack<Visit> visits;
    visits.push({start_node, Visit_Type::pre});
    while(!visits.empty()) {
      Visit visit = visits.top(); visits.pop();
      switch(visit.type) {
      case Visit_Type::pre:
	if (visited[visit.node]) break; // we can get into the stack twice
	pre(visit.node);
	visited[visit.node] = true;
	visits.push({visit.node, Visit_Type::post});
	for (auto e : g[visit.node]) {
	  if (!visited[e.target]) {
	    edge(e);
	    visits.push({e.target, Visit_Type::pre});
	  }
	}
	break;
      case Visit_Type::post:
	post(visit.node);
	break;
      }
    }
  }

  template<class E,
	   class Pre=Do_Nothing<int>,
	   class Post=Do_Nothing<int>,
	   class Edge=Do_Nothing<E>>
  void dfw_all(const Graph<E>& g,
	       Pre pre=Do_Nothing<int>{},
	       Post post=Do_Nothing<int>{},
	       Edge edge=Do_Nothing<E>{}) {
    vector<bool> visited(g.node_count(), false);
    for (int i = 0; i < g.node_count(); i++) {
      if (!visited[i]) {
	dfw(g, i, visited, pre, post, edge);
      }
    }
  }



  /**
     STRONGLY CONNECTED COMPONENTS, using multiple DFWs
   **/

  template<class E>
  vector<vector<int>> scc(Graph<E>& g) {

    // find completion times is primary graph
    vector<int> finish_times;
    auto post1 = [&](int node) { finish_times.push_back(node); };
    dfw_all(g, Do_Nothing<int>{}, post1);

    // collect components in dual graph
    Graph<E> d = dual(g);
    vector<vector<int>> result;
    vector<int> current;
    vector<bool> visited(d.node_count(), false);
    auto pre2 = [&](int node) { current.push_back(node); };
    for (auto n = finish_times.rbegin(); n != finish_times.rend(); n++) {
      if (!visited[*n]) {
	dfw(d, *n, visited, pre2);
	result.push_back(current);
	current = vector<int>{};
      }
    }
    return result;
  }



  /**
     DIJKSTRA'S ALGORITHM
  **/

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
