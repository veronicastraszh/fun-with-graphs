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

void test_dijkstra() {
   Graph<Weighted_Edge> g {{0,1,2},{0,2,8},
             	          {1,2,5},{1,3,3},
                          {2,1,6},{2,4,0},
			  {3,2,1},{3,4,7},{3,5,6},
			  {4,3,4},
			  {5,4,2}};
   auto dial_results = dijkstra<Weighted_Edge, dial_heap>(g, 0);
   auto radix_results = dijkstra<Weighted_Edge, radix_heap>(g, 0);
   if (dial_results != radix_results) {
     cerr << "Graphs did not match\n";
     print_graph(g);
     print_results(dial_results);
     print_results(radix_results);
     exit(1);
   } else {
     cout << "Dijkstra passed\n";
   }
}

int main() {
  test_dijkstra();
}
