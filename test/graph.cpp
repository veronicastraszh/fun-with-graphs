#include <algorithm>
#include <vector>

#include "graph.h"
#include "edge.h"

#include "graph_utils.h"

using namespace std;
using namespace graph;

using graph_type = graph<weighted_edge<> >;
using node_type = typename graph_type::node_type;
using weight_type = typename graph_type::edge_type::weight_type;

graph_type gr {{0,1,2},{0,2,8},
               {1,2,5},{1,3,3},
               {2,1,6},{2,4,0},
               {3,2,1},{3,4,7},{3,5,6},
               {4,3,4},
               {5,4,2}};

void test_graph_iterator(graph_type g,
                         vector<weight_type> expected_weights)
{
    vector<weight_type> weights;
    for (auto e: g) {
        weights.push_back(e.weight());
    }
    if(!is_permutation(weights.begin(), weights.end(), expected_weights.begin())) {
        cout << "Graph iteration failed\n";
        print_graph(g);
        for (auto w : weights) cout << w << ' ';
        cout << '\n';
        exit(1);
    }
    cout << "Graph iteration passed\n";
}

void test_edge_iteration(graph_type g,
                         typename graph_type::node_type n,
                         vector<weight_type> expected_weights)
{
    if (g[n].size() != expected_weights.size()) {
        cout << "Edge size failed for node " << n << '\n';
        exit(1);
    }
    vector<weight_type> weights;
    for (auto e: g[n]) {
        weights.push_back(e.weight());
    }
    if (!is_permutation(weights.begin(), weights.end(), expected_weights.begin())) {
        cout << "Edge iteration failed for node " << n << '\n';
        for (auto w: weights) cout << w << ' ';
        cout << '\n';
        exit(1);
    }
    cout << "Edge iteration passed\n";
}

void test_contains_edge(graph_type g,
                        vector<typename graph_type::location_type> present_locs,
                        vector<typename graph_type::location_type> absent_locs)
{
    for (auto loc : present_locs) {
        if (!g.contains_edge(loc)) {
            cout << "Test contains_egde failed on " << loc.first << ',' << loc.second << '\n';
            exit(1);
        }
    }
    for (auto loc : absent_locs) {
        if (g.contains_edge(loc)) {
            cout << "Test does not contains_egde failed on " << loc.first << ',' << loc.second << '\n';
            exit(1);
        }
    }
    cout << "Contains edge passed\n";
}

void test_delete_edges(graph_type g,
                       vector<typename graph_type::location_type> kill_edges,
                       vector<weight_type> expected_weights)
{
    for (auto k : kill_edges) {
        g.delete_edge(k);
    }
    test_graph_iterator(g, expected_weights);
    cout << "Delete edges passed\n";
}

void test_edges_at(graph_type g,
                   vector<typename graph_type::location_type> modify_edges,
                   weight_type new_value,
                   vector<weight_type> expected_weights)
{
    for (auto m : modify_edges) {
        g.edge_at(m).weight() = new_value;
    }
    test_graph_iterator(g, expected_weights);
    cout << "Edges at passed\n";
}

int main()
{
    cout << "Testing graph access and iteration\n";
    graph_type empty_graph;
    test_graph_iterator(empty_graph, {});
    test_graph_iterator(gr, {2,8,5,3,6,0,1,7,6,4,2});
    test_edge_iteration(gr, 0, {2, 8});
    test_edge_iteration(gr, 3, {1, 7, 6});
    test_edge_iteration(gr, 4, {4});
    test_contains_edge(gr, {{1,2},{2,1},{1,3}}, {{3,3},{0,3}});
    test_delete_edges(gr, {{1,2},{5,4},{1,3}}, {2,8,6,0,1,7,6,4});
    test_edges_at(gr, {{0,1},{5,4}}, 7, {7,8,5,3,6,0,1,7,6,4,7});
}

// End of file
