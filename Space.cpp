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

template <typename T, typename = void>
struct has_allocate : std::false_type {};

template <typename T>
struct has_allocate<T,
                    std::void_t<decltype(std::declval<T>().allocate(size_t()))>>
    : std::is_same<decltype(std::declval<T>().allocate(size_t())), void*> {};

template <typename T, typename = void>
struct has_deallocate : std::false_type {};

template <typename T>
struct has_deallocate<T, std::void_t<decltype(std::declval<T>().deallocate(
                             static_cast<void*>(nullptr), ptrdiff_t()))>>
    : std::true_type {};

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
    auto executionSpacePn = cool::make_pretty_name<ExecutionSpace>();

    os << "ExecutionSpace: " << executionSpacePn << '\n';

    using execution_space = typename ExecutionSpace::execution_space;
    auto execution_space_pn = cool::make_pretty_name<execution_space>();

    os << "ExecutionSpace::execution_space: " << execution_space_pn << '\n';

    const bool isSameExecutionSpace =
        std::is_same<ExecutionSpace, execution_space>();
    os << "ExecutionSpace == ExecutionSpace::execution_space: "
       << isSameExecutionSpace << " == 1\n";

    using memory_space = typename ExecutionSpace::memory_space;
    auto memory_space_pn = cool::make_pretty_name<memory_space>();
    os << "ExecutionSpace::memory_space: " << memory_space_pn << '\n';

    using device_type = typename ExecutionSpace::device_type;
    auto device_type_pn = cool::make_pretty_name<device_type>();
    os << "ExecutionSpace::device_type: " << device_type_pn << '\n';

    using Device = Kokkos::Device<execution_space, memory_space>;
    bool isSameDevice = std::is_same<device_type, Device>();
    os << "ExecutionSpace::device_type == Device<executionSpace, "
          "memory_space>: "
       << isSameDevice << " == 1\n";

    using scratch_memory_space = typename ExecutionSpace::scratch_memory_space;
    auto scratch_memory_space_pn =
        cool::make_pretty_name<scratch_memory_space>();
    os << "ExecutionSpace::scratch_memory_space: " << scratch_memory_space_pn
       << '\n';

    using array_layout = typename ExecutionSpace::array_layout;
    auto array_layout_pn = cool::make_pretty_name<array_layout>();
    os << "ExecutionSpace::array_layout: " << array_layout_pn << '\n';

    bool isDefaultConstructible =
        std::is_default_constructible<ExecutionSpace>();
    os << "is default constructible: " << isDefaultConstructible << " == 1\n";

    bool isCopyConstructible = std::is_copy_constructible<ExecutionSpace>();
    os << "is copy constructible: " << isCopyConstructible << " == 1\n";

    bool hasName = has_name<ExecutionSpace>();
    os << "has name: " << hasName << " == 1\n";

    bool hasPrintConfigurationOstream =
        has_print_configuration_ostream<ExecutionSpace>();
    os << "has print_configuration(ostream&): " << hasPrintConfigurationOstream
       << " == 1\n";

    bool hasPrintConfigurationOstreamBool =
        has_print_configuration_ostream_bool<ExecutionSpace>();
    os << "has print_configuration(ostream&, bool): "
       << hasPrintConfigurationOstreamBool << " == 1\n";

    bool hasInParallel = has_in_parallel<ExecutionSpace>();
    os << "has in_parallel: " << hasInParallel << " == 1\n";

    bool hasFence = has_fence<ExecutionSpace>();
    os << "has fence: " << hasFence << " == 1 \n";

    return isSameExecutionSpace && isSameDevice && isDefaultConstructible &&
           isCopyConstructible && hasName && hasPrintConfigurationOstream &&
           hasPrintConfigurationOstreamBool && hasInParallel && hasFence;
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

