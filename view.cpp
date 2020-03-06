#include <Kokkos_Core.hpp>
#include <iostream>

int main()
{
    Kokkos::ScopeGuard _;
    std::cout << Kokkos::is_execution_space<Kokkos::DefaultExecutionSpace>::value << '\n';
    std::cout << Kokkos::is_space<Kokkos::DefaultExecutionSpace>::value << '\n';
    std::cout << Kokkos::is_execution_space<Kokkos::Experimental::SYCL>::value << '\n';
    std::cout << Kokkos::is_space<Kokkos::Experimental::SYCL>::value << '\n';
    Kokkos::print_configuration(std::cout, true);
    Kokkos::DefaultExecutionSpace::print_configuration(std::cout);
    Kokkos::DefaultExecutionSpace::print_configuration(std::cout, true);
    Kokkos::View<int*, Kokkos::Experimental::SYCLHostUSMSpace> v("V", 1);
}

