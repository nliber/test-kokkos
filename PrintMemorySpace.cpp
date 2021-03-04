#include "pretty_name.h"
#include <Kokkos_Core.hpp>
#include <iostream>

int main() {
    using Space = Kokkos::HostSpace;

    Kokkos::ScopeGuard _;

    std::cout << "Space:\t\t\t" << cool::pretty_name((Space*)0) << '\n';
    std::cout << "memory_space:\t\t" << cool::pretty_name((Space::memory_space*)0) << '\n';
    std::cout << "execution_space:\t" << cool::pretty_name((Space::execution_space*)0) << '\n';
    std::cout << "device_type:\t\t" << cool::pretty_name((Space::device_type*)0) << '\n';

}

