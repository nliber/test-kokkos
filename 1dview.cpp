#include <Kokkos_Core.hpp>
#include <Kokkos_DualView.hpp>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc,argv); {

  Kokkos::DualView<int[3], Kokkos::Experimental::SYCL> v("v");

  Kokkos::View<int[3], Kokkos::DefaultHostExecutionSpace> a = v.h_view;
  //Kokkos::View<int*, Kokkos::DefaultHostExecutionSpace> a = v.h_view;

  } Kokkos::finalize();

}

