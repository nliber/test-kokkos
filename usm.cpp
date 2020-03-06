#include <CL/sycl.hpp>
#include <iostream>

using namespace cl::sycl;

int main() {
  queue q{cpu_selector()};
  auto dev  = q.get_device();
  auto ctxt = q.get_context();
  float* a  = static_cast<float*>(malloc_shared(10 * sizeof(float), dev, ctxt));
  float* b  = static_cast<float*>(malloc_shared(10 * sizeof(float), dev, ctxt));
  float* c  = static_cast<float*>(malloc_shared(10 * sizeof(float), dev, ctxt));

  auto e = q.submit([&](handler& cgh) {
    cgh.parallel_for<class init_a>(range<1>(10), [=](id<1> ID) {
      size_t i = ID[0];
      a[i]     = 2;
    });
  });
  e.wait();

  auto f = q.submit([&](handler& cgh) {
    cgh.parallel_for<class init_b>(range<1>(10), [=](id<1> ID) {
      size_t i = ID[0];
      b[i]     = 3;
    });
  });
  f.wait();

  auto g = q.submit([&](handler& cgh) {
    cgh.parallel_for<class vec_add>(range<1>{10}, [=](id<1> ID) {
      size_t i = ID[0];
      c[i]     = a[i] + b[i];
    });
  });
  g.wait();

  for (int i = 0; i < 10; ++i)
  {
      std::cout << i << ' ' << a[i] << ' ' << b[i] << ' ' << c[i] << '\n';
  }
}

