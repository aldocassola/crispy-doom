#pragma once
#ifndef _LUMP_H_
#define _LUMP_H_

#include <string>
#include "memory/memory.hpp"
#include "w_wad.h"

template <typename T>
class lump_ptr {
    lumpindex_t lumpnum;
    std::size_t size_;
    z_unique_ptr<T> data;
public:
    using iterator = T *;
    using const_iterator = const T *;

    lump_ptr() = default;
    lump_ptr(const lump_ptr &) = delete;
    lump_ptr(lump_ptr &&other) : lump_ptr{} {
        std::swap(*this, other);
    }

    lump_ptr(lumpindex_t l, int tag) noexcept
        : lumpnum(l),
        size_(static_cast<std::size_t>(W_LumpLength(lumpnum))),
        data(static_cast<T *>(W_CacheLumpNum(lumpnum, tag))) {}

    lump_ptr(const std::string &name, int tag) noexcept
        : lumpnum(W_GetNumForName(name.c_str())),
        size_(static_cast<std::size_t>(W_LumpLength(lumpnum))),
        data(static_cast<T*>(W_CacheLumpNum(lumpnum, tag))) {}

    const T *get() const { return data; }
    T* get() { return data; }
    std::size_t size() { return size_; }
    T &operator[](std::size_t i) { return data.get()[i]; }
    const T &operator[](std::size_t i) const { return data.get[i]; }

    iterator begin() { return data.get(); }
    iterator end() { return data.get() + size() + 1; }

    const_iterator cbegin() const { return data.get(); }
    const_iterator end() const { return data.get() + size() + 1; }

    ~lump_ptr() {
        W_ReleaseLumpNum(lumpnum);
    }

    lump_ptr &operator=(const lump_ptr&) = delete;
    lump_ptr &operator=(lump_ptr &&other) {
        std::swap(*this, other);
    };
    bool operator!() { return data == nullptr; }
    operator bool() { return data != nullptr; }
};

template <typename T>
auto cache_lump_num(int lump, int tag) {
    return lump_ptr<T>(lump, tag);
}

template <typename T>
auto cache_lump_name(const std::string &name, int tag) {
    return lump_ptr<T>(name, tag);
}
#endif //_LUMP_H_