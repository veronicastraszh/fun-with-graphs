// Algorithms over graphs
// by Veronica Straszheim

#ifndef GRAPH_ALGO_MAIN_H
#define GRAPH_ALGO_MAIN_H

#include <utility>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>
#include <string>
#include "graph.h"
#include "edge.h"

using namespace std;

namespace graph {
    
    /**
       COMPUTE DUAL TO A GRAPH
    **/
    
    template<class E> Graph<E>
    dual(const Graph<E>& g) 
    {
        Graph<E> result{};
        for (E e : g) {
            result += reverse_edge(e);
        }
        return result;
    }
    
    
    /**
       DEPTH FIRST WALK
    **/
    
    enum class Edge_Type { tree, forward, back, cross };
    
    /**
       dfw - depth first walk
       
       This class holds the needed data for a detailed depth first walk of a graph.
       
       For examples, see the algorithms below, such as top_sort or scc.
       
       The data structures are as follows:
       
       parent[node] := the parents of the node in the depth first walk
       entered[node] := a tick of when this node was entered, orders the nodes
       exited[node] := a tick of when this node was completed
       
       pre, post, and edge are callback functions, for pre-order and post-order walks,
       and edge classification
       
       the callback functions can set "finished" true, which will halt the walk
       
       set directed false to not explore reverse edges.
    **/
    
    template<class E>
    class dfw {
    public:
        using node_type = typename Graph<E>::node_type;
        using tick_type = unsigned long;
        const node_type token_node = numeric_limits<node_type>::max();
        const tick_type token_tick = numeric_limits<tick_type>::max();
        
        vector<node_type> parents;
        vector<tick_type> entered;
        vector<tick_type> exited;
        function<void(node_type)> pre{[](node_type n) {}};
        function<void(node_type)> post{[](node_type n) {}};
        function<void(E)> edge{[](E e) {}};
        bool finished{false};
        bool directed{true};
        
        dfw(const Graph<E>& g) :
            parents{vector<node_type>(g.node_count(), token_node)},
            entered{vector<tick_type>(g.node_count(), token_tick)},
            exited{vector<tick_type>(g.node_count(), token_tick)} {};
        
        void mark_discovered(node_type n) { entered[n] = ticks++; }
        bool discovered(node_type n) const { return entered[n] != token_tick; };
        
        void mark_processed(node_type n) { exited[n] = ticks++; };
        bool processed(node_type n) const { return exited[n] != token_tick; };
        
        bool entered_before(node_type n, node_type m) const { return entered[n] < entered[m]; }
        bool exited_before(node_type n, node_type m) const { return exited[n] < exited[m]; }
        
        Edge_Type classify_edge(E e) const;
        
        // main operation
        void operator()(const Graph<E>& g, node_type n);
        
    private:
        tick_type ticks{0};    
    };
    
    // the main walk
    template<class E>
    void dfw<E>::operator()(const Graph<E>& g, node_type n) 
    {
        if (finished) return;
        mark_discovered(n);
        pre(n);
        for (auto e : g[n]) {
            if (!discovered(e.target)) {
                parents[e.target] = n;
                edge(e);
                (*this)(g, e.target);
            } else if (directed || !processed(e.target)) {
                edge(e);
            }
            if (finished) return;
        }
        post(n);
        mark_processed(n);
    }
    
    // edge classification
    template<class E>
    Edge_Type dfw<E>::classify_edge(E e) const
    {
        if (parents[e.target] == e.source) return Edge_Type::tree;
        if (discovered(e.target) && !processed(e.target)) return Edge_Type::back;
        if (processed(e.target) && entered_before(e.source, e.target)) return Edge_Type::forward;
        if (processed(e.target) && entered_before(e.target, e.source)) return Edge_Type::cross;
        throw logic_error{"Unclassified edge"};    
    }
    
    
    /**
       TOPOLOGICAL SORT
    **/
    
    /**
       top_sort - topological sort
       
       This returns the list in _reverse_ of sorted order
    **/

    template<E>
    class cycle_found : public logic_error {
    public:
        using parents_type = vector<typename E::edge_type>;
        E edge;
        parent_type parents;
        cycle_found(E e, parents_type p, char *c) : edge{e}, parents{p}, logic_error{c} {};
    };
    
    template<class E>
    vector<typename Graph<E>::node_type> top_sort(const Graph<E>& g)
    {
        using node_type = typename Graph<E>::node_type;
        vector<node_type> results;
        dfw<E> walk{g};
        walk.post = [&](node_type n) { results.push_back(n); };
        walk.edge = [&](E e) {
            if (walk.classify_edge(e) == Edge_Type::back) {
                throw cycle_found{e, walk.parents, "Cycle found"};
            }
        };
        for (node_type n = 0; n < g.node_count(); n++) {
            if (!walk.processed(n)) walk(g,n);
        }
        return results;
    }
    
    
    /**
       STRONGLY CONNECTED COMPONENTS
    **/
    
    /**
       scc - strongly connected components
       
       Given a graph, returns a vector of vectors, each of which is a
       single strongly-connected-component of g.
       
       g is a graph
       d is the dual of the graph, computed using dual(g)
    **/
    
    template<class E>
    vector<vector<typename Graph<E>::node_type>> scc(const Graph<E>& g, const Graph<E>& d)
    {
        using node_type = typename Graph<E>::node_type;
        
        // find completion times in primary graph
        vector<node_type> finish_times;
        dfw<E> walk1{g};
        walk1.post = [&](int node) { finish_times.push_back(node); };
        for (node_type n = 0; n < g.node_count(); n++) {
            if (!walk1.processed(n)) walk1(g,n);
        }
        
        // collect components in dual graph
        vector<vector<node_type>> result;
        vector<node_type> current;
        dfw<E> walk2{d};
        walk2.pre = [&](int node) { current.push_back(node); };
        for (auto n = finish_times.rbegin(); n != finish_times.rend(); n++) {
            if (!walk2.processed(*n)) {
                walk2(d, *n);
                result.push_back(current);
                current = vector<node_type>{};
            }
        }
        return result;
    }
    
    
    
    /**
       SHORTEST PATH
    **/
    
    /**
       dijkstra - Dijkstra's shortest path algoritm
       
       Walks a graph from the source_node and discovers the shortest
       path to each other node.
       
       Returns a pair of vectors. The first vector is the cost to
       reach each node. The second is the parent of each node, which
       is the node FROM which we can reach this node. In other words,
       the edge {parents(n), n} is the final edge in the path from
       source_node to n. The edge {parents(parents(n)), parents(n)} is
       likewise the second to last.
       
       E is the edge type, which must include a field named
       'weight'. That weight must be an unsigned type. (Dijkstra has
       unspecified behavior for negative edge weights.)
       
       H is a heap class, as provided in heaps.h.

       unreachable nodes are assigned a maximum value for their type.
       
    **/
    
    template<class E, template<class,class> class H>
    pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type> >
    dijkstra(const Graph<E>& g,
             typename Graph<E>::node_type source_node,
             typename E::weight_type max_edge_cost)
    {    
        using node_type = typename Graph<E>::node_type;
        using weight_type = typename E::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a heap to track the best next-node
        H<weight_type, node_type> heap(g.node_count(), max_edge_cost);

        // here we track each objects location in the heap
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
                    heap.decrease_key(locations[edge.target], costs[edge.target], this_cost);
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
    pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type> >
    dijkstra(const Graph<E>& g,
             typename Graph<E>::node_type source_node)
    {
        using weight_type = typename E::weight_type;
        weight_type max_edge_cost = 0;
        for (auto e : g) {
            if (e.weight > max_edge_cost) max_edge_cost = e.weight;
        }
        return dijkstra<E,H>(g, source_node, max_edge_cost);
    }

    /**
       q_lc - queued label correcting algorithm

       This shortest path algorithm supports negative edge costs,
       provided no negative cycles are present. (Negative cycles are
       detected, however.)

       This runs in O(n*m) worst case.
     **/

    template<class N>
    class negative_cycle_found : public logic_error {
    public:
        N node;
        vector<N> parents;
        negative_cycle_found(N n, vector<N> p, char *c) : logic_error{c}, node{n}, parents{p} {};
    };
    
    template<class E>
    pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type> >
    q_lc(const Graph<E>& g, typename Graph<E>::node_type source_node)
    {
        using node_type = typename Graph<E>::node_type;
        using weight_type = typename E::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a queue to hold our nodes-to-examine
        queue<node_type> queue;
        vector<bool> in_queue(g.node_count(), false);

        // count how many times we've seen a node, for cycle detection
        vector<node_type> counts(g.node_count);
        
        costs[source_node] = 0;
        queue.push(source_node);
        in_queue[source_node] = true;
        
        while(!queue.empty()) {
            node_type n = queue.front(); queue.pop(); in_queue[n] = false;
            if (++counts[n] >= g.node_count()) {
                throw negative_cycle_found{n, parents, "Negative cycle found"};
            }
            for (auto e : g[n]) {
                weight_type candidate_cost = costs[n] + e.weight;
                // if this edge is better, use that
                if (costs[e.target] > candidate_cost) {
                    costs[e.target] = candidate_cost;
                    parents[e.target] = n;
                    if (!in_queue[e.target]) {
                        queue.push(e.target); in_queue[e.target] = true;
                    }
                }
            }
        }
        return make_pair(costs, parents);
    }

    template<class E>
    pair<vector<typename E::weight_type>,vector<typename Graph<E>::node_type> >
    dq_lc(const Graph<E>& g, typename Graph<E>::node_type source_node)
    {
        using node_type = typename Graph<E>::node_type;
        using weight_type = typename E::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a queue to hold our nodes-to-examine
        deque<node_type> deque;
        vector<bool> in_deque(g.node_count(), false);

        // has this node been seen?
        vector<bool> seen(g.node_count(), false);
        
        costs[source_node] = 0;
        queue.push_back(source_node);
        in_deque[source_node] = true;
        seen[source_node] = true;
        
        while(!deque.empty()) {
            node_type n = deque.front(); deque.pop_front(); in_deque[n] = false;
            for (auto e : g[n]) {
                weight_type candidate_cost = costs[n] + e.weight;
                // if this edge is better, use that
                if (costs[e.target] > candidate_cost) {
                    costs[e.target] = candidate_cost;
                    parents[e.target] = n;
                    if (!in_deque[e.target]) {
                        if (seen[target]) {
                            deque.push_front(n); in_deque[n] = true;
                        } else {
                            deque.push_back(n); in_deque[n] = true; seen[n] = true;
                        }
                    }
                }
            }
        }
        return make_pair(costs, parents);
    }

    
}

#endif
