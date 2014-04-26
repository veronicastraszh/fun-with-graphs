
#ifndef GRAPH_UTIL
#define GRAPH_UTIL 1

#include<iostream>

using namespace std;
using namespace graph;

template<class G>
void print_graph(const G& g) {
  for (auto edge : g) {
    cout << edge << '\n';
  }
}

#endif
