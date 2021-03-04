#include <iostream>

#include "Kokkos_Core.hpp"

int main() {
    Kokkos::InitArguments args_kokkos;
    Kokkos::initialize(args_kokkos);
    {
        Kokkos::View<double*> vd("vd", 64);
        std::cout << vd.size() << '\n';
    }
    Kokkos::finalize();

    return 0;
}

