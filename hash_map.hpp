#include <initializer_list>
#include <utility>

namespace ds {

template <typename K, typename V, typename HashFunc>
class HashMap {
   public:
    HashMap(std::initializer_list<std::pair<K, V> list) {

    }

    V find(const K& key) const {

    }

    V& operator[](const K& key) {

    }

    bool empty() {

    }

   private:
    HashFunc hashFunc;
};

}  // namespace ds
