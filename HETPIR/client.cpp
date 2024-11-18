#include "client.hpp"

std::tuple<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>>> Query(int n, int p, const std::vector<uint64_t>& q_values, 
                                                                                                            std::vector<uint64_t>& s)
{
    std::vector<uint64_t> plaintext(n, 0); 
    auto a = sampleModQ(n,q_values);
    auto b = encrypt(n,p,q_values,plaintext,s,a);
    return {a,b};
}