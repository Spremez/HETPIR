#include <cstdint>
#include <vector>
#include "preprocess.hpp"
#include "fheoperations.hpp"
#include "params.hpp"



std::tuple<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>>> Query(int n, int p, const std::vector<uint64_t>& q_values, 
                                                                                                            std::vector<uint64_t>& s);
