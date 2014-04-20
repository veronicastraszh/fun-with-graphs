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

  class Do_Nothing0 {
  public:
    void operator()() { }
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
    vector<bool> seen(g.node_count(), false);
    visits.push({start_node, Visit_Type::pre});
    while(!visits.empty()) {
      Visit visit = visits.top(); visits.pop();
      switch(visit.type) {
      case Visit_Type::pre:
	pre(visit.node);
	visited[visit.node] = true;
	visits.push({visit.node, Visit_Type::post});
	for (auto e : g[visit.node]) {
	  if (!visited[e.target] && !seen[e.target]) {
	    edge(e);
	    visits.push({e.target, Visit_Type::pre});
	    seen[e.target] = true;
	  }
	}
	break;
      case Visit_Type::post:
	post(visit.node);
	break;
      }
    }
  }

  class Default_Selector {
  public:
    int operator()(vector<bool>& visited) {
      for(int i = 0; i < visited.size(); i++) {
	if (!visited[i]) return i;
      }
      return -1;
    };
  };

  template<class E,
	   class Pre=Do_Nothing<int>,
	   class Post=Do_Nothing<int>,
	   class Edge=Do_Nothing<E>,
	   class Sel=Default_Selector,
	   class Comp=Do_Nothing0>
  void dfw_all(const Graph<E>& g,
	       Pre pre=Do_Nothing<int>{},
	       Post post=Do_Nothing<int>{},
	       Edge edge=Do_Nothing<E>{},
	       Sel sel=Default_Selector{},
	       Comp comp=Do_Nothing0{}) {
    vector<bool> visited(g.node_count(), false);
    for(;;) {
      int next_node = sel(visited);
      if (next_node == -1) break;
      dfw(g, next_node, visited, pre, post, edge);
      comp();
    }
  }



  /**
     STRONGLY CONNECTED COMPONENTS, using multiple DFWs
   **/

  template<class E>
  vector<vector<int>> scc(Graph<E>& g) {
    vector<int> finish_times;
    auto pre1 = [](int node) { cout << node << ' '; };
    auto post1 = [&](int node) { finish_times.push_back(node); };
    dfw_all(g, /*Do_Nothing<int>{}*/ pre1, post1);
    cout << '\n';
    for (int n : finish_times) { cout << n << ' '; } ; cout << '\n';
    Graph<E> d = dual(g);
    vector<vector<int>> result;
    vector<int> current;
    auto pre2 = [&](int node) { cout << node << ' '; current.push_back(node); };
    auto comp2 = [&]() { cout << "c "; result.push_back(current); current = vector<int>{}; };
    auto where = finish_times.rbegin();
    auto sel2 = [&](vector<bool> visited) {
      while (where != finish_times.rend()) {
	if (!visited[*(where)]) return *where;
	where++;
      }
      return -1;
    };
    dfw_all(d, pre2, Do_Nothing<int>{}, Do_Nothing<E>{}, sel2, comp2);
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
