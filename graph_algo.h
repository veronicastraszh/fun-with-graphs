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

  template<class T>
  class Do_Nothing {
  public:
    void operator()(T& nothing) {
#pragma unused(nothing)
    }
  };

  template<class T, class S>
  class Do_Nothing2 {
  public:
    void operator()(T& nothing, S&nothing2) {
#pragma unused(nothing)
#pragma unused(nothing2)
    }
  };

  namespace dfw_help {

    enum class Visit_Type { pre, post };
    
    template<class E>
    struct Visit {
      using node_type = typename Graph<E>::node_type;
      Visit(E e) : type{Visit_Type::pre} { edge = e; };
      Visit(node_type n) : type{Visit_Type::post} { node = n; };
      Visit_Type type;
      union {
	node_type node;
	E edge;
      };
    };
    
  }

  /**
     dfw - depth first walk
     
     Walks from start_node, marking the visited nodes in the
     vector<bool>. For each node, call pre and post respecively in
     pre-order and post-order. For each edge, we call edge as we
     encounter it.

     Note this will only visit nodes reachable from start_node.
   **/

  template<class E,
	   class Pre=Do_Nothing<typename Graph<E>::node_type>,
	   class Post=Do_Nothing<typename Graph<E>::node_type>,
	   class Edge=Do_Nothing<E>>
  void dfw(const Graph<E>& g,
	   typename Graph<E>::node_type start_node,
	   vector<bool>& visited, // modified in place
	   Pre pre=Do_Nothing<typename Graph<E>::node_type>{},
	   Post post=Do_Nothing<typename Graph<E>::node_type>{},
	   Edge edge=Do_Nothing<E>{}) {
    using namespace dfw_help;
    stack<Visit<E>> visits;
    pre(start_node);
    visited[start_node] = true;
    visits.push(Visit<E>{start_node});
    for (auto e : g[start_node]) visits.push(Visit<E>{e});
    while(!visits.empty()) {
      Visit<E> visit = visits.top(); visits.pop();
      switch(visit.type) {
      case Visit_Type::pre:
	edge(visit.edge);
	if (visited[visit.edge.target]) break;
	pre(visit.edge.target);
	visited[visit.edge.target] = true;
	visits.push(Visit<E>{visit.edge.target});
	for (auto e : g[visit.edge.target]) visits.push(Visit<E>{e});
	break;
      case Visit_Type::post:
	post(visit.node);
	break;
      }
    }
  }

  /**
     Enter -> discovered -> pre -> each-edge(edge,!discovered->dfw) -> post
   **/

  /**
     dfw_all - depth first walk, all nodes

     Calls dfw repeatedly so that all nodes are visited. After each
     walk, finds the lowest remaining node and starts a walk from
     there.
   **/

  template<class E,
	   class Pre=Do_Nothing<typename Graph<E>::node_type>,
	   class Post=Do_Nothing<typename Graph<E>::node_type>,
	   class Edge=Do_Nothing<E>>
  void dfw_all(const Graph<E>& g,
	       Pre pre=Do_Nothing<typename Graph<E>::node_type>{},
	       Post post=Do_Nothing<typename Graph<E>::node_type>{},
	       Edge edge=Do_Nothing<E>{}) {
    using node_type = typename Graph<E>::node_type;
    vector<bool> visited(g.node_count(), false);
    for (node_type i = 0; i < g.node_count(); i++) {
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
  vector<vector<typename Graph<E>::node_type>> scc(Graph<E>& g) {
    using node_type = typename Graph<E>::node_type;

    // find completion times in primary graph
    vector<node_type> finish_times;
    auto post1 = [&](int node) { finish_times.push_back(node); };
    dfw_all(g, Do_Nothing<node_type>{}, post1);
    
    // collect components in dual graph
    Graph<E> d = dual(g);
    vector<vector<node_type>> result;
    vector<node_type> current;
    vector<bool> visited(d.node_count(), false);
    auto pre2 = [&](int node) { current.push_back(node); };
    for (auto n = finish_times.rbegin(); n != finish_times.rend(); n++) {
      if (!visited[*n]) {
	dfw(d, *n, visited, pre2);
	result.push_back(current);
	current = vector<node_type>{};
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

   **/

  template<class E, template<class,class> class H>
  pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type>>
    dijkstra(Graph<E> g,
	     typename Graph<E>::node_type source_node,
	     typename E::weight_type max_edge_cost) {
    
    using node_type = typename Graph<E>::node_type;
    using weight_type = typename E::weight_type;
    vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
    vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());
    H<weight_type, node_type> heap(g.node_count(), max_edge_cost);
    vector<typename decltype(heap)::location_type> locations(g.node_count());
    costs[source_node] = 0;
    locations[source_node] = heap.insert(0, source_node);
    while(!heap.empty()) {
      node_type node = heap.find_min();
      heap.delete_min();
      for (auto edge : g[node]) {
	weight_type this_cost = costs[node] + edge.weight;
	if (costs[edge.target] == numeric_limits<weight_type>::max()) {
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

  template<class E, template<class,class> class H>
  pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type>>
    dijkstra(Graph<E> g,
	     typename Graph<E>::node_type source_node) {
    using weight_type = typename E::weight_type;
    weight_type max_edge_cost = 0;
    for (auto e : g) {
      if (e.weight > max_edge_cost) max_edge_cost = e.weight;
    }
    return dijkstra<E,H>(g, source_node, max_edge_cost);
  }

}

#endif
