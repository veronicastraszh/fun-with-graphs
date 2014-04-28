#include <tuple>
#include <string>
#include <iostream>

#include "graph.h"
#include "edge.h"
#include "shortest_paths.h"
#include "heaps.h"

#include "graph_utils.h"

using namespace std;
using namespace graph;
using namespace heaps;


template<class G>
pair<bool, string> verify_shortest_paths(const G& g,
                                         vector<typename G::edge_type::weight_type> weights,
                                         vector<typename G::node_type> parents)
{
    for (auto e : g) {
        // for each edge we will verify the shortest path conditions
        bool failure = false;
        if (parents[e.target()] == e.source()) {
            if (!(weights[e.source()] + e.weight() == weights[e.target()])) failure = true;
        } else {
            if (!(weights[e.source()] + e.weight() >= weights[e.target()])) failure = true;
        }
        if (failure) return make_pair(false, "Failure at edge " + string(e));
    }
    return make_pair(true, "");
}

template<class G, class S>
void verify_graph(string name, const G& g, const S& fun)
{
    vector<typename G::edge_type::weight_type> weights;
    vector<typename G::node_type> parents;
    tie (weights, parents) = fun(g, 0);
    bool success;
    string message;
    tie(success, message) = verify_shortest_paths(g, weights, parents);

    if (!success) {
        cout << name << " failed\n";
        print_graph(g);
        cout << "Weights ";
        for (auto w : weights) cout << w << ' ';
        cout << '\n';
        cout << "Parents ";
        for (auto p : parents) cout << p << ' ';
        cout << '\n';
        cout << message << '\n';
        exit(1);
    }
    cout << name << " passed\n";
}

using positive_graph_type = graph<weighted_edge<> >;

positive_graph_type positive_graph {{0,1,2},{0,2,8},
                                    {1,2,5},{1,3,3},
                                    {2,1,6},{2,4,0},
                                    {3,2,1},{3,4,7},{3,5,6},
                                    {4,3,4},
                                    {5,4,2}};

int main()
{
    cout << "Testing shortest path algorithms\n";
    auto f_dijkstra_dial = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dijkstra<positive_graph_type,dial_heap>(g,n);
    };
    verify_graph("Dijkstra (dial)", positive_graph, f_dijkstra_dial);
                                                                     
    auto f_dijkstra_radix = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dijkstra<positive_graph_type,radix_heap>(g,n);
    };
    verify_graph("Dijkstra (radix)", positive_graph, f_dijkstra_radix);

    auto f_q_lc = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return q_lc(g,n);
    };
    verify_graph("Queued label correcting", positive_graph, f_q_lc);
    
    auto f_dq_lc = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dq_lc(g,n);
    };
    verify_graph("Deque label correcting", positive_graph, f_dq_lc);

    // TODO test with negative paths and negative cycles
}
