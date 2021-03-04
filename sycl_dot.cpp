double sycl_dot(int R) {
    //DOT f(*this);
    const auto y_ = y.data();
    const auto x_ = x.data();

    auto sycl_queue = cl::sycl::queue(cl::sycl::gpu_selector());

    // Warmup
    double result = 0.;
    auto result_ptr = static_cast<double*>(
        sycl::malloc(sizeof(result), sycl_queue, sycl::usm::alloc::shared));
    sycl_queue.submit([&](cl::sycl::handler& cgh) {
        auto reduction = cl::sycl::ONEAPI::reduction(result_ptr, std::plus<>());
        cgh.parallel_for(cl::sycl::nd_range<1>(N, 1), reduction,
                         [=](cl::sycl::nd_item<1> itemId, auto& sum) {
                             const int i = itemId.get_global_id();
                             sum.combine(x_[i] * y_[i]);
                         });
    });
    sycl_queue.wait();
}

