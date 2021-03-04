#include "CL/sycl.hpp"

struct NotTriviallyCopyable {
    explicit NotTriviallyCopyable(char* p) : m_p(p) {}

    void operator()() const { ++*m_p; }

    ~NotTriviallyCopyable() {}  // This makes it not trivially copyable

    char* m_p;
};
static_assert(!std::is_trivially_copyable_v<NotTriviallyCopyable>);

int main() {
    sycl::queue queue{sycl::default_selector()};

    char* p =
        static_cast<char*>(sycl::malloc(1, queue, sycl::usm::alloc::shared));
    *p = 2;
    NotTriviallyCopyable ntc(p);

    queue.submit([&](cl::sycl::handler& cgh) { cgh.single_task(ntc); });
    queue.wait();

    assert(3 == *p);
    sycl::free(p, queue);

    return 0;
}

