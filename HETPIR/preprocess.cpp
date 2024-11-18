// Polynomial.cpp
#include "preprocess.hpp"
#include "params.hpp"


// 创建一个新的多项式
std::vector<std::vector<uint64_t>> create_polynomial(int degree) {
    std::vector<std::vector<uint64_t>> coeffs(degree + 1, std::vector<uint64_t>(degree + 1, 0));
    return coeffs;
}
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;  // 确保 base < mod
    while (exp > 0) {
        // 如果 exp 是奇数，乘上当前的 base
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        // exp 是偶数时，将 base 平方，exp 减半
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}
// 预处理生成look-up table
std::vector<std::vector<uint64_t>> preprocess(int k, int degree, int p, int level, const std::vector<std::vector<uint64_t>>& poly) {
    std::vector<std::vector<uint64_t>> look_up_table(primes[k - 1], std::vector<uint64_t>(k, 0));
    for (int i = 0; i < primes[k - 1]; ++i) {
        for (int j = 0; j < k; ++j) {
            uint64_t temp = 0;
            for (int m = 0; m <= degree; ++m) {
                temp += poly[level][m] * mod_exp((uint64_t)i, (uint64_t)m, primes[j]);
            }
            look_up_table[i][j] = temp;
        }
    }
    return look_up_table;
}

// 计算阶乘
uint64_t factorial(int n) {
    uint64_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// 计算组合数 C(i, k)
uint64_t binomial_coefficient(int i, int k) {
    return factorial(i) / (factorial(k) * factorial(i - k));
}

// 计算product_sum
uint64_t product_sum(int i, int l, int p) {
    int n = l - 1;
    std::vector<uint64_t> elements(n);
    for (int j = 0; j < n; ++j) elements[j] = p - (j + 1);

    uint64_t total_sum = 0;
    std::vector<int> indices(i);
    std::iota(indices.begin(), indices.end(), 0);

    while (true) {
        uint64_t product = 1;
        for (int j : indices) product *= elements[j];
        total_sum += product;

        int idx = i - 1;
        while (idx >= 0 && indices[idx] == n - (i - idx)) --idx;
        if (idx < 0) break;
        ++indices[idx];
        for (int j = idx + 1; j < i; ++j) indices[j] = indices[j - 1] + 1;
    }

    return total_sum;
}

// 计算多项式 (a+b)((a+b)-1)...((a+b)-(l-1))
std::vector<std::vector<uint64_t>> compute_polynomial(int l, int p) {
    auto result = create_polynomial(l);
    for (int k = 0; k <= l - 1; ++k) {
        uint64_t temp = product_sum(k, l, p);
        for (int j = 0; j <= l - k; ++j) {
            int idx = l - k - j;
            result[j][idx] = (result[j][idx] + temp * binomial_coefficient(l - k, j)) % p;
        }
    }
    return result;
}
