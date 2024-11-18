#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <cstdint>
#include <vector>
#include <cmath>
#include <numeric>

std::vector<std::vector<uint64_t>> create_polynomial(int degree);
std::vector<std::vector<uint64_t>> preprocess(int k, int degree, int p, int level, const std::vector<std::vector<uint64_t>>& poly);
uint64_t factorial(int n);
uint64_t binomial_coefficient(int i, int k);
uint64_t product_sum(int i, int l, int p);
std::vector<std::vector<uint64_t>> compute_polynomial(int l, int p);
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod);

#endif