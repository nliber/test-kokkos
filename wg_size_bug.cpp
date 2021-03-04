#include <CL/sycl.hpp>

// Customer reported a bug for # of bytes >= 409600
const size_t nelem = 409604 / sizeof(float);
// const size_t nelem = 409600 / sizeof(float);

int main() {
    auto max_group_sz = 256;
    auto num_gz = (nelem + max_group_sz - 1) / max_group_sz;
    cl::sycl::range<1> local(max_group_sz);
    cl::sycl::range<1> global(num_gz * max_group_sz);
    std::array<float, 1> result;
    std::vector<float> in_array(nelem);
    result[0] = 0.0f;
    cl::sycl::queue q;
    {
        cl::sycl::buffer<float> res_buf(result.data(), 1);
        cl::sycl::buffer<float> in_buf(in_array.data(), nelem);
        printf("local: %zu, glocal: %zu\n", local.size(), global.size());
        q.submit([&](cl::sycl::handler& cgh) {
            auto res_acc =
                res_buf.template get_access<cl::sycl::access::mode::read_write>(
                    cgh);
            auto in =
                in_buf.template get_access<cl::sycl::access::mode::read>(cgh);
            cgh.parallel_for(
                cl::sycl::nd_range<1>(global, local),
                cl::sycl::ONEAPI::reduction(res_acc, 0.0f,
                                            cl::sycl::ONEAPI::plus<float>()),
                [=](cl::sycl::nd_item<1> it, auto& res_acc) {
                    int lid = it.get_global_id(0);
                    if (lid >= nelem) return;
                    res_acc += in[lid];
                });
        });
    }
    std::cout << "finished\n";
}

