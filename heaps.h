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

       This second constraint follows from the fact that shorter paths
       are always discovered before longer paths.

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
      using location_type = typename list<value_type>::iterator;

      dial_heap(int nodes, int max_weight) : elems{vector<list<value_type>>(max_weight + 1)}, base{0}, count{0} {
#pragma unused(nodes)
      }

      location_type insert(key_type k, value_type t);
      location_type decrease_key(location_type loc, key_type old_k, key_type new_k);
      
      value_type find_min();
      void delete_min(); // must follow call to find_min();
      key_type size() const { return count; }
      bool empty() const { return count == 0; }

    private:
      vector<list<value_type>> elems;
      key_type base;
      key_type count;

      key_type get_index(key_type k) { return k % elems.size(); }
      void rebase();
    };

    template<class K, class T>
    typename dial_heap<K,T>::location_type dial_heap<K,T>::insert(key_type k, value_type t) {
      key_type index = get_index(k);
      elems[index].push_front(t);
      count++;
      return elems[index].begin();
    }

    template<class K, class T>
    typename dial_heap<K,T>::location_type dial_heap<K,T>::decrease_key(location_type loc,
									key_type old_k,
									key_type new_k) {
      key_type old_index = get_index(old_k);
      key_type new_index = get_index(new_k);
      elems[new_index].push_front(*loc);
      elems[old_index].erase(loc);
      return elems[new_index].begin();
    }

    template<class K, class T>
    void dial_heap<K,T>::rebase() {
      for(key_type c = 0; c < elems.size(); c++) {
	key_type i = get_index(base + c);
	if (!elems[i].empty()) {
	  base = i;
	  return;
	}
      }
      throw out_of_range{"dial_heap empty"};
    }
    
    template<class K, class T>
    typename dial_heap<K,T>::value_type dial_heap<K,T>::find_min() {
      rebase();
      return elems[base].front();
    }

    template<class K, class T>
    void dial_heap<K,T>::delete_min() {
      elems[base].pop_front();
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
      using elem = pair<key_type, value_type>;
      using location_type = typename list<elem>::iterator;

      radix_heap(int nodes, int max_weight);

      location_type insert(key_type k, value_type t);
      location_type decrease_key(location_type loc, key_type old_k, key_type new_k);
      
      value_type find_min();
      void delete_min(); // must follow call to find_min();
      key_type size() const { return count; }
      bool empty() const { return count == 0; }

    private:

      vector<list<elem>> buckets;
      vector<key_type> ranges;
      int count;

      int find_bucket(key_type e);
      void new_ranges(key_type start, key_type end);
      int first_occupied();
    };

    template<class K, class T>
    radix_heap<K,T>::radix_heap(int nodes, int max_weight) : buckets{}, ranges{}, count{0} {
      int size = nodes * max_weight;
      int depth = 2;
      int t_size = size;
      while (t_size >>= 1) ++depth;
      ranges.resize(depth);
      buckets.resize(depth);
      new_ranges(0, size);
      count = 0;
    }

    template<class K, class T>
    int radix_heap<K,T>::find_bucket(key_type e) {
      for (unsigned int i = 0; i < ranges.size(); i++) {
	if (ranges[i] >= e) return i;
      }
      throw length_error{"beyond end of radix heap"};
    }

    template<class K, class T>
    void radix_heap<K,T>::new_ranges(key_type start, key_type end) {
      ranges[0] = start;
      key_type i = start;
      int width = 1;
      int bucket = 1;
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
    int radix_heap<K,T>::first_occupied() {
      for (unsigned int i = 0; i < buckets.size(); i++) {
	if (!buckets[i].empty()) return i;
      }
      throw out_of_range{"radix heap empty"};
    }

    template<class K, class T>
    typename radix_heap<K,T>::location_type radix_heap<K,T>::insert(key_type k, value_type t) {
      int bucket = find_bucket(k);
      buckets[bucket].push_front(make_pair(k,t));
      count++;
      return buckets[bucket].begin();
    }

    template<class K, class T>
    typename radix_heap<K,T>::location_type radix_heap<K,T>::decrease_key(location_type loc,
									  key_type old_k,
									  key_type new_k) {
      loc->first = new_k;
      int old_bucket = find_bucket(old_k);
      int new_bucket = find_bucket(new_k);
      if (old_bucket != new_bucket) {
	buckets[new_bucket].push_front(*loc);
	buckets[old_bucket].erase(loc);
	return buckets[new_bucket].begin();
      } else {
	return loc;
      }
    }

    template<class K, class T>
    typename radix_heap<K,T>::value_type radix_heap<K,T>::find_min() {
      // reshuffle elements to get minimum up front
      int f = first_occupied();
      key_type min = numeric_limits<key_type>::max();
      list<elem> bucket;
      swap(bucket, buckets[f]);
      count -= bucket.size();
      for (elem e : bucket) if (e.first < min) min = e.first;
      new_ranges(min, ranges[f]);
      for (elem e : bucket) insert(e.first, e.second);
      return buckets[0].front().second;
    }

    template<class K, class T>
    void radix_heap<K,T>::delete_min() {
      buckets[0].pop_front();
      count -= 1;
    }

  }

}

#endif

// end of file
