#include<iostream>
#include "graph.h"
#include "edge.h"

using namespace std;


int main() {
  Graph<Edge> graph {};
  graph += {0,2};
  graph += {1,3};
  cout << graph.node_count() << '\n';
  cout << graph.edge_count() << '\n';

  cout << graph[0].size() << ' ' << graph[0][0] << '\n';
  cout << graph[1].size() << ' ' << graph[1][0] << '\n';
  cout << graph[2].size() << '\n';

  Graph_Iterator<Edge> q = graph.begin(); // Graph_Iterator<Edge>(&graph, 0);
  Graph_Iterator<Edge> e = graph.end(); // Graph_Iterator<Edge>(&graph, graph.node_count());
  cout << *q << '\n';
  q++;
  cout << *q << '\n';
  ++q;
  cout << (q == e) << ' ' << (q != e) << '\n';
}
