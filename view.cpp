#include <Kokkos_Core.hpp>
#include <iostream>

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

int main() {
    Kokkos::ScopeGuard _;
    std::cout
        << Kokkos::is_execution_space<Kokkos::DefaultExecutionSpace>::value
        << '\n';
    std::cout << Kokkos::is_space<Kokkos::DefaultExecutionSpace>::value << '\n';
    std::cout << Kokkos::is_execution_space<Kokkos::Experimental::SYCL>::value
              << '\n';
    std::cout << Kokkos::is_space<Kokkos::Experimental::SYCL>::value << '\n';
    Kokkos::print_configuration(std::cout, true);
    Kokkos::DefaultExecutionSpace::print_configuration(std::cout);
    Kokkos::DefaultExecutionSpace::print_configuration(std::cout, true);

    using V = Kokkos::View<int*, Kokkos::Experimental::SYCLHostUSMSpace>;
    ViewTypedefs<V>();

    V v("V", 1);
}

