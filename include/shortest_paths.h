// Algorithms to compute shortest paths
// by Veronica Straszheim

#ifndef SHORTEST_PATHS_H
#define SHORTEST_PATHS_H

#include <vector>
#include <limits>
#include <utility>
#include <queue>
#include <deque>

using namespace std;

namespace graph {

    /**
       DIJKSTRA'S
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
       
       G::edge_type must include a field named 'weight()'. That weight
       must be an unsigned type. (Dijkstra has unspecified behavior
       for negative edge weights.)
       
       H is a heap class, as provided in heaps.h.

       unreachable nodes are assigned a maximum value for their type.
       
    **/
    
    template<class G, template<class,class> class H>
    pair<vector<typename G::edge_type::weight_type>,
         vector<typename G::node_type> >
    dijkstra(const G& g,
             typename G::node_type source_node,
             typename G::edge_type::weight_type max_edge_cost)
    {
        using edge_type = typename G::edge_type;
        using node_type = typename G::node_type;
        using weight_type = typename edge_type::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a heap to track the best next-node
        H<weight_type, node_type> heap(g.node_count(), max_edge_cost);

        // here we track each object's location in the heap
        vector<typename decltype(heap)::location_type> locations(g.node_count());

        costs[source_node] = 0;
        locations[source_node] = heap.insert(0, source_node);

        while(!heap.empty()) {
            node_type node = heap.find_min();
            heap.delete_min();
            for (auto edge : g[node]) {
                weight_type this_cost = costs[node] + edge.weight();
                if (costs[edge.target()] == numeric_limits<weight_type>::max()) {
                    // new entry
                    costs[edge.target()] = this_cost;
                    parents[edge.target()] = node;
                    locations[edge.target()] = heap.insert(this_cost, edge.target());
                } else if (this_cost < costs[edge.target()]) {
                    // existing entry improved
                    parents[edge.target()] = node;
                    heap.decrease_key(locations[edge.target()], costs[edge.target()], this_cost);
                    costs[edge.target()] = this_cost;
                }
            }
        }
        return make_pair(costs, parents);
    }
    
    /**
       dijkstra - Dikstra's shortest path algoritm
       
       As above, but computes the max_edge_cost.
    **/
    
    template<class G, template<class,class> class H>
    pair<vector<typename G::edge_type::weight_type>,
         vector<typename G::node_type> >
    dijkstra(const G& g,
             typename G::node_type source_node)
    {
        using edge_type = typename G::edge_type;
        using weight_type = typename edge_type::weight_type;
        weight_type max_edge_cost = 0;
        for (auto e : g) {
            if (e.weight() > max_edge_cost) max_edge_cost = e.weight();
        }
        return dijkstra<G,H>(g, source_node, max_edge_cost);
    }

    /**
       BELLMAN-FORD BASED ALGORITHMS
     **/
    
    /**
       q_lc - queued label correcting algorithm

       This shortest path algorithm supports negative edge costs,
       provided no negative cycles are present. (Negative cycles are
       detected, however.)

       Note, this algorithm is essentially Bellman-Ford, but with
       better average case performance. It runs in O(n*m) worst case.
     **/

    template<class N>
    class negative_cycle_found : public logic_error {
    public:
        N node;
        vector<N> parents;
        negative_cycle_found(N n, vector<N> p, string s) : logic_error{s}, node{n}, parents{p} {};
    };
    
    template<class G>
    pair<vector<typename G::edge_type::weight_type>,
         vector<typename G::node_type> >
    q_lc(const G& g,
         typename G::node_type source_node)
    {
        using edge_type = typename G::edge_type;
        using node_type = typename G::node_type;
        using weight_type = typename edge_type::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a queue to hold our nodes-to-examine
        queue<node_type> q;
        vector<bool> in_q(g.node_count(), false);

        // count how many times we've seen a node, for cycle detection
        vector<node_type> counts(g.node_count());
        
        costs[source_node] = 0;
        q.push(source_node);
        in_q[source_node] = true;
        
        while(!q.empty()) {
            node_type n = q.front();
            q.pop();
            in_q[n] = false;
            if (++counts[n] >= g.node_count()) {
                throw negative_cycle_found<node_type>{n, parents, "Negative cycle found"};
            }
            for (auto e : g[n]) {
                weight_type candidate_cost = costs[n] + e.weight();
                // if this edge is better, use that
                if (costs[e.target()] > candidate_cost) {
                    costs[e.target()] = candidate_cost;
                    parents[e.target()] = n;
                    // e.target() has improved; perhaps its children
                    // will improve, so recheck
                    if (!in_q[e.target()]) {
                        q.push(e.target());
                        in_q[e.target()] = true;
                    }
                }
            }
        }
        return make_pair(costs, parents);
    }

    /**
       dq_lc - deque-based label correcting algorithm

       As q_lc, this uses a label correcting strategy, which allows
       for negative edge weights. Unlike q_lc, this method does not
       perform negative cycle detection. If a negative cycle is
       present, ths algorithm will diverge. (TODO: fix this.)

       The worst case complexity of this algorithm is poor,
       O(min(n*m*C,m*2^n)), where C is the maximum edge cost.

       However, note that this algorithm has good average case
       performance on sparse graphs.
     **/

    template<class G>
    pair<vector<typename G::edge_type::weight_type>,
         vector<typename G::node_type> >
    dq_lc(const G& g,
          typename G::node_type source_node)
    {
        using edge_type = typename G::edge_type;
        using node_type = typename G::node_type;
        using weight_type = typename edge_type::weight_type;

        vector<weight_type> costs(g.node_count(), numeric_limits<weight_type>::max());
        vector<node_type> parents(g.node_count(), numeric_limits<node_type>::max());

        // a deque to hold our nodes-to-examine
        deque<node_type> dq;
        vector<bool> in_dq(g.node_count(), false);

        // has this node been seen?
        vector<bool> seen(g.node_count(), false);
        
        costs[source_node] = 0;
        dq.push_back(source_node);
        in_dq[source_node] = true;
        seen[source_node] = true;
        
        while(!dq.empty()) {
            node_type n = dq.front();
            dq.pop_front();
            in_dq[n] = false;
            for (auto e : g[n]) {
                weight_type candidate_cost = costs[n] + e.weight();
                // if this edge is better, use that
                if (costs[e.target()] > candidate_cost) {
                    costs[e.target()] = candidate_cost;
                    parents[e.target()] = n;
                    // e.target() has improved, so recheck its children
                    if (!in_dq[e.target()]) {
                        if (seen[e.target()]) {
                            // nodes we have already seen go up front,
                            // as their children are likely in-queue
                            dq.push_front(e.target());
                            in_dq[e.target()] = true;
                        } else {
                            // new nodes can wait
                            dq.push_back(e.target());
                            in_dq[e.target()] = true;
                            seen[e.target()] = true;
                        }
                    }
                }
            }
        }
        return make_pair(costs, parents);
    }


    
}

#endif
