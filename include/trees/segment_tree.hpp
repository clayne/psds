#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

namespace psds {

struct segment_tree {
    template <typename T>
    void build(T const* input, size_t n) {
        m_size = size_t(1) << static_cast<size_t>(ceil(log2(n)));
        m_tree.resize(2 * m_size - 1, 0);
        std::vector<int64_t> in(m_size, 0);
        for (uint64_t i = 0; i != n; ++i) in[i] = input[i];
        build(in.data(), 0, m_size - 1, 0);
    }

    static std::string name() {
        return "segment_tree";
    }

    int64_t sum(uint64_t i) const {
        if ((m_size + 1) / 2 < 4194304) {  // go branch-less here...
            uint64_t n = m_size;
            uint64_t m = (m_size - 1) / 2;
            uint64_t p = 0;
            int64_t sum = 0;
            while (n != 1) {
                uint64_t cmp = i > m;
                p = 2 * p + 1;
                sum += cmp * m_tree[p];
                p += cmp;
                n /= 2;
                int64_t offset = cmp * n - n / 2;
                m += offset;
            }
            return sum + m_tree[p];
        }
        // but switch to branchy code for large n
        size_t l = 0;
        size_t h = m_size - 1;
        size_t p = 0;
        int64_t sum = 0;
        while (l < h) {
            if (i == h) break;
            size_t m = (l + h) / 2;
            p = 2 * p + 1;
            if (i > m) {
                sum += m_tree[p];
                l = m + 1;
                p += 1;
            } else {
                h = m;
            }
        }
        return sum + m_tree[p];
    }

    void update(uint64_t i, int64_t delta) {
        if ((m_size + 1) / 2 < 4194304) {
            uint64_t n = m_size;
            uint64_t m = (m_size - 1) / 2;
            uint64_t p = 0;
            while (n != 1) {
                m_tree[p] += delta;
                uint64_t cmp = i > m;
                p = 2 * p + cmp + 1;
                n /= 2;
                int64_t offset = cmp * n - n / 2;
                m += offset;
            }
            m_tree[p] += delta;
            return;
        }
        size_t l = 0;
        size_t h = m_size - 1;
        size_t p = 0;
        while (l < h) {
            m_tree[p] += delta;
            size_t m = (l + h) / 2;
            p = 2 * p + 1;
            if (i > m) {
                l = m + 1;
                p += 1;
            } else {
                h = m;
            }
        }
        m_tree[p] += delta;
    }

private:
    size_t m_size;
    std::vector<int64_t> m_tree;

    template <typename T>
    int64_t build(T const* input, size_t l, size_t h, size_t p) {
        if (l == h) return m_tree[p] = input[l];
        size_t m = (l + h) / 2;
        int64_t l_sum = build(input, l, m, 2 * p + 1);
        int64_t r_sum = build(input, m + 1, h, 2 * p + 2);
        return m_tree[p] = l_sum + r_sum;
    }
};

}  // namespace psds