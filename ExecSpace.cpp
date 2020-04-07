#include <Kokkos_Core.hpp>
#include <iostream>

#include "pretty_name.h"

int main() {
    using Space = Kokkos::Experimental::SYCL;
    // using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    // using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;

    Kokkos::ScopeGuard _;

    Space space;
    std::cout << "name: " << space.name() << '\n';
    std::cout << "in_parallel: " << space.in_parallel() << '\n';
    std::cout << "print_configureation: ";
    space.print_configuration(std::cout);

    std::cout << "memory_space: " << cool::pretty_name((Space::memory_space*)0)
              << '\n';
    std::cout
        << "accessible: "
        << Kokkos::SpaceAccessibility<Space, Space::memory_space>::accessible
        << '\n';

    std::cout << "array_layout: " << cool::pretty_name((Space::array_layout*)0)
              << '\n';
    std::cout << "scratch_memory_space: "
              << cool::pretty_name((Space::scratch_memory_space*)0) << '\n';

    std::cout << "is_default_constructible: "
              << std::is_default_constructible<Space>() << '\n';
    std::cout << "is_copy_constructible: "
              << std::is_copy_constructible<Space>() << '\n';

    std::cout << "is_execution_space: "
              << Kokkos::is_execution_space<Space>::value << '\n';
    std::cout << "is_space: " << Kokkos::is_space<Space>::value << '\n';

    const bool passedTestIncrExecSpaceTypedef =
        (!std::is_same<void, Space::memory_space>::value) &&
        std::is_same<Space, Space::execution_space>::value &&
        !std::is_same<void, Space::scratch_memory_space>::value &&
        !std::is_same<void, Space::array_layout>::value;
    std::cout << "TestIncrExecSpaceTypedef: " << passedTestIncrExecSpaceTypedef << '\n';

    {
        using device_type = Space::device_type;
        using memory_space = device_type::memory_space;
        using execution_space = device_type::execution_space;

        const bool passed = std::is_same<device_type,
                     Kokkos::Device<execution_space, memory_space>>::value;
        std::cout << " TestIncrExecSpace passed: " << passed << '\n';
    }

}

