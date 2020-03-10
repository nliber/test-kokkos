#include <Kokkos_Core.hpp>
#include <iostream>

int main()
{
    Kokkos::ScopeGuard _;
    Kokkos::View<int*, Kokkos::Experimental::SYCLHostUSMSpace> v("V", 10);
    for (int i = 0; i != 10; ++i)
        v[i] = i;

    for (int i = 0; i != 10; ++i)
        std::cout << v[i] << '\n';

    Kokkos::View<int*, Kokkos::Experimental::SYCLHostUSMSpace> w("W", 10);

    for (int i = 0; i != 10; ++i)
        std::cout << w[i] << '\n';

    Kokkos::deep_copy(w, v);

    for (int i = 0; i != 10; ++i)
        std::cout << w[i] << '\n';


}

