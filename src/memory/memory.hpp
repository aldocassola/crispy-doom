#pragma once
#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "z_zone.h"

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
struct FreeDeleter {
    void operator()(T *p) {
        free(p);
    }
};

using str_ptr = std::unique_ptr<char, FreeDeleter<char>>;

template <typename T>
auto zmalloc(int size, int tag, void *user) {
    return static_cast<T>(Z_Malloc(size, tag, user));
}

#endif //_MEMORY_HPP_