#ifndef SRAM_H
#define SRAM_H

#include "./error_and_warning.h"
#include <string>
#include <list>
#include <utility>  // for std::pair

/*
    @brief  和 DUT 的 SRAM 类似，用于存储键值对
*/
template <typename Key, typename Value> class sram
{
  private:
    limited_list<std::pair<Key, Value>> kv_list;
    std::string sram_name;
    unsigned int maximum_capacity;

  public:
    explicit sram(const std::string &sram_name = "unnamed sram", unsigned int maximum_capacity = 32)
        : sram_name(sram_name), maximum_capacity(maximum_capacity)
    {
        if (maximum_capacity == 0) {
            CPP_ERROR("sram maximum_capacity must be greater than 0");
        }
    }

    ~sram() = default;

    void insert(const Key &key, const Value &value)
    {
        if (kv_list.size() >= maximum_capacity) {
            kv_list.pop_front();  // 淘汰最旧
        }
        kv_list.push_back(std::make_pair(key, value));  // 添加最新
    }

    bool find(const Key &key, Value &out_value) const
    {
        for (auto it = kv_list.rbegin(); it != kv_list.rend(); ++it) {
            if (it->first == key) {
                out_value = it->second;
                return true;
            }
        }
        return false;
    }

  private:  // Disabled
    bool fetch(const Key &key, Value &out_value)
    {
        bool found = false;

        for (auto it = kv_list.rbegin(); it != kv_list.rend(); ++it) {
            if (it->first == key) {
                out_value = it->second;
                found = true;
                break;
            }
        }

        if (!found)
            return false;

        for (auto it = kv_list.begin(); it != kv_list.end();) {
            if (it->first == key) {
                it = kv_list.erase(it);
            } else {
                ++it;
            }
        }

        return true;
    }
};

#endif
