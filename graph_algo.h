// Algorithms over graphs
// by Veronica Straszheim

#ifndef GRAPH_ALGO_MAIN_H
#define GRAPH_ALGO_MAIN_H

#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
#include "graph.h"
#include "edge.h"

using namespace std;

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

  /**
     dfw - depth first walk
     
     Walks from start_node, marking the visited nodes in the
     vector<bool>. For each node, call pre and post respecively in
     pre-order and post-order. For each edge, we call edge as we
     encounter it.

     Note this will only visit nodes reachable from start_node.
   **/

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
	  edge(e);
	  if (!visited[e.target]) {
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

  /**
     dfw_all - depth first walk, all nodes

     Calls dfw repeatedly so that all nodes are visited. After each
     walk, finds the lowest remaining node and starts a walk from
     there.
   **/

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
     STRONGLY CONNECTED COMPONENTS
   **/

  /**
     scc - strongly connected components

     Given a graph, returns a vector of vectors, each of which is a
     single strongly-connected-component of g.
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

  /**
     dijkstra - Dikstra's shortest path algoritm

     Walks a graph from the source_node and discovers the shortest
     path to each other node.

     Returns a pair of vectors. The first vector is the cost to reach
     each node. (-1 is returned for unreachable nodes.) The second is
     the parent of each node, which is the node FROM which we can
     reach this node. In other words, the edge {parent(n), n} is the
     final edge in the path from source_node to n. The edge
     {parent(parent(n)), parent(n)} is likewise the second to last.

     E is the edge type, which must include a field named weight.

     H is a heap class, as provided in heaps.h.

     W is the type of edge_cost, usually long.
   **/

  template<class E, template<class,class> class H, class W=long>
  pair<vector<W>,vector<int>> dijkstra(Graph<E> g, int source_node, int max_edge_cost) {
    vector<long> costs(g.node_count(), -1);
    vector<int> parents(g.node_count(), -1);
    H<W,W> heap(g.node_count(), max_edge_cost);
    vector<typename decltype(heap)::location_type> locations(g.node_count());
    costs[source_node] = 0;
    locations[source_node] = heap.insert(0, source_node);
    while(!heap.empty()) {
      int node = heap.find_min();
      heap.delete_min();
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
    return make_pair(costs, parents);
  }
  
  /**
     dijkstra - Dikstra's shortest path algoritm

     As above, but computes the max_edge_cost.
  **/

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
