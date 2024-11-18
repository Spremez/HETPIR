#include "server.hpp"


std::vector<std::vector<uint64_t>> server_offline(int l, int k, int level, int p, int n){
    auto poly = compute_polynomial(l,p);
    auto lookup_table = preprocess(k, l, p, level, poly);
    return lookup_table;
}

std::vector<std::vector<std::vector<uint64_t>>> process(int l, int k, int n,  std::vector<std::vector<uint64_t>>& a,
                                                                         std::vector<std::vector<uint64_t>>& b,
                                                                         const std::vector<std::vector<uint64_t>>& look_up_table,
                                                                        std::vector<std::vector<uint64_t>>& database){
    std::vector<std::vector<std::vector<uint64_t>>> ciphertexts(l+1,std::vector<std::vector<uint64_t>>(k,std::vector<uint64_t>(n,0))); 
    std::vector<uint64_t> tempb(n,0);
    for (int i = 0; i < l+1 ; i++)
    {
        for (int j = 0; j < k; j++)
        {
            for (int m = 0; m < n; m++)
            {
    
                    uint64_t idx = b[j][m] % primes[i];
                    ciphertexts[i][j][m] = look_up_table[(int)idx][j];
                    tempb = ciphertexts[i][j];
                
            }
            
            for (int m = 0; m < (1ULL << l); m++)
            {
                if (i == l)
                {
                   intel::hexl::EltwiseMultMod(tempb.data(),tempb.data(),database[m].data(),tempb.size(),primes[j],1);
                    intel::hexl::EltwiseAddMod(ciphertexts[i][j].data(),tempb.data(),ciphertexts[i][j].data(),tempb.size(),primes[j]);
                }
                else{
                    intel::hexl::EltwiseMultMod(database[m].data(),a[j].data(),database[m].data(),a[j].size(),primes[j],1);
                    intel::hexl::EltwiseMultMod(tempb.data(),tempb.data(),database[m].data(),tempb.size(),primes[j],1);
                    intel::hexl::EltwiseAddMod(ciphertexts[i][j].data(),tempb.data(),ciphertexts[i][j].data(),tempb.size(),primes[j]);
                }
                
            }
            
        }
        
    }
     return ciphertexts;                                                                  
}

std::tuple<std::vector<uint64_t>, std::vector<uint64_t> >relinear_modswitch(int l, int k, int n,
                                                                std::vector<std::vector<std::vector<uint64_t>>>& ciphertexts,
                                                                std::vector<std::vector<std::vector<uint64_t>>>& relinearkey,
                                                                std::vector<std::vector<uint64_t>>& a ){
    auto[tempa,tempb] = relinear(n,l,primes,ciphertexts,relinearkey,a);
    auto[ta,tb] = modswitch(n,primes,tempb,tempa);
    return {ta,tb};
    }
std::tuple<std::vector<uint64_t>, std::vector<uint64_t> > server_process(int l, int k, int n, std::vector<std::vector<uint64_t>>& a,
                                   std::vector<std::vector<uint64_t>>& b,
                                    const std::vector<std::vector<uint64_t>>& look_up_table,
                                    std::vector<std::vector<uint64_t>>& database,
                                    std::vector<std::vector<std::vector<uint64_t>>>& relinearkey )
{
    auto ciphertexts = process(l,k,n,a,b,look_up_table,database);
    auto[tempa,tempb] = relinear_modswitch(l,k,n,ciphertexts,relinearkey,a);
    return {tempa,tempb};

}
std::vector<std::vector<uint64_t>> generateRandomMatrix(int l, int n, int p) {
    // 计算第一维的大小：2^l
    int rows = std::pow(2, l);
    
    // 初始化二维数组
    std::vector<std::vector<uint64_t>> matrix(rows, std::vector<uint64_t>(n));

    // 设置随机数生成器
    std::random_device rd;  // 随机数种子
    std::mt19937 gen(rd()); // 使用Mersenne Twister算法生成随机数
    std::uniform_int_distribution<uint64_t> dist(0, p - 1); // 在 [0, p-1] 范围内生成随机数

    // 填充二维数组
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = dist(gen); // 生成模p的随机值
        }
    }

    return matrix;
}