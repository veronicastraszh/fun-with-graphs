#include<iostream>

using namespace std;

#ifndef EDGE_MAIN_H
#define EDGE_MAIN_H 1

namespace graph {
  
  // Different kinds of edges
  
  struct Edge;

  class Hash_Edge {
    const int shift = 32; // TODO make this a param
    size_t operator()(const Edge& e) { return e.source << shift + e.target; }
  }

  struct Edge {
    using hash_type = Hash_Edge;
    int source;
    int target;
  };
  
  ostream& operator<<(ostream& os, Edge e);

  Edge reverse_edge(Edge e);


  struct Weighted_Edge;

  class Hash_Weighted_Edge {
    const int shift = 32; // TODO make this a param
    size_t operator()(const Weighted_Edge& e) { return (e.source << shift + e.target) << shift + e.weight; }
  }

  struct Weighted_Edge {
    using hash_type = Hash_Weighted_Edge;
    int source;
    int target;
    long weight;
  };
  
  ostream& operator<<(ostream& os, Weighted_Edge e);

  Weighted_Edge reverse_edge(Weighted_Edge e);

}

class Hash_Weighted_Edge

#endif
