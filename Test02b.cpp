#include <Kokkos_Core.hpp>
#include <iostream>

using MemSpace = Kokkos::Experimental::SYCLHostUSMSpace;

void testit_free()
{
    const int N = 100000;
    const int M = 100000;

    for (int i = 0; i < N; ++i) {
            double *data = (double *)Kokkos::kokkos_malloc<MemSpace>(
          "data", M * sizeof(double));
      assert(data);
      Kokkos::kokkos_free<MemSpace>(data);
    }
};

void testit_malloc() {
    const int num_elements = 10;
    int *data = (int *)Kokkos::kokkos_malloc<MemSpace>(
        "data", num_elements * sizeof(int));
    assert(data);
    Kokkos::kokkos_free<MemSpace>(data);
}

int main() {
    Kokkos::ScopeGuard _;
    testit_free();
}

