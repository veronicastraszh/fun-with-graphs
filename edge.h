#include<iostream>

using namespace std;

#ifndef EDGE_MAIN_H
#define EDGE_MAIN_H 1

namespace graph {
  
  // Different kinds of edges
  
  struct Edge {
    int source;
    int target;
  };
  
  ostream& operator<<(ostream& os, Edge e);

  Edge reverse_edge(Edge e);

  struct Weighted_Edge {
    int source;
    int target;
    long weight;
  };
  
  ostream& operator<<(ostream& os, Weighted_Edge e);

  Weighted_Edge reverse_edge(Weighted_Edge e);

}

#endif
