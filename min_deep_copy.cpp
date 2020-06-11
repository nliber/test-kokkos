#include <Kokkos_Core.hpp>
#include <iostream>
#include "pretty_name.h"

volatile int dummy;
using execution_space = Kokkos::Experimental::SYCL;
using memory_space = execution_space::memory_space;
using view_type = Kokkos::View<int*>;

using PF = Kokkos::Impl::ParallelFor<
    Kokkos::Impl::ViewCopy<
        Kokkos::View<int *, ::Kokkos::LayoutLeft,
                     ::Kokkos::Device< ::Kokkos::Experimental::SYCL,
                                       ::Kokkos::AnonymousSpace>,
                     ::Kokkos::MemoryTraits<0> >,
        Kokkos::View<const int *, ::Kokkos::LayoutLeft,
                     ::Kokkos::Device< ::Kokkos::Experimental::SYCL,
                                       ::Kokkos::AnonymousSpace>,
                     ::Kokkos::MemoryTraits<0> >,
        Kokkos::LayoutLeft, Kokkos::Experimental::SYCL, 1, long, false>,
    Kokkos::RangePolicy< ::Kokkos::Experimental::SYCL,
                         ::Kokkos::IndexType<long> >,
    Kokkos::Experimental::SYCL>;

void view_test()
{
    std::cout << __PRETTY_FUNCTION__ << " Before view " << ++dummy << std::endl;
    Kokkos::View<int*> v("V", 1);
    std::cout << __PRETTY_FUNCTION__ << " After view " << ++dummy << std::endl;
}

void view_pf_test()
{
    std::cout << __PRETTY_FUNCTION__ << " Before view " << ++dummy << std::endl;
    view_type v("V", 1);
    std::cout << __PRETTY_FUNCTION__ << " After view " << ++dummy << std::endl;

    std::cout << __PRETTY_FUNCTION__ << " Before pf " << ++dummy << std::endl;
    using functor_type = PF::functor_type;
    using Policy = PF::Policy;
    PF pf(functor_type{v, v}, Policy{});
    std::cout << __PRETTY_FUNCTION__ << " After pf " << ++dummy << std::endl;
}

void view_pf_execute_test()
{
    std::cout << __PRETTY_FUNCTION__ << " Before view " << ++dummy << std::endl;
    view_type v("V", 1);
    std::cout << __PRETTY_FUNCTION__ << " After view " << ++dummy << std::endl;

    std::cout << __PRETTY_FUNCTION__ << " Before pf " << ++dummy << std::endl;
    using functor_type = PF::functor_type;
    using Policy = PF::Policy;
    PF pf(functor_type{v, v}, Policy{});
    std::cout << __PRETTY_FUNCTION__ << " After pf " << ++dummy << std::endl;

    std::cout << __PRETTY_FUNCTION__ << " Before execute " << ++dummy << std::endl;
    pf.execute();
    std::cout << __PRETTY_FUNCTION__ << " After execute " << ++dummy << std::endl;
}

int main() {
    Kokkos::ScopeGuard _;

    view_test();
    std::cout << std::endl;

    view_pf_test();
    std::cout << std::endl;

    view_pf_execute_test();
    std::cout << std::endl;
}

