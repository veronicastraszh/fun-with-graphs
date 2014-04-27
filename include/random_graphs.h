// Creating random graphs
// by Veronica Straszheim

#ifndef RANDOM_GRAPHS_H
#define RANDOM_GRAPHS_H

#include <random>

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

       R edge_generator is a funciton that takes two arguments, i and
       j, and returns a new edge.
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
}

#endif
