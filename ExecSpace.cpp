#include <Kokkos_Core.hpp>
#include <iostream>
#include <cassert>

#include "pretty_name.h"

int main() {
    using Space = Kokkos::Experimental::SYCL;
    // using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    // using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;

    Kokkos::ScopeGuard _;

    Space space;
    std::cout << "Space: " << cool::pretty_name(space) << '\n';
    std::cout << "name: " << space.name() << '\n';
    std::cout << "in_parallel: " << space.in_parallel() << " == 0\n";
    std::cout << "concurrency: " << space.concurrency() << " > 0\n";
    std::cout << "print_configureation: ";
    space.print_configuration(std::cout);

    std::cout << "memory_space: "
              << cool::make_pretty_name<Space::memory_space>() << '\n';
    std::cout
        << "accessible: "
        << Kokkos::SpaceAccessibility<Space, Space::memory_space>::accessible
        << '\n';

    std::cout << "array_layout: "
              << cool::make_pretty_name<Space::array_layout>() << '\n';
    std::cout << "scratch_memory_space: "
              << cool::make_pretty_name<Space::scratch_memory_space>() << '\n';

    std::cout << "is_default_constructible: "
              << std::is_default_constructible<Space>() << '\n';
    std::cout << "is_copy_constructible: "
              << std::is_copy_constructible<Space>() << '\n';

    std::cout << "is_execution_space: "
              << Kokkos::is_execution_space<Space>::value << " == 1\n";
    std::cout << "is_space: " << Kokkos::is_space<Space>::value << '\n';

    bool passedTestIncrExecSpaceTypedef;
    {
        passedTestIncrExecSpaceTypedef =
            (!std::is_same<void, Space::memory_space>::value) &&
            std::is_same<Space, Space::execution_space>::value &&
            !std::is_same<void, Space::scratch_memory_space>::value &&
            !std::is_same<void, Space::array_layout>::value;
        std::cout << "TestIncrExecSpaceTypedef passed: " << passedTestIncrExecSpaceTypedef << " == 1\n";
    }

    bool passedTestIncrExecSpace;
    {
        using device_type = Space::device_type;
        using memory_space = device_type::memory_space;
        using execution_space = device_type::execution_space;

        std::cout << "space::device_type: "
                  << cool::make_pretty_name<device_type>() << '\n';
        std::cout << "device_type::memory_space: "
                  << cool::make_pretty_name<memory_space>() << '\n';
        std::cout << "device_type::execution_space: "
                  << cool::make_pretty_name<execution_space>() << '\n';

        passedTestIncrExecSpace =
            std::is_same<device_type,
                         Kokkos::Device<execution_space, memory_space>>::value;
        std::cout << "TestIncrExecSpace passed: " << passedTestIncrExecSpace << " == 1\n";
    }

    assert(passedTestIncrExecSpaceTypedef);
    assert(passedTestIncrExecSpace);
    auto concurrency = Space().concurrency();
    assert(concurrency > 0);
    int in_parallel = Space::in_parallel();
    assert(!in_parallel);
    assert(Kokkos::is_execution_space<Space>::value);

}

