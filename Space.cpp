#include <Kokkos_Core.hpp>
#include <cassert>
#include <iostream>

#include "pretty_name.h"

namespace {
template <typename T, typename = void>
struct has_name : std::false_type {};

template <typename T>
struct has_name<
    T, std::void_t<decltype(std::declval<std::add_const_t<T>>().name())>>
    : std::is_same<decltype(std::declval<std::add_const_t<T>>().name()),
                   const char*> {};

template <typename, typename = void>
struct has_execution_space : std::false_type {};

template <typename T>
struct has_execution_space<T, std::void_t<typename T::execution_space>>
    : std::true_type {};

template <typename, typename = void>
struct has_memory_space : std::false_type {};

template <typename T>
struct has_memory_space<T, std::void_t<typename T::memory_space>>
    : std::true_type {};

template <typename, typename = void>
struct has_size_type : std::false_type {};

template <typename T>
struct has_size_type<T, std::void_t<typename T::size_type>> : std::true_type {};

template <typename, typename = void>
struct has_array_layout : std::false_type {};

template <typename T>
struct has_array_layout<T, std::void_t<typename T::array_layout>>
    : std::true_type {};

template <typename, typename = void>
struct has_scratch_memory_space : std::false_type {};

template <typename T>
struct has_scratch_memory_space<T,
                                std::void_t<typename T::scratch_memory_space>>
    : std::true_type {};

template <typename, typename = void>
struct has_device_type : std::false_type {};

template <typename T>
struct has_device_type<T, std::void_t<typename T::device_type>>
    : std::true_type {};

template <typename T, typename = void>
struct has_allocate : std::false_type {};

template <typename T>
struct has_allocate<T, std::void_t<decltype(std::declval<std::add_const_t<T>>()
                                                .allocate(size_t()))>>
    : std::is_same<decltype(
                       std::declval<std::add_const_t<T>>().allocate(size_t())),
                   void*> {};

template <typename T, typename = void>
struct has_deallocate : std::false_type {};

template <typename T>
struct has_deallocate<
    T, std::void_t<decltype(std::declval<std::add_const_t<T>>().deallocate(
           static_cast<void*>(nullptr), ptrdiff_t()))>> : std::true_type {};

template <typename T, typename = void>
struct has_print_configuration_ostream : std::false_type {};

template <typename T>
struct has_print_configuration_ostream<
    T, std::void_t<decltype(
           std::declval<std::add_const_t<T>>().print_configuration(
               std::declval<std::ostream&>()))>> : std::true_type {};

template <typename T, typename = void>
struct has_print_configuration_ostream_bool : std::false_type {};

template <typename T>
struct has_print_configuration_ostream_bool<
    T, std::void_t<decltype(
           std::declval<std::add_const_t<T>>().print_configuration(
               std::declval<std::ostream&>(), bool()))>> : std::true_type {};

template <typename T, typename = void>
struct has_in_parallel : std::false_type {};

template <typename T>
struct has_in_parallel<
    T, std::void_t<decltype(std::declval<std::add_const_t<T>>().in_parallel())>>
    : std::is_convertible<
          decltype(std::declval<std::add_const_t<T>>().in_parallel()), bool> {};

template <typename T, typename = void>
struct has_fence : std::false_type {};

template <typename T>
struct has_fence<
    T, std::void_t<decltype(std::declval<std::add_const_t<T>>().fence())>>
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

template <typename ExecutionSpace>
bool TestExecutionSpace(std::ostream& os = std::cout) {
    os << "ExecutionSpace: " << cool::make_pretty_name<ExecutionSpace>()
       << '\n';

    bool isCopyConstructible = std::is_copy_constructible<ExecutionSpace>();
    os << "is_copy_constructible<ExecutionSpace>: " << isCopyConstructible
       << '\n';

    bool isDefaultConstructible =
        std::is_default_constructible<ExecutionSpace>();
    os << "is_default_constructible<ExecutionSpace>: " << isDefaultConstructible
       << '\n';

    bool isDestructible = std::is_destructible<ExecutionSpace>();
    os << "is_destructible<ExecutionSpace>: " << isDestructible << '\n';

    bool hasExecutionSpace = has_execution_space<ExecutionSpace>();
    os << "has execution_space: " << hasExecutionSpace << '\n';

    bool isSameExecutionSpace = false;
    if (hasExecutionSpace) {
        using execution_space = typename ExecutionSpace::execution_space;
        os << "ExecutionSpace::execution_space: "
           << cool::make_pretty_name<execution_space>() << '\n';

        isSameExecutionSpace = std::is_same<ExecutionSpace, execution_space>();
        os << "is_same<ExecutionSpace, ExecutionSpace::executionSpace>: "
           << isSameExecutionSpace << '\n';
    }

    bool hasMemorySpace = has_memory_space<ExecutionSpace>();
    os << "has memory_space: " << hasMemorySpace << '\n';

    bool isMemorySpace = false;
    if (hasMemorySpace) {
        using memory_space = typename ExecutionSpace::memory_space;
        os << "ExecutionSpace::memory_space: "
           << cool::make_pretty_name<memory_space>() << '\n';

        isMemorySpace = Kokkos::is_memory_space<memory_space>::value;
        os << "is_memory_space<memory_space>: " << isMemorySpace << '\n';
    }

    bool hasSizeType = has_size_type<ExecutionSpace>();
    os << "has size_type: " << hasSizeType << '\n';

    bool isIntegralSizeType = false;
    if (hasSizeType) {
        using size_type = typename ExecutionSpace::size_type;
        os << "ExecutionSpace::size_type: "
           << cool::make_pretty_name<size_type>() << '\n';

        isIntegralSizeType = std::is_integral<size_type>();
        os << "is_integral<size_type>: " << isIntegralSizeType << '\n';
    }

    bool hasArrayLayout = has_array_layout<ExecutionSpace>();
    os << "has array_layout: " << hasArrayLayout << '\n';

    bool isArrayLayout = false;
    if (hasArrayLayout) {
        using array_layout = typename ExecutionSpace::array_layout;
        os << "ExecutionSpace::array_layout: "
           << cool::make_pretty_name<array_layout>() << '\n';

        isArrayLayout = Kokkos::is_array_layout<array_layout>::value;
        os << "is_array_layout<array_layout>: " << isArrayLayout << '\n';
    }

    bool hasScratchMemorySpace = has_scratch_memory_space<ExecutionSpace>();
    os << "has scratch_memory_space: " << hasScratchMemorySpace << '\n';

    bool isScratchMemorySpace = false;
    if (hasScratchMemorySpace) {
        using scratch_memory_space =
            typename ExecutionSpace::scratch_memory_space;
        os << "ExecutionSpace::scratch_memory_space: "
           << cool::make_pretty_name<scratch_memory_space>() << '\n';

        isScratchMemorySpace =
            Kokkos::is_memory_space<scratch_memory_space>::value;
        os << "is_memory_space<scratch_memory_space>: " << isScratchMemorySpace
           << '\n';
    }

    bool hasDeviceType = has_device_type<ExecutionSpace>();
    os << "has device_type: " << hasDeviceType << '\n';

    bool isSameDeviceType = false;
    if (hasDeviceType) {
        using device_type = typename ExecutionSpace::device_type;
        os << "ExecutionSpace::device_type: "
           << cool::make_pretty_name<device_type>() << '\n';

        if (hasMemorySpace) {
            using memory_space = typename ExecutionSpace::memory_space;
            using Device = Kokkos::Device<ExecutionSpace, memory_space>;
            isSameDeviceType = std::is_same<device_type, Device>();
        }

        os << "is_same<device_type, Device<ExecutionSpace, memory_space>: "
           << isSameDeviceType << '\n';
    }

    bool hasInParallel = has_in_parallel<ExecutionSpace>();
    os << "has (bool)in_parallel() const: " << hasInParallel << '\n';

    bool hasFence = has_fence<ExecutionSpace>();
    os << "has fence() const: " << hasFence << '\n';

    bool hasName = has_name<ExecutionSpace>();
    os << "has const char* name() const: " << hasName << '\n';

    bool hasPrintConfigurationOstream =
        has_print_configuration_ostream<ExecutionSpace>();
    os << "has print_configuration(ostream&) const: "
       << hasPrintConfigurationOstream << '\n';

    bool hasPrintConfigurationOstreamBool =
        has_print_configuration_ostream_bool<ExecutionSpace>();
    os << "has print_configuration(ostream&, bool) const: "
       << hasPrintConfigurationOstreamBool << '\n';

    bool isExecutionSpace = Kokkos::is_execution_space<ExecutionSpace>::value;
    os << "is_execution_space<ExecutionSpace>: " << isExecutionSpace << '\n';

    bool validExecutionSpace =
        isCopyConstructible && isDefaultConstructible && isDestructible &&
        hasExecutionSpace && isSameExecutionSpace && hasMemorySpace &&
        isMemorySpace && hasSizeType && isIntegralSizeType && hasArrayLayout &&
        isArrayLayout && hasScratchMemorySpace && isScratchMemorySpace &&
        hasDeviceType && isSameDeviceType && hasInParallel && hasFence &&
        hasName && hasPrintConfigurationOstream &&
        hasPrintConfigurationOstreamBool && isExecutionSpace;

    if (!validExecutionSpace) {
        os << "NOT VALID ExecutionSpace: "
           << cool::make_pretty_name<ExecutionSpace>() << '\n';
    }

    return validExecutionSpace;
}

}  // namespace

int main() {
    Kokkos::ScopeGuard _;

    bool isSYCLMemorySpace = TestMemorySpace<Kokkos::Experimental::SYCL>();
    std::cout << '\n';

    TestExecutionSpace<Kokkos::Experimental::SYCL>();
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

