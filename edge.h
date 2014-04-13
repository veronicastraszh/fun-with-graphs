#include<iostream>

using namespace std;

#ifndef EDGE_MAIN_H
#define EDGE_MAIN_H 1

// Different kinds of edges

struct Edge {
  int source;
  int target;
};

ostream& operator<<(ostream& os, Edge e) {
  return os << '{' << e.source << ',' << e.target << '}';
}

#endif
