#include "fheoperations.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

using namespace intel::hexl;

// 生成长度为n，标准差为sigma的离散高斯分布向量
std::vector<uint64_t> sampleDiscreteGaussian(int n, double sigma, int p) {
    std::vector<uint64_t> gaussian_vector(n);
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, sigma);

    for (int i = 0; i < n; ++i) {
        gaussian_vector[i] = static_cast<uint64_t>(p * std::round(distribution(generator)));
    }

    return gaussian_vector;
}

// 生成k个长度为n的模q_i的随机向量
std::vector<std::vector<uint64_t>> sampleModQ(int n, const std::vector<uint64_t>& q_values) {
    int k = q_values.size();
    std::vector<std::vector<uint64_t>> modq_vectors(k, std::vector<uint64_t>(n));
    std::default_random_engine generator;

    for (int i = 0; i < k; ++i) {
        std::uniform_int_distribution<uint64_t> distribution(0, q_values[i] - 1);
        for (int j = 0; j < n; ++j) {
            modq_vectors[i][j] = distribution(generator);
        }
    }

    return modq_vectors;
}

// 生成长度为n，权重为n/2的0-1稀疏向量
std::vector<uint64_t> sampleBinarySparse(int n) {
    std::vector<uint64_t> binary_vector(n, 0);
    std::fill(binary_vector.begin(), binary_vector.begin() + n / 2, 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(binary_vector.begin(), binary_vector.end(), g);

    return binary_vector;
}

// 加密函数
std::vector<std::vector<uint64_t>> encrypt(int n, int p, const std::vector<uint64_t>& q_values,
                                           std::vector<uint64_t>& plaintext,
                                           std::vector<uint64_t>& secretkey,
                                           std::vector<std::vector<uint64_t>>& a) {
    int k = q_values.size();
    std::vector<std::vector<uint64_t>> ciphertexts(k, std::vector<uint64_t>(n,0));

    for (int i = 0; i < k; i++) {
        std::vector<uint64_t> error = sampleDiscreteGaussian(n, 3.19, p);
        uint64_t q = q_values[i];
        intel::hexl::NTT ntt(n, q_values[i]);
        std::vector<uint64_t>  tempa = a[i];

        ntt.ComputeForward(tempa.data(), tempa.data(), 1, 1);
        ntt.ComputeForward(secretkey.data(), secretkey.data(), 1, 1);
        ntt.ComputeForward(error.data(), error.data(), 1, 1);
        intel::hexl::EltwiseMultMod(ciphertexts[i].data(), tempa.data(), secretkey.data(), tempa.size(), q_values[i], 1);
        intel::hexl::EltwiseAddMod(ciphertexts[i].data(), a[i].data(), error.data(), a[i].size(), q_values[i]);
        intel::hexl::EltwiseAddMod(ciphertexts[i].data(), a[i].data(), plaintext.data(), a[i].size(), q_values[i]);
    }

    return ciphertexts;
}

// 生成重线性化密钥
std::vector<std::vector<std::vector<uint64_t>>> relinearkeygen(int n, int l, int p, std::vector<uint64_t>& q_values, 
                                                        std::vector<std::vector<uint64_t>>& a, 
                                                        std::vector<uint64_t>& secretkey)
{
    int k = q_values.size();
    std::vector<std::vector<std::vector<uint64_t>>> relinearkey(l-1, std::vector<std::vector<uint64_t>>(k,std::vector<uint64_t>(n,0)));
    for (int i = 0; i < l-1; i++)
    {
        std::vector<uint64_t> plaintext = secretkey;

        for (int j = 0; j < k; j++) {



            if(i == 0)
            {
                // 生成离散高斯噪声向量，标准差可以根据需要调整
        std::vector<uint64_t> error = sampleDiscreteGaussian(n, 3.19, p);
        uint64_t q = q_values[i]; // 当前模数
        intel::hexl::NTT ntt(n, q_values[i]);

        ntt.ComputeForward(a[i].data(), a[i].data(), 1, 1);
        ntt.ComputeForward(secretkey.data(), secretkey.data(), 1, 1);
        ntt.ComputeForward(error.data(), error.data(), 1, 1);
        intel::hexl::EltwiseMultMod(plaintext.data(), plaintext.data(),secretkey.data(),plaintext.size(),q_values[i],1);
        intel::hexl::EltwiseMultMod(relinearkey[i][j].data(), a[i].data(), secretkey.data(), a[i].size(), q_values[i], 1);
        intel::hexl::EltwiseAddMod(relinearkey[i][j].data(), a[i].data(), error.data(), a[i].size(), q_values[i]);
        intel::hexl::EltwiseAddMod(relinearkey[i][j].data(), a[i].data(), plaintext.data(), a[i].size(), q_values[i]);
            }
            else {
                // 生成离散高斯噪声向量，标准差可以根据需要调整
        std::vector<uint64_t> error = sampleDiscreteGaussian(n, 3.19, p);
        uint64_t q = q_values[i]; // 当前模数
        intel::hexl::NTT ntt(n, q);

        ntt.ComputeForward(a[i].data(), a[i].data(), 1, 1);
        ntt.ComputeForward(error.data(), error.data(), 1, 1);
        intel::hexl::EltwiseMultMod(plaintext.data(), plaintext.data(),secretkey.data(),plaintext.size(),q,1);
        intel::hexl::EltwiseMultMod(relinearkey[i][j].data(), a[i].data(), secretkey.data(), a[i].size(), q, 1);
        intel::hexl::EltwiseAddMod(relinearkey[i][j].data(), a[i].data(), error.data(), a[i].size(), q);
        intel::hexl::EltwiseAddMod(relinearkey[i][j].data(), a[i].data(), plaintext.data(), a[i].size(), q);
            }
        
    }

    }
    return relinearkey;

}

std::tuple<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>> >relinear(int n, int l, std::vector<uint64_t>& q_values, 
                                                std::vector<std::vector<std::vector<uint64_t>>>& ciphertexts, 
                                                std::vector<std::vector<std::vector<uint64_t>>>& relinearkey,
                                                std::vector<std::vector<uint64_t>>& a )
{
    int k = q_values.size();
    std::vector<std::vector<uint64_t>> resulta (k, std::vector<uint64_t>(n,0));
    std::vector<std::vector<uint64_t>> resultb (k, std::vector<uint64_t>(n,0));
    for (int i = 0; i < l-1; i++)
    {
        for (int j = 0; j < k; j++)
        {
            uint64_t q = q_values[j];
            intel::hexl::EltwiseMultMod(relinearkey[i][j].data(), relinearkey[i][j].data(),ciphertexts[i+2][j].data(),relinearkey[i][j].size(),q,1);
            intel::hexl::EltwiseAddMod(resulta[j].data(), resulta[j].data(), relinearkey[i][j].data(), resulta[j].size(), q);
            intel::hexl::EltwiseMultMod(a[j].data(), a[j].data(),ciphertexts[i+2][j].data(),a[j].size(),q,1);
            intel::hexl::EltwiseAddMod(resultb[j].data(), resultb[j].data(), a[j].data(), resultb[j].size(), q);

        }
        
    }
    return {resulta,resultb};
    
}

std::tuple<std::vector<uint64_t>,std::vector<uint64_t>> modswitch(int n, const std::vector<uint64_t>& q_values, 
                                                                    std::vector<std::vector<uint64_t>>& ciphertexts,
                                                                    std::vector<std::vector<uint64_t>>& a ){
    int k = q_values.size();
    std::vector<uint64_t> resultc(n,0);
    std::vector<uint64_t> resulta(n,0);
    for (int i = 0; i < k; ++i) {
        uint64_t m_i = 1;
        for (int j = 0; j < k; j++)
        {
            if (j==i)
            {
                continue;
            }
            m_i *= q_values[j] % q_values[i]; 
            
        }
        
        uint64_t m_i_inverse = intel::hexl::InverseMod(m_i, q_values[i]);
        uint64_t temp = (m_i_inverse * q_values[0] + q_values[i]/2)/q_values[i];
        intel::hexl::EltwiseFMAMod(a[i].data(), a[i].data(), temp, nullptr,a[i].size(), q_values[0], 1);
        intel::hexl::EltwiseFMAMod(ciphertexts[i].data(), ciphertexts[i].data(), temp, nullptr, ciphertexts[i].size(), q_values[0], 1);
        intel::hexl::EltwiseAddMod(resulta.data(), a[i].data(), resulta.data(), a[i].size(), q_values[0]);
        intel::hexl::EltwiseAddMod(resultc.data(), ciphertexts[i].data(), resultc.data(), ciphertexts[i].size(), q_values[0]);
    }
    return {resulta,resultc};
}


std::vector<uint64_t> decrypt(int n, int p, const uint64_t q_values, std::vector<uint64_t>& secretkey,
                                            std::vector<uint64_t>& a,
                                            std::vector<uint64_t>& ciphertexts, int flag ) //flag = 1 ntt,=0 none ntt
{
    std::vector<uint64_t> plaintext(n,0);
    uint64_t modulus = p;
    intel::hexl::EltwiseMultMod(a.data(), a.data(), secretkey.data(), a.size(), q_values, 1);
    intel::hexl::EltwiseAddMod(plaintext.data(), a.data(), ciphertexts.data(), a.size(), modulus);
    
    if ( flag == 1) return plaintext;
    else {
        intel::hexl::NTT ntt(n, (uint64_t)p);
        ntt.ComputeInverse(plaintext.data() , plaintext.data() , 1, 1);
        return plaintext;
    }

} 