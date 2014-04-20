// Some priority heaps for Dijkstra

#ifndef HEAPS_MAIN_H
#define HEAPS_MAIN_H

#include <vector>
#include <list>

using namespace std;

namespace graph {

  namespace heaps {

    template<class K, class T>
    class dial_heap {
    public:

      using key_type = K;
      using value_type = T;
      using location_type = typename list<value_type>::iterator;

      dial_heap(int size) : elems{vector<list<value_type>>(size)}, base{0}, count{0} {}

      location_type insert(key_type k, value_type t);
      location_type decrease_key(location_type loc, key_type old_k, key_type new_k);
      
      value_type find_min();
      void delete_min();
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
      value_type save = *loc;
      key_type old_index = get_index(old_k);
      elems[old_index].erase(loc);
      key_type new_index = get_index(new_k);
      elems[new_index].push_front(save);
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
    }
    
    template<class K, class T>
    typename dial_heap<K,T>::value_type dial_heap<K,T>::find_min() {
      rebase();
      return elems[base].front();
    }

    template<class K, class T>
    void dial_heap<K,T>::delete_min() {
      rebase();
      elems[base].pop_front();
      count -= 1;
    }

  }

}

#endif
