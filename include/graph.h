// A datatype for graphs
// by Veronica Straszheim

#ifndef GRAPH_MAIN_H
#define GRAPH_MAIN_H 1

#include<vector>
#include<algorithm>

using namespace std;


namespace graph {

    template<class G> class graph_iterator;
  
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
        
        graph() {};
        graph(const graph<edge_type>& g)
        {
            for (auto e : g) operator+=(e);
        }
        graph(graph<edge_type>&& g)
        {
            swap(edges, g.edges);
            swap(edges_n, g.edges_n);
        }
        graph(initializer_list<edge_type> es)
        {
            for (auto e : es) operator+=(e);
        }
        void operator=(const graph<edge_type>& g)
        {
            edges = g.edges;
            edges_n = g.edges_n;
        }
        void operator=(const graph<edge_type>&& g)
        {
            edges = vector<vector<edge_type>>(0);
            edges_n = 0;
            swap(edges, g.edges);
            swap(edges_n, g.edges_n);
        }
    
        node_type node_count() const { return static_cast<node_type>(edges.size()); }
        size_type edge_count() const { return edges_n; }
        
        vector<edge_type>& operator[](node_type node) { return edges[node]; }
        const vector<edge_type>& operator[](node_type node) const { return edges[node]; }
        
        graph_iterator<graph<E> > begin() const { return graph_iterator<graph<E> >{*this, 0}; }
        graph_iterator<graph<E> > end() const
        {
            return graph_iterator<graph<E> >{*this, static_cast<node_type>(edges.size()), 0};
        }
        
        graph& operator+=(edge_type edge);
        
    private:
        size_type edges_n {0};
        vector<vector<edge_type>> edges;
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
    
    
    // A graph iterator
    
    template<class G>
    class graph_iterator {
    public:
        using edge_type = typename G::edge_type;
        using node_type = typename edge_type::node_type;
        
        graph_iterator(const G& g, node_type n) : graph{g}, node{n}, offset{0} { correct_node(); };
        graph_iterator(const G& g, node_type n, node_type o) : graph{g}, node{n}, offset{o} {};
  
        const edge_type& operator*() { return graph[node][offset]; }
        const edge_type& operator*() const { return graph[node][offset]; }
        graph_iterator operator++(int) { auto old = *this; ++offset; correct_node(); return old; }
        graph_iterator& operator++() { ++offset; correct_node(); return *this; }
        
        bool operator==(const graph_iterator<G>& o) { return node == o.node && offset == o.offset; }
        bool operator!=(const graph_iterator<G>& o) { return !(*this).operator==(o); }
        
    private:
        const G& graph;
        node_type node;
        node_type offset;
        
        void correct_node() {
            while(node < graph.node_count() && offset == graph[node].size()) {
                offset = 0;
                node++;
            }
        }
    };
    
}
      
#endif

// end of file

