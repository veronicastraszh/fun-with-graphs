// Some priority heaps for Dijkstra
// by Veronica Straszheim

#ifndef HEAPS_MAIN_H
#define HEAPS_MAIN_H

#include <vector>
#include <list>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

namespace graph {
    
    namespace heaps {
        
        /**
           These heaps are designed to work with the Dijkstra shortest
           path algoritm, and thus can follow certain constraints, namely
           these:
           
           1. delete_min() must always be preceded by a find_min(), which
           is to say, there should be no intervening modifications to
           the queue.
           
           2. Once an element is found by delete_min(), no element will be
           added with a key of less than the minimum found. This means
           as follows: if we call find_min(), and the heap returns an
           element whose key was 15, no element will ever be added with
           a key of less than 15.

           3. The key size of new elements added to the heap are
           limited as follows: no new element will have a weight
           greater than the current best-in-heap plus the maximum edge
           cost.
           
           This second constraint follows from the fact that shorter
           paths are always discovered before longer paths and that
           negative cost edges are disallowed. The third constraint
           follows from the way Dijkstra fans out over the
           nodes. Since the algorithm searches the best-so-far nodes,
           the maximum under consideration at any one time is limited
           by the maximum edge cost. Together these facts allow our
           heaps to be optimized for a narrow range of values.
           
           Two heaps are provided:
           
           * A dial_heap
           
           * radix_heap
           
           dial_heap takes size equal to the largest edge weight in the
           graph (plus one). radix_heap takes size equal to ln(N * ME),
           where N is the node_count of the graph and ME is the maximum
           edge weight. Both heaps have a find_min and delete_min that
           run O(s), where s is their size.
           
           For dial_heal, insert and decrease_key are constant time. For
           radix_heap, they are both O(s).
           
           However, for both of the heaps, their average case time is
           quite a bit better.
           
           If the big-Oh size is acceptable, dial_heap should be prefered,
           as it has excellent constant time performance.
        **/
        
        
        /**
           DIAL HEAP
        **/
        
        template<class K, class T>
        class dial_heap {
        public:
            
            using key_type = K;
            using value_type = T;
            using size_type = key_type;
            using bucket_type = list<value_type>;
            using bucket_index_type = typename vector<bucket_type>::size_type;
            using location_type = typename bucket_type::iterator;
            
            dial_heap(size_type nodes, size_type max_weight) :
                buckets{vector<bucket_type>(max_weight + 1)},
                base{0},
                count{0} {
#pragma unused(nodes)
                }
            
            location_type insert(key_type k, value_type t);
            void decrease_key(location_type loc, key_type old_k, key_type new_k);
            
            value_type find_min();
            void delete_min(); // must follow call to find_min();
            size_type size() const { return count; }
            bool empty() const { return count == 0; }
            
        private:
            vector<bucket_type> buckets;
            bucket_index_type base;
            size_type count;
            
            bucket_index_type get_index(key_type k) { return k % buckets.size(); }
            void rebase();
        };
        
        template<class K, class T>
        typename dial_heap<K,T>::location_type dial_heap<K,T>::insert(key_type k, value_type t) {
            if (k < base) throw out_of_range{"dial heap, key too small"};
            if (k >= base+buckets.size()) throw out_of_range{"dial_heap, key to large"};
            bucket_index_type index = get_index(k);
            buckets[index].push_front(t);
            count++;
            return buckets[index].begin();
        }
        
        template<class K, class T>
        void dial_heap<K,T>::decrease_key(location_type loc,
                                          key_type old_k,
                                          key_type new_k) {
            if (new_k < base) throw out_of_range{"dial heap, key decreased too low"};
            if (new_k > old_k) throw out_of_range{"dial heap, attempted key increase"};
            bucket_index_type old_index = get_index(old_k);
            bucket_index_type new_index = get_index(new_k);
            buckets[new_index].splice(buckets[new_index].begin(), buckets[old_index], loc);
        }
        
        template<class K, class T>
        void dial_heap<K,T>::rebase() {
            for(key_type c = 0; c < buckets.size(); c++) {
                bucket_index_type i = get_index(base + c);
                if (!buckets[i].empty()) {
                    base = i;
                    return;
                }
            }
            throw out_of_range{"dial_heap empty"};
        }
        
        template<class K, class T>
        typename dial_heap<K,T>::value_type dial_heap<K,T>::find_min() {
            rebase();
            return buckets[base].front();
        }
        
        template<class K, class T>
        void dial_heap<K,T>::delete_min() {
            buckets[base].pop_front();
            count -= 1;
        }
        
        
        /**
           RADIX HEAP
        **/
        
        template<class K, class T>
        class radix_heap {
        public:
            
            using key_type = K;
            using value_type = T;
            using size_type = key_type;
            using elem = pair<key_type, value_type>;
            using bucket_type = list<elem>;
            using bucket_index_type = typename vector<bucket_type>::size_type;
            using location_type = typename bucket_type::iterator;
            
            radix_heap(size_type nodes, size_type max_weight);
            
            location_type insert(key_type k, value_type t);
            void decrease_key(location_type loc, key_type old_k, key_type new_k);
            
            value_type find_min();
            void delete_min(); // must follow call to find_min();
            size_type size() const { return count; }
            bool empty() const { return count == 0; }
            
        private:
            
            vector<bucket_type> buckets;
            vector<key_type> ranges;
            size_type count;
            
            bucket_index_type find_bucket(key_type e);
            void new_ranges(key_type start, key_type end);
            bucket_index_type first_occupied();
        };
        
        template<class K, class T>
        radix_heap<K,T>::radix_heap(size_type nodes, size_type max_weight) : buckets{}, ranges{}, count{0} {
            size_type size = nodes * max_weight;
            size_type depth = 2;
            size_type t_size = size;
            while (t_size >>= 1) ++depth;
            ranges.resize(depth);
            buckets.resize(depth);
            new_ranges(0, size);
            count = 0;
        }
        
        template<class K, class T>
        typename radix_heap<K,T>::bucket_index_type
        radix_heap<K,T>::find_bucket(key_type e) {
            for (bucket_index_type i = 0; i < ranges.size(); i++) {
                if (ranges[i] >= e) return i;
            }
            throw length_error{"beyond end of radix heap"};
        }
        
        template<class K, class T>
        void radix_heap<K,T>::new_ranges(key_type start, key_type end) {
            ranges[0] = start;
            key_type i = start;
            bucket_index_type width = 1;
            bucket_index_type bucket = 1;
            for (;;) {
                i += width;
                if (i >= end) {
                    ranges[bucket] = end;
                    return;
                }
                ranges[bucket] = i;
                width *= 2;
                bucket++;
            }
        }
        
        template<class K, class T>
        typename radix_heap<K,T>::bucket_index_type
        radix_heap<K,T>::first_occupied() {
            for (bucket_index_type i = 0; i < buckets.size(); i++) {
                if (!buckets[i].empty()) return i;
            }
            throw out_of_range{"radix heap empty"};
        }
        
        template<class K, class T>
        typename radix_heap<K,T>::location_type radix_heap<K,T>::insert(key_type k, value_type t) {
            if (k < ranges.front()) throw out_of_range{"radix heap, key too small"};
            if (k > ranges.back()) throw out_of_range{"radix heap, key too large"};
            bucket_index_type bucket = find_bucket(k);
            buckets[bucket].push_front(make_pair(k,t));
            count++;
            return buckets[bucket].begin();
        }
        
        template<class K, class T>
        void radix_heap<K,T>::decrease_key(location_type loc,
                                           key_type old_k,
                                           key_type new_k) {
            if (new_k < ranges.front()) throw out_of_range{"radix heap, decrease key too low"};
            if (new_k > old_k) throw out_of_range{"radix heap, attempted to increase key"};
            loc->first = new_k;
            bucket_index_type old_bucket = find_bucket(old_k);
            bucket_index_type new_bucket = find_bucket(new_k);
            if (old_bucket != new_bucket) {
                buckets[new_bucket].splice(buckets[new_bucket].begin(), buckets[old_bucket], loc);
            }
        }
        
        template<class K, class T>
        typename radix_heap<K,T>::value_type radix_heap<K,T>::find_min() {
            bucket_index_type f = first_occupied();
            if (f >= 2) {
                // reshuffle elements to get minimum up front
                bucket_type remove;
                swap(remove, buckets[f]);
                key_type min = numeric_limits<key_type>::max();
                for (elem e : remove) if (e.first < min) min = e.first;
                new_ranges(min, ranges[f]);
                vector<location_type> locs;
                for (location_type loc = remove.begin(); loc != remove.end(); loc++) locs.push_back(loc);
                for (location_type loc : locs) {
                    bucket_index_type bucket = find_bucket(loc->first);
                    buckets[bucket].splice(buckets[bucket].begin(), remove, loc);
                }
                f = 0;
            }
            return buckets[f].front().second;
        }
        
        template<class K, class T>
        void radix_heap<K,T>::delete_min() {
            bucket_index_type f = first_occupied();
            buckets[f].pop_front();
            count -= 1;
        }
        
    }
    
}

#endif

// end of file
