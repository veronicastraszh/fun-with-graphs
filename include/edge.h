// Datatpes for edges
// by Veronica Straszheim

#ifndef EDGE_MAIN_H
#define EDGE_MAIN_H 1

#include<string>
#include<iostream>

using namespace std;

namespace graph {
    
    
    /**
       A BASIC EDGE
    **/

    template<class T=unsigned int>
    class edge {
    public:
        using node_type = T;

        edge(node_type source, node_type target) : s{source}, t{target} {}
        node_type source() const { return s; };
        node_type target() const { return t; };

        operator string() const { return "{" + to_string(s) + "," + to_string(t) + "}"; }
    private:
        node_type s;
        node_type t;
    };

    template<class T=unsigned int>
    ostream& operator<<(ostream& os, edge<T> e)
    {
        return os << string(e);
    }

    template<class T>
    edge<T> reverse_edge(edge<T> e)
    {
        return edge<T>{e.target(), e.source()};
    }

    template<class T>
    edge<T> merge_edges(typename edge<T>::node_type s,
                        typename edge<T>::node_type t,
                        edge<T> a,
                        edge<T> b)
    {
        return edge<T>{s, t};
    }
        
    
    /**
       A WEIGHTED EDGE
    **/

    template<class W=unsigned long, class T=unsigned int>
    struct weighted_edge {
    public:
        using node_type = T;
        using weight_type = W;

        weighted_edge(node_type source, node_type target, weight_type weight) :
            s{source}, t{target}, w{weight} {}
        node_type source() const { return s; };
        node_type target() const { return t; };
        weight_type& weight() { return w; };
        weight_type weight() const { return w; };

        operator string() const { return "{" + to_string(s) + "," + to_string(t) + "}(" + to_string(w) + ")"; }
    private:
        node_type s;
        node_type t;
        weight_type w;
    };


    template<class W=unsigned long, class T=unsigned int>
    ostream& operator<<(ostream& os, weighted_edge<W,T> e)
    {
        return os << string(e);
    }
    
    template<class W=unsigned long, class T=unsigned int>
    weighted_edge<W,T> reverse_edge(weighted_edge<W,T> e)
    {
        return weighted_edge<W,T>{e.target(), e.source(), e.weight()};
    }

    template<class W=unsigned long, class T=unsigned int>
    weighted_edge<W,T> merge_edges(typename weighted_edge<W,T>::nodetype s,
                                  typename weighted_edge<W,T>::nodetype t,
                                  weighted_edge<W,T> a,
                                  weighted_edge<W,T> b)
    {
        return weighted_edge<W,T>{s,t,a.weight() + b.weight()};
    }

}

#endif

// end of file
