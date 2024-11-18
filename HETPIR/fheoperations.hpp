#ifndef FHEOPERATIONS_HPP
#define FHEOPERATIONS_HPP

#include <vector>
#include <cstdint>
#include <hexl/hexl.hpp>

// 声明函数接口

std::vector<uint64_t> sampleDiscreteGaussian(int n, double sigma, int p);

std::vector<std::vector<uint64_t>> sampleModQ(int n, const std::vector<uint64_t>& q_values);

std::vector<uint64_t> sampleBinarySparse(int n);

std::vector<std::vector<uint64_t>> encrypt(int n, int p, const std::vector<uint64_t>& q_values,
                                           std::vector<uint64_t>& plaintext,
                                           std::vector<uint64_t>& secretkey,
                                           std::vector<std::vector<uint64_t>>& a);

std::vector<std::vector<std::vector<uint64_t>>> relinearkeygen(int n, int l, int p,
                                                               std::vector<uint64_t>& q_values,
                                                               std::vector<std::vector<uint64_t>>& a,
                                                               std::vector<uint64_t>& secretkey);

std::tuple<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>>> relinear(
    int n, int l, std::vector<uint64_t>& q_values,
    std::vector<std::vector<std::vector<uint64_t>>>& ciphertexts,
    std::vector<std::vector<std::vector<uint64_t>>>& relinearkey,
    std::vector<std::vector<uint64_t>>& a);

std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> modswitch(
    int n, const std::vector<uint64_t>& q_values,
    std::vector<std::vector<uint64_t>>& ciphertexts,
    std::vector<std::vector<uint64_t>>& a);

std::vector<uint64_t> decrypt(int n, int p, const uint64_t q_values, std::vector<uint64_t>& secretkey,
                                            std::vector<uint64_t>& a,
                                            std::vector<uint64_t>& ciphertexts, int flag );

#endif // FHEOPERATIONS_HPP
