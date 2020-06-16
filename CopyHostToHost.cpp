#include <Kokkos_Core.hpp>
#include "pretty_name.h"
#include <iostream>
#include <memory>
#include "pretty_name.h"

namespace {
template <typename View>
void ViewTypedefs(std::ostream& os = std::cout) {
    os << "View: " << cool::pretty_type<View>() << '\n';

    os << "rank: " << +View::rank << '\n';
    os << "rank_dynamic: " << +View::rank_dynamic << '\n';
    os << "reference_type_is_lvalue_reference: "
       << +View::reference_type_is_lvalue_reference << '\n';

    os << "data_type: " << cool::pretty_type<typename View::data_type>()
       << '\n';
    os << "const_data_type: "
       << cool::pretty_type<typename View::const_data_type>() << '\n';
    os << "non_const_data_type: "
       << cool::pretty_type<typename View::non_const_data_type>() << '\n';
    os << "scalar_array_type: "
       << cool::pretty_type<typename View::scalar_array_type>() << '\n';
    os << "const_scalar_array_type: "
       << cool::pretty_type<typename View::const_scalar_array_type>() << '\n';
    os << "non_const_scalar_array_type: "
       << cool::pretty_type<typename View::non_const_scalar_array_type>()
       << '\n';

    os << "value_type: " << cool::pretty_type<typename View::value_type>()
       << '\n';
    os << "const_value_type: "
       << cool::pretty_type<typename View::const_value_type>() << '\n';
    os << "non_const_value_type: "
       << cool::pretty_type<typename View::non_const_value_type>() << '\n';

    os << "execution_space: "
       << cool::pretty_type<typename View::execution_space>() << '\n';
    os << "memory_space: " << cool::pretty_type<typename View::memory_space>()
       << '\n';
    os << "device_type: " << cool::pretty_type<typename View::device_type>()
       << '\n';
    os << "memory_traits: " << cool::pretty_type<typename View::memory_traits>()
       << '\n';
    os << "host_mirror_space: "
       << cool::pretty_type<typename View::host_mirror_space>() << '\n';

    os << "non_const_type: "
       << cool::pretty_type<typename View::non_const_type>() << '\n';
    os << "const_type: " << cool::pretty_type<typename View::const_type>()
       << '\n';
    os << "HostMirror: " << cool::pretty_type<typename View::HostMirror>()
       << '\n';

    os << "reference_type: "
       << cool::pretty_type<typename View::reference_type>() << '\n';
    os << "pointer_type: " << cool::pretty_type<typename View::pointer_type>()
       << '\n';

    os << "array_layout: " << cool::pretty_type<typename View::array_layout>()
       << '\n';
    os << "size_type: " << cool::pretty_type<typename View::size_type>()
       << '\n';
    os << "dimension: " << cool::pretty_type<typename View::dimension>()
       << '\n';
    os << "specialize: " << cool::pretty_type<typename View::specialize>()
       << '\n';
}
}  // namespace

template<typename... Ts>
void printView(Kokkos::View<int*, Ts...> const& v, std::ostream& os = std::cout) {
    os << __PRETTY_FUNCTION__ << '\n';
    //Kokkos::View<int*, Kokkos::HostSpace> vv("VV", v.size());
    Kokkos::View<int*> vv("VV", v.size());
    Kokkos::deep_copy(vv, v);
    for (size_t s = 0; s != vv.size(); ++s) os << vv[s] << '\n';
    os << '\n';
}

int main() {
    using Space = Kokkos::Experimental::SYCL;
    //using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    //using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;
    //using Space = Kokkos::Experimental::SYCLSharedUSMSpace;
    using memory_space = Space::memory_space;

    Kokkos::ScopeGuard _;
    std::cout << "Space: " << cool::pretty_type<Space>() << '\n';
    std::cout << "memory_space: " << cool::pretty_type<memory_space>() << '\n';

    using view_type = Kokkos::View<int*, Space>;

#if 1
    view_type v("V", 10);
    Kokkos::deep_copy(v, v);
#else
    const cl::sycl::queue& queue = *Kokkos::Experimental::SYCL().impl_internal_space_instance()->m_queue;
    cl::sycl::usm_allocator<view_type, cl::sycl::usm::alloc::host> allocator(queue);
    std::vector<view_type, decltype(allocator)> vec(allocator);
    vec.reserve(1);
    vec.emplace_back("V", 10);
    view_type& v = vec.front();
#endif
    //view_type* pv = &v;


    //printView(v);
#if 1
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

    //Kokkos::View<int*, Space> w("W", v.size());
    Kokkos::View<int*> w("W", v.size());
    printView(w);

    Kokkos::deep_copy(w, v);
    printView(w);
#endif
}

