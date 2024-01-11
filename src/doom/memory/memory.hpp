#pragma once
#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "z_zone.h"
#include "w_wad.h"

template <typename T, int tag>
struct z_allocator {
    using value_type = T;
    using is_always_equal = std::true_type;

    z_allocator() = default;

    template <typename U>
    z_allocator(const z_allocator<U, tag>) noexcept {}

    template <typename U>
    struct rebind { using other = z_allocator<U, tag>; };

    T *allocate(std::size_t n) {
        return static_cast<T*>(Z_Malloc(n * sizeof(T), tag, nullptr));
    }
    void deallocate(T *p, std::size_t n) {
        //Z_Free(p);
    }
};

template <typename T, typename U, int tag>
bool operator==(const z_allocator<T, tag> &a, const z_allocator<U, tag> &b) {
    return true;
}

template <typename T, typename U, int tag>
bool operator!=(const z_allocator<T, tag> &a, const z_allocator<U, tag> &b) {
    return false;
}

template <typename T>
using z_level_vector = std::vector<T, z_allocator<T, PU_LEVEL>>;

template <typename T>
using z_static_vector = std::vector<T, z_allocator<T, PU_STATIC>>;

template <typename T, typename Alloc>
void vector_clear(std::vector<T, Alloc> &v) {
    std::vector<T, Alloc>().swap(v);
}
template <typename T>
struct zone_free {
    void operator()(T *ptr) {}
};

template <typename T>
using z_unique_ptr = std::unique_ptr<T, zone_free<T>>;

template <typename T>
auto make_z_ptr(int size, int tag, void *user) {
    return z_unique_ptr<T>(Z_Malloc(size, tag, user));
}

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

template <typename T>
struct FreeDeleter {
    void operator()(T *p) {
        free(p);
    }
};

using str_ptr = std::unique_ptr<char, FreeDeleter<char>>;

#endif //_MEMORY_HPP_