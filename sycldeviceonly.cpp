#include <CL/sycl.hpp>

struct A {
    explicit A(unsigned char* p) : m_p(p) {}

    void operator()() const {
        *m_p = 0;

        *m_p |= 1 << 0;
#ifdef __SYCL_DEVICE_ONLY__
        *m_p |= 1 << 1;
#endif  // __SYCL_DEVICE_ONLY__
#ifndef __SYCL_DEVICE_ONLY__
        *m_p |= 1 << 2;
#endif  // !__SYCL_DEVICE_ONLY
    }

    unsigned char* m_p;
};

int main() {
    sycl::queue q;

    auto* ucp = static_cast<unsigned char*>(
        sycl::malloc(1, q, sycl::usm::alloc::shared));
    *ucp = 0;

    *ucp |= 1 << 0;
#ifdef __SYCL_DEVICE_ONLY__
    *ucp |= 1 << 1;
#endif  // __SYCL_DEVICE_ONLY__
#ifndef __SYCL_DEVICE_ONLY__
    *ucp |= 1 << 2;
#endif  // !__SYCL_DEVICE_ONLY

    q.submit([&](sycl::handler& cgh) {
        cgh.single_task([=] {
            *ucp |= 1 << 4;
#ifdef __SYCL_DEVICE_ONLY__
            *ucp |= 1 << 5;
#endif  // __SYCL_DEVICE_ONLY__
#ifndef __SYCL_DEVICE_ONLY__
            *ucp |= 1 << 6;
#endif  // !__SYCL_DEVICE_ONLY__
        });
    });
    q.wait();

    std::cout << std::hex << "0x" << +*ucp << '\n';

    A a(ucp);
    a();
    std::cout << std::hex << "0x" << +*a.m_p << '\n';

    q.submit([&](sycl::handler& cgh) {
        cgh.single_task([=]{ a(); });
    });
    q.wait();
    std::cout << std::hex << "0x" << +*a.m_p << '\n';

    sycl::free(ucp, q);

    return 0;
}
