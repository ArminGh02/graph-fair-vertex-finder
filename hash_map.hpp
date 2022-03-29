#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <algorithm>
#include <forward_list>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace ds {

namespace detail {

template <typename Map>
class HashMapIter {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = typename Map::value_type;

    HashMapIter(std::forward_list<value_type>* currentBucket,
                std::forward_list<value_type>* lastBucket,
                typename std::forward_list<value_type>::iterator it)
        : currentBucket_(currentBucket), lastBucket_(lastBucket), it_(it) {}
    ~HashMapIter() = default;

    friend bool operator==(const HashMapIter& lhs, const HashMapIter& rhs) { return lhs.it_ == rhs.it_; }

    friend bool operator!=(const HashMapIter& lhs, const HashMapIter& rhs) { return !(lhs == rhs); }

    value_type& operator*() const { return *it_; }

    typename std::forward_list<value_type>::iterator operator->() const { return it_; }

    HashMapIter& operator++() {
        if (++it_ == currentBucket_->end()) {
            if (currentBucket_ != lastBucket_) {
                do {
                    ++currentBucket_;
                } while (currentBucket_->empty() && currentBucket_ != lastBucket_);
            }
            if (currentBucket_->empty()) {
                it_ = currentBucket_->end();
            } else {
                it_ = currentBucket_->begin();
            }
        }
        return *this;
    }

    HashMapIter operator++(int) {
        HashMapIter temp(*this);
        ++(*this);
        return temp;
    }

private:
    std::forward_list<value_type>* currentBucket_;
    std::forward_list<value_type>* lastBucket_;
    typename std::forward_list<value_type>::iterator it_;
};

template <typename Map>
class HashMapConstIter {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = typename Map::value_type;

    HashMapConstIter(std::forward_list<value_type>* currentBucket,
                     std::forward_list<value_type>* lastBucket,
                     typename std::forward_list<value_type>::const_iterator it)
        : currentBucket_(currentBucket), lastBucket_(lastBucket), it_(it) {}
    ~HashMapConstIter() = default;

    friend bool operator==(const HashMapConstIter& lhs, const HashMapConstIter& rhs) { return lhs.it_ == rhs.it_; }

    friend bool operator!=(const HashMapConstIter& lhs, const HashMapConstIter& rhs) { return !(lhs == rhs); }

    const value_type& operator*() const { return *it_; }

    typename std::forward_list<value_type>::const_iterator operator->() const { return it_; }

    HashMapConstIter& operator++() {
        if (++it_ == currentBucket_->end()) {
            if (currentBucket_ != lastBucket_) {
                do {
                    ++currentBucket_;
                } while (currentBucket_->empty() && currentBucket_ != lastBucket_);
            }
            if (currentBucket_->empty()) {
                it_ = currentBucket_->end();
            } else {
                it_ = currentBucket_->begin();
            }
        }
        return *this;
    }

    HashMapConstIter operator++(int) {
        HashMapConstIter temp(*this);
        ++(*this);
        return temp;
    }

private:
    std::forward_list<value_type>* currentBucket_;
    std::forward_list<value_type>* lastBucket_;
    typename std::forward_list<value_type>::const_iterator it_;
};

}  // namespace detail

template <typename K, typename V, typename HashFunc = std::hash<K>>
class HashMap {
public:
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<K, V>;
    using size_type   = std::size_t;

    using iterator       = detail::HashMapIter<HashMap>;
    using const_iterator = detail::HashMapConstIter<HashMap>;

    HashMap()
        : hasher_(HashFunc()), bucketsCount_(1024), buckets_(new std::forward_list<value_type>[1024]()), size_(0) {}
    ~HashMap() { delete[] buckets_; }

    void insert(const key_type& key, const mapped_type& val) {
        if (find(key) != end()) {
            return;
        }
        buckets_[index(key)].emplace_front(key, val);
        ++size_;
    }

    iterator find(const key_type& key) {
        auto& bucket = buckets_[index(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (i->first == key) {
                return iterator(&bucket, &buckets_[bucketsCount_ - 1], i);
            }
        }
        return end();
    }

    const_iterator find(const key_type& key) const {
        auto& bucket = buckets_[index(key)];
        for (auto i = bucket.begin(); i != bucket.end(); ++i) {
            if (i->first == key) {
                return const_iterator(&bucket, &buckets_[bucketsCount_ - 1], i);
            }
        }
        return end();
    }

    mapped_type& operator[](const key_type& key) {
        auto& bucket = buckets_[index(key)];
        for (auto& pair : bucket) {
            if (key == pair.first) {
                return pair.second;
            }
        }
        bucket.emplace_front(key, value_type());
        ++size_;
        return bucket.front().second;
    }

    iterator begin() {
        if (empty()) {
            return end();
        }
        for (int i = 0; i < bucketsCount_; ++i) {
            if (!buckets_[i].empty()) {
                return iterator(&buckets_[i], &buckets_[bucketsCount_ - 1], buckets_[i].begin());
            }
        }
        throw std::runtime_error("invalid state in begin");
    }

    const_iterator begin() const {
        if (empty()) {
            return end();
        }
        for (int i = 0; i < bucketsCount_; ++i) {
            if (!buckets_[i].empty()) {
                return const_iterator(&buckets_[i], &buckets_[bucketsCount_ - 1], buckets_[i].begin());
            }
        }
        throw std::runtime_error("invalid state in begin");
    }

    iterator end() {
        return iterator(&buckets_[bucketsCount_ - 1], &buckets_[bucketsCount_ - 1], buckets_[bucketsCount_ - 1].end());
    }

    const_iterator end() const {
        return const_iterator(&buckets_[bucketsCount_ - 1],
                              &buckets_[bucketsCount_ - 1],
                              buckets_[bucketsCount_ - 1].end());
    }

    size_type size() const { return size_; }

    bool empty() const { return size_ == 0; }

private:
    HashFunc hasher_;
    size_type bucketsCount_;
    std::forward_list<value_type>* buckets_;
    size_type size_;

    size_type index(const key_type& key) const { return hasher_(key) & (bucketsCount_ - 1); }
};

}  // namespace ds

#endif  // HASH_MAP_HPP
