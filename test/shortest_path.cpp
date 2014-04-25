#include "graph.h"
#include "graph_algo.h"
#include "heaps.h"

#include "graph_utils.h"

using namespace std;
using namespace graph;
using namespace graph::heaps;

template<class T, class J>
void print_results(pair<vector<T>, vector<J>> results) {
  for (typename vector<T>::size_type i = 0; i < results.first.size(); i++) {
    cout << "node " << i << " cost " << results.first[i] << " parent " << results.second[i] << '\n';
  }
}

Graph<Weighted_Edge> positive_graph {{0,1,2},{0,2,8},
                                     {1,2,5},{1,3,3},
                                     {2,1,6},{2,4,0},
                                     {3,2,1},{3,4,7},{3,5,6},
                                     {4,3,4},
                                     {5,4,2}};

void test_dijkstra()
{
   auto dial_results = dijkstra<Weighted_Edge, dial_heap>(positive_graph, 0);
   auto radix_results = dijkstra<Weighted_Edge, radix_heap>(positive_graph, 0);
   // TODO actually verify content
   if (dial_results != radix_results) {
     cout << "Dijkstra failed\nGraphs did not match\n";
     print_graph(positive_graph);
     cout << "Results with dial heap:\n";
     print_results(dial_results);
     cout << "Results with radix heap:\n";
     print_results(radix_results);
     exit(1);
   } else {
     cout << "Dijkstra passed\n";
   }
}

void test_lc()
{
    auto dial_results = dijkstra<Weighted_Edge, dial_heap>(positive_graph, 0);
    auto q_lc_results = q_lc<Weighted_Edge>(positive_graph, 0);
    auto dq_lc_results = dq_lc<Weighted_Edge>(positive_graph, 0);
    if (dial_results != q_lc_results) {
        cout << "q_lc returned bad results\n";
        print_graph(positive_graph);
        cout << "Results from Dijkstra:\n";
        print_results(dial_results);
        cout << "Results from q_lc:\n";
        print_results(q_lc_results);
        exit(1);
    }
    if (q_lc_results != dq_lc_results) {
        cout << "dq_lc returned bad results\n";
        print_graph(positive_graph);
        cout << "Results from q_lc:\n";
        print_results(q_lc_results);
        cout << "Results from dq_lc\n";
        print_results(dq_lc_results);
        exit(1);
    }
    cout << "Label correcting algorithms passed\n";
}

int main()
{
    cout << "Testing shortest path algorithms\n";
    test_dijkstra();
    test_lc();
    // TODO test with negative paths and negative cycles
}
