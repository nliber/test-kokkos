#include "CL/sycl.hpp"

struct JustBool {
    bool b;
};

struct WithPtr {
    bool b;
    void* p;
};

struct InUnion {
    bool b;
    union U { void* p; } u;
};

int main() {
    cl::sycl::device device{cl::sycl::default_selector()};
    cl::sycl::queue queue(device);

    int i = 2;
    JustBool jb{true};
    WithPtr np{true, nullptr};
    WithPtr wp{true, &i};
    InUnion iu{true, {&i}};
    {
        cl::sycl::buffer<int, 1> ibuffer(&i, cl::sycl::range<1>(1));

        queue.submit([&](cl::sycl::handler& cgh) {
            auto iaccess =
                ibuffer.get_access<cl::sycl::access::mode::discard_write>(cgh);
            cgh.single_task([=]() {
                if (jb.b) iaccess[0] = 3;
            });
        });
        queue.submit([&](cl::sycl::handler& cgh) {
            auto iaccess =
                ibuffer.get_access<cl::sycl::access::mode::discard_write>(cgh);
            cgh.single_task([=]() {
                if (np.b) iaccess[0] = 5;
            });
        });
        queue.submit([&](cl::sycl::handler& cgh) {
            auto iaccess =
                ibuffer.get_access<cl::sycl::access::mode::discard_write>(cgh);
            cgh.single_task([=]() {
                if (iu.b) iaccess[0] = 7;
            });
        });

        // terminate called after throwing an instance of 'cl::sycl::runtime_error'
        // what():  Native API failed. Native API returns: -30 (CL_INVALID_VALUE) -30 (CL_INVALID_VALUE)
        queue.submit([&](cl::sycl::handler& cgh) {
            auto iaccess =
                ibuffer.get_access<cl::sycl::access::mode::discard_write>(cgh);
            cgh.single_task([=]() {
                if (wp.b) iaccess[0] = 11;
            });
        });
    }

    return 0;
}

