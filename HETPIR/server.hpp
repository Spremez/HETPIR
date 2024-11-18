#include <cstdint>
#include <vector>
#include <random>
#include <cmath>
#include "preprocess.hpp"
#include "fheoperations.hpp"
#include "params.hpp"

std::vector<std::vector<uint64_t>> server_offline(int l, int k, int level, int p, int n);
std::vector<std::vector<uint64_t>> generateRandomMatrix(int l, int n, int p);
std::vector<std::vector<std::vector<uint64_t>>> process(int l, int k, int n,  std::vector<std::vector<uint64_t>>& a,
                                                                         std::vector<std::vector<uint64_t>>& b,
                                                                        const std::vector<std::vector<uint64_t>>& look_up_table,
                                                                        std::vector<std::vector<uint64_t>>& database);
std::tuple<std::vector<uint64_t>, std::vector<uint64_t> >relinear_modswitch(int l, int k, int n,
                                                                std::vector<std::vector<std::vector<uint64_t>>>& ciphertexts,
                                                                std::vector<std::vector<std::vector<uint64_t>>>& relinearkey,
                                                                std::vector<std::vector<uint64_t>>& a );

std::tuple<std::vector<uint64_t>, std::vector<uint64_t> > server_process(int l, int k, int n, std::vector<std::vector<uint64_t>>& a,
                                   std::vector<std::vector<uint64_t>>& b,
                                    const std::vector<std::vector<uint64_t>>& look_up_table,
                                    std::vector<std::vector<uint64_t>>& database,
                                    std::vector<std::vector<std::vector<uint64_t>>>& relinearkey );