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

void test_dfw() {
  using node = Edge::node_type;
  Graph<Edge> g{{0,2},
                {1,0},
                {2,1},
                {3,1},{3,2},{3,4},
                {4,5},{4,7},
                {5,6},
                {6,5},
                {7,3},
                {8,7},
                {8,9},
		{9,7}};
  ostringstream walk;
  auto pre = [&](node x) { walk << "enter " << x << ' '; };
  auto post = [&](node x) { walk << "exit " << x << ' '; };
  auto edge = [&](Edge e) { walk << "edge " << e << ' '; };
  vector<bool> visited(g.node_count());
  for(typename vector<bool>::size_type i = 0; i < visited.size(); i++) {
    if (!visited[i]) dfw(g,i,visited,pre,post,edge);
  }
  string expected{
 "enter 0 edge {0,2} enter 2 edge {2,1} enter 1 edge {1,0} exit 1 exit 2 exit 0 enter 3 edge {3,4} "
 "enter 4 edge {4,7} enter 7 edge {7,3} exit 7 edge {4,5} enter 5 edge {5,6} enter 6 edge {6,5} "
 "exit 6 exit 5 exit 4 edge {3,2} edge {3,1} exit 3 enter 8 edge {8,9} enter 9 edge {9,7} exit 9 "
 "edge {8,7} exit 8 "};
  if (walk.str() != expected) {
    print_graph(g);
    cout << "Found:    " << walk.str() << '\n';
    cout << "Expected: " << expected << '\n';
    exit(1);
  } else {
    cout << "DFW succeeded\n";
  }
}

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
  result_type result = scc(g);
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
    print_graph(g);
    cout << "Expected:\n";
    print(result);
    cout << "\nFound:\n";
    print(expected);
    cout << '\n';
    exit(1);
  } else {
    cout << "SCC succeeded\n";
  }
}

int main() {
  test_dfw();
  test_scc();
}
