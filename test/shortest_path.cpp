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

using positive_graph_type = graph<weighted_edge<> >;

positive_graph_type positive_graph {{0,1,2},{0,2,8},
                                    {1,2,5},{1,3,3},
                                    {2,1,6},{2,4,0},
                                    {3,2,1},{3,4,7},{3,5,6},
                                    {4,3,4},
                                    {5,4,2}};

template<template<class,class> class H>
void test_dijkstra()
{
    vector<positive_graph_type::edge_type::weight_type> weights;
    vector<positive_graph_type::node_type> parents;
    tie (weights, parents) = dijkstra<positive_graph_type, H>(positive_graph, 0);
    bool success;
    string message;
    tie(success, message) = verify_shortest_paths(positive_graph, weights, parents);

    if (!success) {
        cout << "Dijkstra failed\n";
        print_graph(positive_graph);
        cout << "Weights ";
        for (auto w : weights) cout << w << ' ';
        cout << '\n';
        cout << "Parents ";
        for (auto p : parents) cout << p << ' ';
        cout << '\n';
        cout << message << '\n';
        exit(1);
    }
    cout << "Dijkstra passed\n";
}

void test_q_lc()
{
    vector<positive_graph_type::edge_type::weight_type> weights;
    vector<positive_graph_type::node_type> parents;
    tie (weights, parents) = q_lc(positive_graph, 0);
    bool success;
    string message;
    tie(success, message) = verify_shortest_paths(positive_graph, weights, parents);
    if (!success) {
        cout << "Queued label correcting algorithm failed\n";
        print_graph(positive_graph);
        cout << "Weights ";
        for (auto w : weights) cout << w << ' ';
        cout << '\n';
        cout << "Parents ";
        for (auto p : parents) cout << p << ' ';
        cout << '\n';
        cout << message << '\n';
        exit(1);
    }
    cout << "Queued label correcting algorithm passed\n";
}

void test_dq_lc()
{
    vector<positive_graph_type::edge_type::weight_type> weights;
    vector<positive_graph_type::node_type> parents;
    tie (weights, parents) = dq_lc(positive_graph, 0);
    bool success;
    string message;
    tie(success, message) = verify_shortest_paths(positive_graph, weights, parents);
    if (!success) {
        cout << "Deque label correcting algorithm failed\n";
        print_graph(positive_graph);
        cout << "Weights ";
        for (auto w : weights) cout << w << ' ';
        cout << '\n';
        cout << "Parents ";
        for (auto p : parents) cout << p << ' ';
        cout << '\n';
        cout << message << '\n';
        exit(1);
    }
    cout << "Deque label correcting algorithm passed\n";
}

int main()
{
    cout << "Testing shortest path algorithms\n";
    test_dijkstra<dial_heap>();
    test_dijkstra<radix_heap>();
    test_q_lc();
    test_dq_lc();
    // TODO test with negative paths and negative cycles
}
