// Datatpes for edges
// by Veronica Straszheim

#ifndef EDGE_MAIN_H
#define EDGE_MAIN_H 1

#include<iostream>

using namespace std;

namespace graph {


  /**
     A BASIC EDGE
  **/
  
  struct Edge {
    int source;
    int target;
  };
  
  ostream& operator<<(ostream& os, Edge e);

  Edge reverse_edge(Edge e);

  
  /**
     A WEIGHTED EDGE
  **/

  struct Weighted_Edge {
    int source;
    int target;
    long weight;
  };
  
  ostream& operator<<(ostream& os, Weighted_Edge e);

  Weighted_Edge reverse_edge(Weighted_Edge e);

}

#endif

// end of file
