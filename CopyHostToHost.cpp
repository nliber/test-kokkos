#include <Kokkos_Core.hpp>
#include "pretty_name.h"
#include <iostream>
#include <memory>

template<typename... Ts>
void printView(Kokkos::View<int*, Ts...> const& v, std::ostream& os = std::cout) {
    os << __PRETTY_FUNCTION__ << '\n';
    Kokkos::View<int*, Kokkos::HostSpace> vv("VV", v.size());
    Kokkos::deep_copy(vv, v);
    for (size_t s = 0; s != vv.size(); ++s) os << vv[s] << '\n';
    os << '\n';
}

int main() {
    //using Space = Kokkos::HostSpace;
    using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    //using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;
    //using Space = Kokkos::Experimental::SYCLSharedUSMSpace;
    using memory_space = Space::memory_space;

    Kokkos::ScopeGuard _;
    std::cout << "Space: " << cool::make_pretty_name<Space>() << '\n';
    std::cout << "memory_space: " << cool::make_pretty_name<memory_space>() << '\n';

    using view_type = Kokkos::View<int*, Space>;

    //view_type v("V", 10);
    const cl::sycl::queue& queue = *Kokkos::Experimental::SYCL().impl_internal_space_instance()->m_queue;
    cl::sycl::usm_allocator<view_type, cl::sycl::usm::alloc::host> allocator(queue);
    std::vector<view_type, decltype(allocator)> vec(allocator);
    vec.reserve(1);
    vec.emplace_back("V", 10);
    view_type& v = vec.front();
    //view_type* pv = &v;


    printView(v);
#if 1
    Kokkos::parallel_for(
        10, KOKKOS_LAMBDA(const size_t i)
        //{ (*pv)[i] = i; });
        { v[i] = i; });
#else
    for (size_t s = 0; s != v.size(); ++s)
        v[s] = s;
#endif
    printView(v);

    Kokkos::View<int*, Space> w("W", v.size());
    printView(w);

    Kokkos::deep_copy(w, v);
    printView(w);
}

