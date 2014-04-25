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
        using node_type = unsigned int;
        node_type source;
        node_type target;
    };
    
    ostream& operator<<(ostream& os, Edge e);
    
    Edge reverse_edge(Edge e);
    
    
    /**
       A WEIGHTED EDGE
    **/
    
    struct Weighted_Edge {
        using node_type = unsigned int;
        using weight_type = unsigned long;
        node_type source;
        node_type target;
        weight_type weight;
    };
    
    ostream& operator<<(ostream& os, Weighted_Edge e);
    
    Weighted_Edge reverse_edge(Weighted_Edge e);
    
}

#endif

// end of file
