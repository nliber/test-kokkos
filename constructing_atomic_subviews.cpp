#include <Kokkos_Core.hpp>

template<typename MT>
void TestSubview()
{
    Kokkos::View<double*, Kokkos::HostSpace> v("v", 7);
    for (int i = 0; i < v.size(); ++i)
        v[i] = static_cast<double>(i);

    auto range = std::make_pair(3, 5);
    auto sv = Kokkos::subview<MT>(v, range);

    for (int j = 0; j < sv.size(); ++j)
        std::cout << sv[j] << '\n';

}


int main() {
    using namespace Kokkos;
    ScopeGuard _;

    View<double*> a("a",10);
    auto range = std::pair(0,4);
    auto s0 = subview(a, range);
    auto s1 = subview<MemoryTraits<Unmanaged>>(a, range);
    auto s2 = subview<MemoryTraits<RandomAccess>>(a, range);
    auto s3 = subview<MemoryTraits<Atomic>>(a, range);   // ERROR!
    auto s4 = subview<MemoryTraits<Restrict>>(a, range);
    auto s5 = subview<MemoryTraits<Aligned>>(a, range);

    TestSubview<MemoryTraits<Unmanaged>>();
    TestSubview<MemoryTraits<RandomAccess>>();
    TestSubview<MemoryTraits<Atomic>>();
    TestSubview<MemoryTraits<Restrict>>();
    //TestSubview<MemoryTraits<Aligned>>();

    TestSubview<MemoryTraits<0>>();
    TestSubview<MemoryTraits<1>>();
    TestSubview<MemoryTraits<2>>();
    TestSubview<MemoryTraits<3>>();
    TestSubview<MemoryTraits<4>>();
    TestSubview<MemoryTraits<5>>();
    TestSubview<MemoryTraits<6>>();
    TestSubview<MemoryTraits<7>>();
    TestSubview<MemoryTraits<8>>();
    TestSubview<MemoryTraits<9>>();
    TestSubview<MemoryTraits<10>>();
    TestSubview<MemoryTraits<11>>();
    TestSubview<MemoryTraits<12>>();
    TestSubview<MemoryTraits<13>>();
    TestSubview<MemoryTraits<14>>();
    TestSubview<MemoryTraits<15>>();
}

