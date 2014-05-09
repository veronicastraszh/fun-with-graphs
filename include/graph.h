// A datatype for graphs
// by Veronica Straszheim

#ifndef GRAPH_MAIN_H
#define GRAPH_MAIN_H 1

#include<vector>
#include<list>
#include<algorithm>

using namespace std;


namespace graph {

    template<class G> class const_graph_iterator;
  
    /**
       A Graph.
       
       The template parameter E should be some edge type, from
       edges.h. For a basic graph, any type with members named
       source() and target() will do.
       
       Edges are added to a graph using the += operator.
       
       Edges are looked up using the [] operator, where the node id is
       provided.
       
       A graph supports iteration, which provides all edges.
    **/
  
    template<class E>
    class graph {
    public:
        using edge_type = E;
        using size_type = typename vector<edge_type>::size_type;
        using node_type = typename E::node_type;
        using list_type = list<edge_type>;
        
        graph() {};
        graph(initializer_list<edge_type> es)
        {
            for (auto e : es) operator+=(e);
        }
    
        node_type node_count() const { return static_cast<node_type>(edges.size()); }
        size_type edge_count() const { return edges_n; }
        
        const list_type& operator[](node_type node) { return edges[node]; }
        const list_type& operator[](node_type node) const { return edges[node]; }
        
        const_graph_iterator<graph<E> > begin() const { return const_graph_iterator<graph<E> >{*this, 0}; }
        const_graph_iterator<graph<E> > end() const { return const_graph_iterator<graph<E> >{*this}; }
        
        graph& operator+=(edge_type edge);
        
    private:
        size_type edges_n {0};
        vector<list_type> edges;
    };
    
    template<class E>
    graph<E>& graph<E>::operator+=(E e)
    {
        node_type max_vertex = max(e.source(), e.target());
        if (max_vertex >= edges.size()) {
            edges.resize(max_vertex+1);
        }
        edges[e.source()].push_back(e);
        edges_n += 1;
        return *this;
    }


    /**
       COMPUTE REVERSE OF A GRAPH
    **/

    /**
       reverse - the reverse of the graph

       This is stimple g with the directions of the edges reversed
    **/
    
    template<class G>
    G reverse(const G& g)
    {
        using edge_type = typename G::edge_type;
        G result{};
        for (edge_type e : g) {
            result += reverse_edge(e);
        }
        return result;
    }
    
    /**
       GRAPH ITERATOR
    **/

    template<class G>
    class const_graph_iterator {
    public:
        using edge_type = typename G::edge_type;
        using node_type = typename edge_type::node_type;
        using list_type = typename G::list_type;
        using iter_type = typename list_type::const_iterator;

        const_graph_iterator(const G& g_) : g{g_}, n{g_.node_count()} { enter_node(); }
        const_graph_iterator(const G& g_, node_type start) : g{g_}, n{start} { enter_node(); }

        const edge_type& operator*() const { return *c; }
        const_graph_iterator operator++(int) { auto old = *this; ++c; correct_node(); return old; }
        const_graph_iterator& operator++() { ++c; correct_node(); return *this; }

        bool operator==(const const_graph_iterator<G>& o) { return c == o.c; }
        bool operator!=(const const_graph_iterator<G>& o) { return !(*this).operator==(o); }

    private:
        const G& g;
        node_type n;
        iter_type c;

        void enter_node();
        void correct_node();
    };

    template<class G>
    void const_graph_iterator<G>::enter_node()
    {
        while (n < g.node_count() && g[n].size() == 0) ++n;
        c = g[n].begin();
    }

    template<class G>
    void const_graph_iterator<G>::correct_node()
    {
        if (c == g[n].end()) {
            ++n;
            enter_node();
        }
    }
    
}
      
#endif

// end of file

