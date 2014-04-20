// Some priority heaps for Dijkstra

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
      using col_type = list<value_type>;
      using location_type = list<value_type>::iterator;

      location_type insert(key_type k, value_type t);
      location_type decrease_key(location_type loc, key_type old_k, key_type new_k);
      
      const col_type find_min();
      void delete_min();
      bool empty() const { return count == 0; }

    private:
      vector<col_type> elems;
      key_type base;
      key_type count;

      key_type get_index(key_type k) { k % elems.size(); }
      void rebase();
    };

    template<class K, class T>
    col_type dial_heap<K,T>::insert(key_type k, value_type t) {
      key_type index = get_index(k);
      elems[index].push_front(t);
      count++;
      return elems[index].begin();
    }

    template<class K, class T>
    location_type dial_heap<K,T>::decrease_key(location_type loc, key_type old_k, key_type new_k) {
      value_type save = *loc;
      elems[get_index[old_k]].erase(loc);
      key_type new_index = get_index(k);
      elems[new_index].push_front(save);
      return elems[new_index].begin();
    }

    template<class K, classT>
    void dial_heap<K,T> rebase() {
      for(key_type c = 0; c < elems.size(); c++) {
	key_type i = get_index(base + c);
	if (!elems[i].empty()) {
	  base = i;
	  return;
	}
      }
    }
    
    template<class K, class T>
    const col_type dial_heap<K,T>::find_min() {
      rebase();
      return elems[base];
    }

    template<class K, class T>
    void dial_heap<K,T>::delete_min() {
      rebase();
      count -= elems[base].size();
      elems[base] = list<value_type>{};
    }

  }

}

