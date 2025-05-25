#ifndef LIMITED_LIST_H
#define LIMITED_LIST_H

#include "./error_and_warning.h"
#include <string>
#include <list>

/*
    @brief  目前：Cpp Model 由于不考虑时序，所以一个通道要输出若干 transfer，所以需要一个 list 来存储
            后续：像 DUT 一样考虑时序，一个通道只输出一个 transfer，所以将 list 容量限制为 1
*/
template <typename T> class limited_list
{
  private:
    limited_list<T> lst;
    std::string list_name;
    unsigned int maximum_capacity;

  public:
    explicit limited_list(const std::string &list_name = "unnamed sram", unsigned int maximum_capacity = 20)
        : list_name(list_name), maximum_capacity(maximum_capacity)
    {
        if (maximum_capacity == 0) {
            CPP_ERROR("list maximum_capacity must be greater than 0");
        }
    }
    ~limited_list() = default;

    void push_back(const T &value)
    {
        if (lst.size() >= maximum_capacity) {
            CPP_ERROR("list list_name : " + list_name + " -----> it is full");
            return;
        }
        lst.push_back(value);
    }
    void push_back(const limited_list<T> &value)
    {
        if (lst.size() + value.size() >= maximum_capacity) {
            CPP_ERROR("list list_name : " + list_name + " -----> it is full");
            return;
        }
        lst.splice(lst.end(), value);
    }
    void clear()
    {
        lst.clear();
    }
    typename limited_list<T>::iterator begin()
    {
        return lst.begin();
    }
    typename limited_list<T>::iterator end()
    {
        return lst.end();
    }
    typename limited_list<T>::const_iterator begin() const
    {
        return lst.begin();
    }
    typename limited_list<T>::const_iterator end() const
    {
        return lst.end();
    }
    typename limited_list<T>::const_iterator cbegin() const
    {
        return lst.cbegin();
    }
    typename limited_list<T>::const_iterator cend() const
    {
        return lst.cend();
    }
};

#endif
