#include <Kokkos_Core.hpp>
#include <cassert>
#include <iostream>

#include "pretty_name.h"

int main()
{
    std::cout << std::is_trivially_copyable_v<Kokkos::Impl::SharedAllocationTracker> << '\n';
}
