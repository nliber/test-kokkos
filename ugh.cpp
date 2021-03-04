#include <Kokkos_Core.hpp>
#include <cassert>
#include <iostream>

#include "pretty_name.h"

int main() {
    Kokkos::ScopeGuard _;
    Kokkos::Impl::ViewMapping<Kokkos::ViewTraits<int*>, void> vm;
    Kokkos::Impl::ViewDimension<0> vd0;
    Kokkos::Impl::ViewDimension1<18446744073709551615u, 1> vd1;
    std::cout << std::is_trivially_copyable_v<decltype(vd0)> << '\n';
    std::cout << std::is_trivially_copyable_v<decltype(vd1)> << '\n';
    std::cout << std::is_trivially_copy_constructible_v<decltype(vm)> << '\n';
    std::cout << std::is_trivially_move_constructible_v<decltype(vm)> << '\n';
    std::cout << std::is_trivially_move_assignable_v<decltype(vm)> << '\n';
    std::cout << std::is_trivially_copy_assignable_v<decltype(vm)> << '\n';
    std::cout << std::is_trivially_destructible_v<decltype(vm)> << '\n';

    using view_type = Kokkos::View<int*>;
    std::cout << cool::pretty_type<view_type>() << '\n';
    std::cout << cool::pretty_type<view_type::execution_space>() << '\n';
    std::cout << cool::pretty_type<view_type::memory_space>() << '\n';


    Kokkos::View<int*> v("V", 10);
    for (int i = 0; i != v.size(); ++i)
        v[i] = i;
    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';

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

    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';

    // Test parallel_for
    Kokkos::parallel_for(
            10, KOKKOS_LAMBDA(const size_t i)
            { v[i] *= 3; });

    for (int i = 0; i != v.size(); ++i)
        std::cout << i << ' ' << v[i] << '\n';
}

