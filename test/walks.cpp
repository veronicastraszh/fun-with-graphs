#include <string>
#include <algorithm>

#include "graph.h"
#include "graph_algo.h"

#include "graph_utils.h"

using namespace std;
using namespace graph;

using basic_graph_type = graph<edge<>>;

void test_top_sort()
{
    basic_graph_type g {{0,2},{0,12},
                       {1,4},{1,2},{1,8},
                       {2,7},
                       {3,8},{3,13},{3,6},
                       {4,7},
                       {5,0},{5,4},{5,11},
                       {8,0},{8,9},{8,13},
                       {9,11},{9,10},
                       {10,6},
                       {12,9},
                       {13,0}};
    using node_type = basic_graph_type::node_type;
    vector<node_type> results = top_sort(g);
    
    // here we do a series of walks to ensure we do not find any errors
    for (auto i = results.begin(); i != results.end(); i++) {
        dfw<basic_graph_type> walk{g};
        walk.pre = [&](node_type n) {
            if (find(i+1,results.end(),n) != results.end()) {
                cout << "Topological Sort Failed!\n";
                cout << n << " found in list after " << *i << '\n';
                print_graph(g);
                for (auto n : results) cout << n << ' ';
                cout << '\n';
                exit(1);
            }
        };
        walk(g,*i);
    }
    cout << "Topological Sort passed\n";
}

void test_top_sort_cycle()
{
    basic_graph_type g {{0,1},{1,2},{1,3},{2,4},{3,5},{5,1}};
    using edge_type = basic_graph_type::edge_type;
    using node_type = basic_graph_type::node_type;
    try {
        vector<node_type> result = top_sort(g);
    } catch (cycle_found<edge_type>) {
        cout << "Topological Sort Cycle Detection passed\n";
        return;
    }
    cout << "Topolgical Sort failed cycle detection\n";
    exit(1);
}

void test_scc()
{
    basic_graph_type g {{0,1}, {1,2}, {1,3},
                       {1,4},
                       {2,0},
                       {3,0}, {3,5}, {3,7},
                       {4,5},
                       {5,6},
                       {6,4},
                       {7,5}};
    using component = vector<basic_graph_type::node_type>;
    using result_type = vector<component>;
    result_type result = scc(g, dual(g));
    result_type expected{{ 0, 1, 2, 3 },
                         { 7 },
                         { 4, 5, 6 }};
    auto same = [](component a, component b) { return is_permutation(a.begin(), a.end(), b.begin()); };
    if ( !is_permutation(result.begin(), result.end(), expected.begin(), same) ) {
        auto print = [](result_type r) {
            for (auto c : r) {
                for (auto n : c) cout << n << ' ';
                cout << '\n';
            }
        };
        cout << "SCC Failed!\n";
        print_graph(g);
        cout << "Found:\n";
        print(result);
        cout << "\nExpected:\n";
        print(expected);
        cout << '\n';
        exit(1);
    } else {
        cout << "SCC passed\n";
    }
}

int main()
{
    cout << "Testing walks\n";
    test_top_sort();
    test_top_sort_cycle();
    test_scc();
}
