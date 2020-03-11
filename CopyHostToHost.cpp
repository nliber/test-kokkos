#include <Kokkos_Core.hpp>
#include <iostream>

template <typename V>
void printView(V const& v, std::ostream& os = std::cout) {
    auto const size = v.size();
    for (size_t s = 0; s != size; ++s) os << v[s] << '\n';
    os << '\n';
}

int main() {
    //using Space = Kokkos::HostSpace;
    using Space = Kokkos::Experimental::HostUSMMemorySpace;

    Kokkos::ScopeGuard _;

    Kokkos::View<int*, Space> v("V", 10);
    printView(v);

    Kokkos::parallel_for(
        10, KOKKOS_LAMBDA(const size_t i)
        { v[i] = i; });
    printView(v);

    Kokkos::View<int*, Space> w("W", v.size());
    printView(w);

    Kokkos::deep_copy(w, v);
    printView(w);
}

