Kokkos::Impl::ParallelFor<
    Kokkos::Impl::ViewCopy<
        Kokkos::View<int *, ::Kokkos::LayoutLeft,
                     ::Kokkos::Device< ::Kokkos::Experimental::SYCL,
                                       ::Kokkos::AnonymousSpace>,
                     ::Kokkos::MemoryTraits<0> >,
        Kokkos::View<
            const int *, ::Kokkos::LayoutLeft,
            ::Kokkos::Device< ::Kokkos::Serial, ::Kokkos::AnonymousSpace>,
            ::Kokkos::MemoryTraits<0> >,
        Kokkos::LayoutLeft, Kokkos::Experimental::SYCL, 1, long, false>,
    Kokkos::RangePolicy< ::Kokkos::Experimental::SYCL,
                         ::Kokkos::IndexType<long> >,
    Kokkos::Experimental::SYCL>

