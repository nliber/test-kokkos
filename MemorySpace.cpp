#include <Kokkos_Core.hpp>
#include <cassert>
#include <iostream>

#include "pretty_name.h"

namespace {
template <typename T, typename = void>
struct has_name : std::false_type {};

template <typename T>
struct has_name<T, std::void_t<decltype(std::declval<T>().name())>>
    : std::is_same<const char*, decltype(std::declval<T>().name())> {};

template <typename T, typename = void>
struct has_allocate : std::false_type {};

template <typename T>
struct has_allocate<T,
                    std::void_t<decltype(std::declval<T>.allocate(size_t()))>>
    : std::is_same<void*, decltype(std::declval<T>.allocate(size_t()))> {};

template <typename T, typename = void>
struct has_deallocate : std::false_type {};

template<typename T>
    struct has_deallocate<T,
    std::void_t<decltype(std::declval<T>().deallocate(static_cast<void*>(nullptr), ptrdiff_t()))>>
        : std::true_type {};

}  // namespace

int main() {
    using MemorySpace = Kokkos::Experimental::SYCL;
    // using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    // using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;

    Kokkos::ScopeGuard _;

    std::cout << "MemorySpace: " << cool::make_pretty_name<MemorySpace>()
              << '\n';

    // Conceptual requirements from
    // <https://github.com/kokkos/kokkos/wiki/Kokkos-Concepts>
    std::cout << "is_copy_constructible: "
              << std::is_copy_constructible<MemorySpace>() << " == 1\n";
    std::cout << "is_default_constructible: "
              << std::is_default_constructible<MemorySpace>() << " == 1\n";
    std::cout << "is_destructible: " << std::is_destructible<MemorySpace>()
              << " == 1 \n";

    using memory_space = MemorySpace::memory_space;
    std::cout << "is_same<MemorySpace, MemorySpace::memory_space>: "
              << std::is_same<MemorySpace, memory_space>() << " == 1 ("
              << cool::make_pretty_name<MemorySpace>()
              << " == " << cool::make_pretty_name<memory_space>() << ")\n";

    using execution_space = MemorySpace::execution_space;
    std::cout << "is_execution_space<MemorySpace::execution_space>: "
              << Kokkos::is_execution_space<execution_space>::value << " == 1 ("
              << cool::make_pretty_name<execution_space>() << ")\n";

    std::cout << "device_type: "
              << cool::make_pretty_name<MemorySpace::device_type>() << '\n';

    std::cout << "has const char* name(): " << has_name<MemorySpace>()
              << " == 1\n";
    std::cout << "has void* allocate(size_t): " << has_allocate<MemorySpace>()
              << " == 1\n";
    std::cout << "has deallocate(void*, ptrdiff_t): " << has_deallocate<MemorySpace>() << " ==1\n";
}
