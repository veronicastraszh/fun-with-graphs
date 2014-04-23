
#ifndef GRAPH_UTIL
#define GRAPH_UTIL 1

#include<iostream>
#include "random_graph.h"

using namespace std;
using namespace graph;

template<class E>
void print_graph(const Graph<E>& g) {
  for (auto edge : g) {
    cout << edge << '\n';
  }
}

#endif
