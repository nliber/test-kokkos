#include <Kokkos_Core.hpp>
#include <cassert>
#include <iostream>


int main() {
    Kokkos::ScopeGuard _;

    Kokkos::View<int*> v("V", 10);
    for (int i = 0; i != v.size(); ++i)
        v[i] = i;

    std::cout << "Sequential Kokkos::View:\n";
    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';
    std::cout << '\n';

    Kokkos::Experimental::SYCL syclExecutionSpace;
    cl::sycl::queue& q =
        *syclExecutionSpace.impl_internal_space_instance()->m_queue;

    // Test that we can send a view into a queue
    q.submit([&](cl::sycl::handler& h) {
            h.single_task([=]{
                    for (int i = 0; i != v.size(); ++i)
                        v[i] *= 2;
                    });
            });
    q.wait();

    std::cout << "x2 after SYCL single_task (Kokkos::View passed to kernel):\n";
    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';
    std::cout << '\n';

    // Test parallel_for
    Kokkos::parallel_for(
            10, KOKKOS_LAMBDA(const size_t i)
            { v[i] *= 3; });

    std::cout << "x6 after Kokkos::parallel_for:\n";
    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';
    std::cout << '\n';
}

