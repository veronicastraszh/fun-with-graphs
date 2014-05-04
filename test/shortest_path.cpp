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

template<class G, class S>
void fail_on_cycle(string name, const G& g, const S& fun) {
    try {
        fun(g,0);
    } catch (negative_cycle_found<typename G::node_type>) {
        cout << name << " passed\n";
        return;
    }
    cout << name << " failed, cycle not found\n";
    print_graph(g);
    exit(1);
}

using positive_graph_type = graph<weighted_edge<> >;

positive_graph_type positive_graph {{0,1,2},{0,2,8},
                                    {1,2,5},{1,3,3},
                                    {2,1,6},{2,4,0},
                                    {3,2,1},{3,4,7},{3,5,6},
                                    {4,3,4},
                                    {5,4,2}};

using fractional_graph_type = graph<weighted_edge<double> >;

fractional_graph_type fractional_graph {{0,1,2.4},{0,2,8.7},
                                        {1,2,5.2},{1,3,3.6},
                                        {2,1,6.666},{2,4,0},
                                        {3,2,1.9},{3,4,7.0002},{3,5,6.5},
                                        {4,3,4.1},
                                        {5,4,2.4}};


using negative_graph_type = graph<weighted_edge<long> >;

negative_graph_type negative_graph {{1,2,10},{1,3,15},
                                    {2,3,25},{2,4,0},{2,5,5},
                                    {3,2,-20},
                                    {4,5,-5},
                                    {5,4,10},{5,3,30},
                                    {0,1,0}};

negative_graph_type negative_graph_cycle {{1,2,10},{1,3,15},
                                          {2,3,25},{2,4,0},{2,5,5},
                                          {3,2,-20},
                                          {4,5,-5},{4,3,-5},
                                          {5,4,10},{5,3,30},
                                          {0,1,0}};


int main()
{
    cout << "Testing shortest path algorithms\n";
    auto f_dijkstra_dial = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dijkstra<positive_graph_type,dial_heap>(g,n);
    };
    auto f_dijkstra_radix = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dijkstra<positive_graph_type,radix_heap>(g,n);
    };
    auto f_dijkstra_pairing = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dijkstra<positive_graph_type,pairing_heap>(g,n);
    };
    auto f_dijkstra_pairing_f = [](const fractional_graph_type& g, fractional_graph_type::node_type n) {
        return dijkstra<fractional_graph_type,pairing_heap>(g,n);
    };
    auto f_q_lc = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return q_lc(g,n);
    };
    auto f_dq_lc = [](const positive_graph_type& g, positive_graph_type::node_type n) {
        return dq_lc(g,n);
    };
    auto f_q_lc_n = [](const negative_graph_type& g, negative_graph_type::node_type n) {
        return q_lc(g,n);
    };
    auto f_dq_lc_n = [](const negative_graph_type& g, negative_graph_type::node_type n) {
        return dq_lc(g,n);
    };

    verify_graph("Dijkstra (dial)", positive_graph, f_dijkstra_dial);
    verify_graph("Dijkstra (radix)", positive_graph, f_dijkstra_radix);
    verify_graph("Dijkstra (pairing)", positive_graph, f_dijkstra_pairing);
    verify_graph("Dijkstra (pairing), fractional", fractional_graph, f_dijkstra_pairing_f);
    verify_graph("Queued label correcting", positive_graph, f_q_lc);
    verify_graph("Deque label correcting", positive_graph, f_dq_lc);
    verify_graph("Queued label correcting, negative", negative_graph, f_q_lc_n);
    verify_graph("Deque label correcting, negative", negative_graph, f_dq_lc_n);
    fail_on_cycle("Queued label correcting, cycle", negative_graph_cycle, f_q_lc_n);

    // TODO test with negative paths and negative cycles
}
