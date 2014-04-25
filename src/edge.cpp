// Routines to support edges
// by Veronica Straszheim

#include "edge.h"

namespace graph {
    
    ostream& operator<<(ostream& os, Edge e) {
        return os << '{' << e.source << ',' << e.target << '}';
    }
    
    Edge reverse_edge(Edge e) {
        return Edge{e.target, e.source};
    }
    
    ostream& operator<<(ostream& os, Weighted_Edge e) {
        return os << '{' << e.source << ',' << e.target << "}(" << e.weight << ")";
    }
    
    Weighted_Edge reverse_edge(Weighted_Edge e) {
        return Weighted_Edge{e.target, e.source, e.weight};
    }
  
}

// end of file
