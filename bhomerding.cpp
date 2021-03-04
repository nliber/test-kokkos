#include <CL/sycl.hpp>
#include <iostream>

#include "pretty_name.h"
using namespace cl::sycl;

int main() {
    double num = 0.0;
    //queue q;

    auto pi = 3.14;
    auto two = 2;
    auto pipowtwo = ::pow(pi, two);
    std::cout << cool::pretty_name(pi) << ' ' << cool::pretty_name(two) << ' '
              << cool::pretty_name(pipowtwo) << '\n';

#if 0
    {
        buffer<double> buf{&num, 1};

        q.submit([&](handler& h) {
            auto num = buf.get_access<access::mode::write>(h);
            h.parallel_for<class test>(range<1>(1), [=](item<1> i) {
                // Assume this code is passed in from a RAJA Lambda
                using cl::sycl::pow;  // <- Existing RAJA/Kokkos kernels require
                                      // edit
                num[i] = ::pow(
                    3.14,
                    2);  // <- error: no matching function for call to 'pow'
                num[i] =
                    pow(3.14, 2.);  // <- This one works, required another edit.
            });
        });
    }
    std::cout << num << std::endl;
#endif

    return 0;
}

