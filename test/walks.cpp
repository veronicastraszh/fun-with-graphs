#include <string>
#include <sstream>
#include <algorithm>

#include "graph.h"
#include "graph_algo.h"
#include "heaps.h"

#include "graph_utils.h"

using namespace std;
using namespace graph;
using namespace graph::heaps;


void test_scc() {
  Graph<Edge> g {{0,1}, {1,2}, {1,3},
                 {1,4},
                 {2,0},
                 {3,0}, {3,5}, {3,7},
                 {4,5},
                 {5,6},
                 {6,4},
                 {7,5}};
  using component = vector<typename Graph<Edge>::node_type>;
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

int main() {
  test_scc();
}
