#pragma once

namespace psds {
namespace testing {

template <typename Tree>
void test_tree(size_t n) {
    essentials::uniform_int_rng<int64_t> distr(-100, +1000000000,
                                               essentials::get_random_seed());
    std::cout << "== testing " << Tree::name() << " with " << n
              << " nodes ==" << std::endl;
    std::vector<int64_t> A(n);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    // std::cout << "array is:\n";
    // for (auto x : A) { std::cout << x << " "; }
    // std::cout << std::endl;
    Tree tree;
    essentials::logger("building tree...");
    tree.build(A.data(), n);

    {
        essentials::logger("testing sum queries...");
        int64_t expected = 0;
        for (uint32_t i = 0; i != n; ++i) {
            int64_t got = tree.sum(i);
            expected += A[i];
            REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                        << " but expected "
                                                        << expected);
        }
    }

    auto update = [&](int64_t delta) {
        essentials::logger("testing update queries...");
        static constexpr uint32_t queries = 5000;
        uint32_t step = n / queries;
        if (step == 0) step += 1;
        for (uint32_t run = 0; run != 100; ++run) {
            int64_t expected = 0;
            uint32_t k = 0;
            for (uint32_t i = 0; i < n; i += step) {
                tree.update(i, delta);
                int64_t got = tree.sum(i);
                A[i] += delta;
                for (; k != i + 1; ++k) expected += A[k];
                REQUIRE_MESSAGE(got == expected,
                                "error during run "
                                    << run << ": got sum(" << i << ") = " << got
                                    << " but expected " << expected);
            }
        }
    };

    int64_t delta = distr.gen();
    std::cout << "delta " << delta << std::endl;
    update(delta);
    std::cout << "\teverything's good" << std::endl;
}

}  // namespace testing
}  // namespace psds