#include <Kokkos_Core.hpp>
#include <iostream>

template<typename... Ts>
void printView(Kokkos::View<int*, Ts...> const& v, std::ostream& os = std::cout) {
    os << __PRETTY_FUNCTION__ << '\n';
    Kokkos::View<int*, Kokkos::HostSpace> vv("VV", v.size());
    Kokkos::deep_copy(vv, v);
    for (size_t s = 0; s != vv.size(); ++s) os << vv[s] << '\n';
    os << '\n';
}

int main() {
    using Space = Kokkos::HostSpace;
    //using Space = Kokkos::Experimental::SYCLHostUSMSpace;
    //using Space = Kokkos::Experimental::SYCLDeviceUSMSpace;

    Kokkos::ScopeGuard _;

    Kokkos::View<int*, Space> v("V", 10);
    printView(v);
#if 0
    Kokkos::parallel_for(
        10, KOKKOS_LAMBDA(const size_t i)
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

