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
                    std::void_t<decltype(std::declval<T>().allocate(size_t()))>>
    : std::is_same<void*, decltype(std::declval<T>().allocate(size_t()))> {};

template <typename T, typename = void>
struct has_deallocate : std::false_type {};

template <typename T>
struct has_deallocate<T, std::void_t<decltype(std::declval<T>().deallocate(
                             static_cast<void*>(nullptr), ptrdiff_t()))>>
    : std::true_type {};

template <typename MemorySpace>
bool TestMemorySpace(std::ostream& os = std::cout) {
    os << "MemorySpace: " << cool::make_pretty_name<MemorySpace>() << '\n';

    // Conceptual requirements from
    // <https://github.com/kokkos/kokkos/wiki/Kokkos-Concepts>
    os << "is_copy_constructible: " << std::is_copy_constructible<MemorySpace>()
       << " == 1\n";
    os << "is_default_constructible: "
       << std::is_default_constructible<MemorySpace>() << " == 1\n";
    os << "is_destructible: " << std::is_destructible<MemorySpace>()
       << " == 1 \n";

    using memory_space = typename MemorySpace::memory_space;
    os << "is_same<MemorySpace, MemorySpace::memory_space>: "
       << std::is_same<MemorySpace, memory_space>() << " == 1 ("
       << cool::make_pretty_name<MemorySpace>()
       << " == " << cool::make_pretty_name<memory_space>() << ")\n";

    using execution_space = typename MemorySpace::execution_space;
    os << "is_execution_space<MemorySpace::execution_space>: "
       << Kokkos::is_execution_space<execution_space>::value << " == 1 ("
       << cool::make_pretty_name<execution_space>() << ")\n";

    using device_type = typename MemorySpace::device_type;
    os << "device_type: " << cool::make_pretty_name<device_type>() << '\n';

    os << "has const char* name(): " << has_name<MemorySpace>() << " == 1\n";
    os << "has void* allocate(size_t): " << has_allocate<MemorySpace>()
       << " == 1\n";
    os << "has deallocate(void*, ptrdiff_t): " << has_deallocate<MemorySpace>()
       << " == 1\n";

    os << "is_memory_space: " << Kokkos::is_memory_space<MemorySpace>::value
       << " == 1\n";

    return std::is_copy_constructible<MemorySpace>() &&
           std::is_default_constructible<MemorySpace>() &&
           Kokkos::is_execution_space<execution_space>::value &&
           has_name<MemorySpace>() && has_allocate<MemorySpace>() &&
           has_deallocate<MemorySpace>() &&
           Kokkos::is_memory_space<MemorySpace>::value;
}

}  // namespace

int main() {
    Kokkos::ScopeGuard _;

    bool isSYCLMemorySpace = TestMemorySpace<Kokkos::Experimental::SYCL>();
    std::cout << '\n';

    bool isSYCLHostUSMSpaceMemorySpace =
        TestMemorySpace<Kokkos::Experimental::SYCLHostUSMSpace>();
    std::cout << '\n';

    bool isSYCLDeviceUSMSpaceMemorySpace =
        TestMemorySpace<Kokkos::Experimental::SYCLDeviceUSMSpace>();
    std::cout << '\n';

    bool isSYCLSharedUSMSpaceMemorySpace =
        TestMemorySpace<Kokkos::Experimental::SYCLSharedUSMSpace>();
    std::cout << '\n';

    assert(!isSYCLMemorySpace);
    assert(isSYCLHostUSMSpaceMemorySpace);
    assert(isSYCLDeviceUSMSpaceMemorySpace);
    assert(isSYCLSharedUSMSpaceMemorySpace);

    return 0;
}

