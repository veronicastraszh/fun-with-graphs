// Creating random graphs
// by Veronica Straszheim

#ifndef RANDOM_GRAPHS_H
#define RANDOM_GRAPHS_H

#include <random>
#include <cmath>

using namespace std;

namespace graph {

    /**
       RANDOM DENSE-ISH GRAPHS
    **/

    /**
       rnd-epsilon-dense - random epsilon-dense graph

       This generates a random dense-ish graph, where the probability
       of an edge between nodes i and j are each set by the call. If
       the probability is set to 1.0, this generates a complete
       graph. If set to a low value, it generates a nearly sparse
       graph.

       R edge_generator is a funciton that takes two arguments, s and
       t, and returns a new edge.
     **/

    template<class G, class R>
    G rnd_epsilon_dense(typename G::node_type node_count,
                        double probability,
                        R edge_generator,
                        unsigned int seed = 5675)
    {
        default_random_engine generator(seed);
        bernoulli_distribution distrub(probability);
        G result;
        for(int s = 0; s < node_count; ++s) {
            for(int t = 0; t < node_count; ++t) {
                if (distrub(generator)) {
                    result += edge_generator(s,t);
                }
            }
        }
        return result;
    }

    
    /**
       RANDOM SPARSE-ISH GRAPHS
    **/

    /**
       rnd_2d_space - random graph with clustered edges

       This algorithm creates a graph where edges tend to pick nodes
       near one other, according to a fake 2d grid.

       height_width is the size of the grid; the resulting graph with
       be this value squared

       base_probability is the chance that an edge will be created
       between two nodes at distance d.

       slope is how quickly these probabilities fall off

       edge_generator is a funciton that takes two arguments, s and
       t, and returns a new edge 
     **/

    template<class G, class R>
    G rnd_2d_space(typename G::node_type height_width,
                   double base_probability,
                   double slope,
                   R edge_generator,
                   unsigned int seed = 4343)
    {
        using node_type = typename G::node_type;
        G result;
        default_random_engine generator(seed);
        uniform_real_distribution distrub(0,1);
        for (node_type i = 0; i < height_width; i++) {
            for (node_type j = 0; j < height_width; j++) {
                for (node_type k = 0; k < heigth_width; k++) {
                    for (node_type l = 0; l < height_width; l++) {
                        node_type dist = max(abs(i-k),abs(j-l));
                        node_type count = 4 * dist;
                        double denom = dist * count * slope;
                        double prob = base_probability / denom;
                        if (distrub(generator) < prob) {
                            node_type s = i * height_width + j;
                            node_type t = k * height_width + l;
                            result += edge_generator(s,t);
                        }
                    }
                }
            }
        }
        return result;
    }
                   
       
    
}

#endif
