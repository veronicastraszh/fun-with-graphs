// Algorithms based on walks over the graph
// by Veronica Straszheim

#ifndef WALKS_H
#define WALKS_H

#include <vector>
#include <functional>
#include <limits>

using namespace std;

namespace graph {
    
    /**
       DEPTH FIRST WALK
    **/
    
    enum class edge_class { tree, forward, back, cross };
    
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
    
    template<class G>
    class dfw {
    public:
        using edge_type = typename G::edge_type;
        using node_type = typename G::node_type;
        using tick_type = unsigned long;
        const node_type token_node = numeric_limits<node_type>::max();
        const tick_type token_tick = numeric_limits<tick_type>::max();
        
        vector<node_type> parents;
        vector<tick_type> entered;
        vector<tick_type> exited;
        function<void(node_type)> pre{[](node_type n) {}};
        function<void(node_type)> post{[](node_type n) {}};
        function<void(edge_type)> edge{[](edge_type e) {}};
        bool finished{false};
        bool directed{true};
        
        dfw(const G& g) :
            parents{vector<node_type>(g.node_count(), token_node)},
            entered{vector<tick_type>(g.node_count(), token_tick)},
            exited{vector<tick_type>(g.node_count(), token_tick)} {};
        
        void mark_discovered(node_type n) { entered[n] = ticks++; }
        bool discovered(node_type n) const { return entered[n] != token_tick; };
        
        void mark_processed(node_type n) { exited[n] = ticks++; };
        bool processed(node_type n) const { return exited[n] != token_tick; };
        
        bool entered_before(node_type n, node_type m) const { return entered[n] < entered[m]; }
        bool exited_before(node_type n, node_type m) const { return exited[n] < exited[m]; }
        
        edge_class classify_edge(edge_type e) const;
        
        // main operation
        void operator()(const G& g, node_type n);
        
    private:
        tick_type ticks{0};    
    };
    
    // the main walk
    template<class G>
    void dfw<G>::operator()(const G& g, node_type n) 
    {
        if (finished) return;
        mark_discovered(n);
        pre(n);
        for (auto e : g[n]) {
            if (!discovered(e.target())) {
                parents[e.target()] = n;
                edge(e);
                (*this)(g, e.target());
            } else if (directed || !processed(e.target())) {
                edge(e);
            }
            if (finished) return;
        }
        post(n);
        mark_processed(n);
    }
    
    // edge classification
    template<class G>
    edge_class dfw<G>::classify_edge(typename G::edge_type e) const
    {
        if (parents[e.target()] == e.source()) return edge_class::tree;
        if (discovered(e.target()) && !processed(e.target())) return edge_class::back;
        if (processed(e.target()) && entered_before(e.source(), e.target())) return edge_class::forward;
        if (processed(e.target()) && entered_before(e.target(), e.source())) return edge_class::cross;
        throw logic_error{"Unclassified edge"};    
    }
    
    
    /**
       TOPOLOGICAL SORT
    **/
    
    /**
       top_sort - topological sort
       
       This returns the list in _reverse_ of sorted order
    **/

    template<class E>
    class cycle_found : public logic_error {
    public:
        using edge_type = E;
        using parents_type = vector<typename edge_type::node_type>;
        edge_type edge;
        parents_type parents;
        cycle_found(edge_type e, parents_type p, string s) : logic_error{s}, edge(e), parents{p} {};
    };
    
    template<class G>
    vector<typename G::node_type> top_sort(const G& g)
    {
        using edge_type = typename G::edge_type;
        using node_type = typename G::node_type;
        vector<node_type> results;
        dfw<G> walk{g};
        walk.post = [&](node_type n) { results.push_back(n); };
        walk.edge = [&](edge_type e) {
            if (walk.classify_edge(e) == edge_class::back) {
                throw cycle_found<edge_type>{e, walk.parents, "Cycle found"};
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
       single strongly connected component of g.
       
       g is a graph
       d is the reverse of the graph, computed using reverse(g)
    **/
    
    template<class G>
    vector<vector<typename G::node_type>> scc(const G& g, const G& r)
    {
        using node_type = typename G::node_type;
        
        // find completion times in primary graph
        vector<node_type> finish_times;
        dfw<G> walk1{g};
        walk1.post = [&](int node) { finish_times.push_back(node); };
        for (node_type n = 0; n < g.node_count(); n++) {
            if (!walk1.processed(n)) walk1(g,n);
        }
        
        // collect components in reverse graph
        vector<vector<node_type>> result;
        vector<node_type> current;
        dfw<G> walk2{r};
        walk2.pre = [&](int node) { current.push_back(node); };
        for (auto n = finish_times.rbegin(); n != finish_times.rend(); n++) {
            if (!walk2.processed(*n)) {
                walk2(r, *n);
                result.push_back(move(current));
                current = vector<node_type>{};
            }
        }
        return result;
    }

}


#endif

