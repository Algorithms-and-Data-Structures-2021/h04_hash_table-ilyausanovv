#include "hash_table.hpp"

#include <stdexcept>

namespace itis {

    int HashTable::hash(int key) const {
        return utils::hash(key, static_cast<int>(buckets_.size()));
    }

    HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor} {
        if (capacity <= 0) {
            throw std::logic_error("hash table capacity must be greater than zero");
        }

        if (load_factor <= 0.0 || load_factor > 1.0) {
            throw std::logic_error("hash table load factor must be in range [0...1]");
        }

        buckets_.resize(capacity);
    }

    std::optional<std::string> HashTable::Search(int key) const {
        int hashCode = hash(key);

        for(auto elem : buckets_[hashCode]){
            if(elem.first == key){
                return elem.second;
            }
        }

        return std::nullopt;
    }

    void HashTable::Put(int key, const std::string &value) {
        int hashCode = hash(key);

        if(ContainsKey(key)){
            for(auto i = buckets_[hashCode].begin(); i != buckets_[hashCode].end(); i++){
                if(i->first == key){
                    i->second = value;
                }
            }
        }

        if(!ContainsKey(key)){
            buckets_[hashCode].push_back({key, value});
            num_keys_++;
        }

        if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_) {
            std::vector<Bucket> newBuckets_(capacity() * kGrowthCoefficient);

            for(auto elem : buckets_){
                for(auto pair : elem){
                    newBuckets_[utils::hash(pair.first, newBuckets_.size())].push_back(pair);
                }
            }

            buckets_.clear();
            buckets_.shrink_to_fit();
            buckets_ = newBuckets_;
        }
    }

    std::optional<std::string> HashTable::Remove(int key) {
        int hashCode = hash(key);

        if(ContainsKey(key)){
            for(auto i = buckets_[hashCode].begin(); i != buckets_[hashCode].end(); i++){
                if(i->first == key){
                    auto deletedItem = i->second;
                    buckets_[hashCode].erase(i);
                    num_keys_--;

                    return deletedItem;
                }
            }

        }

        return std::nullopt;
    }

    bool HashTable::ContainsKey(int key) const {
        return Search(key).has_value();
    }

    bool HashTable::empty() const {
        return size() == 0;
    }

    int HashTable::size() const {
        return num_keys_;
    }

    int HashTable::capacity() const {
        return static_cast<int>(buckets_.size());
    }

    double HashTable::load_factor() const {
        return load_factor_;
    }

    std::unordered_set<int> HashTable::keys() const {
        std::unordered_set<int> keys(num_keys_);
        for (const auto &bucket : buckets_) {
            for (const auto &[key, _] : bucket) {
                keys.insert(key);
            }
        }
        return keys;
    }

    std::vector<std::string> HashTable::values() const {
        std::vector<std::string> values;
        for (const auto &bucket : buckets_) {
            for (const auto &[_, value] : bucket) {
                values.push_back(value);
            }
        }
        return values;
    }

}  // namespace itis