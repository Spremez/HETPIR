// main.cpp
#include "fheoperations.hpp"
#include "preprocess.hpp"
#include "params.hpp"
#include "server.hpp"
#include "client.hpp"
#include <iostream>
#include <chrono>


int main() {
    // Example parameters
    int n = 1ULL << 14;
    int p = 65537;
    int l = 8;
    int level = 1;
    int k = primes.size();

    // Sample usage of preprocess function
    std::cout << "Offline stage by srever:\n";
    auto start = std::chrono::high_resolution_clock::now();

    auto look_up_table = server_offline(l,k,level,p,n);
    auto database = generateRandomMatrix(l,n,p);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Done preprocess the lookup tables offline with degree 8 of the equality function and encode database:\n";
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    std::cout << std::endl;
    
    std::cout << "FHE params: n = 2^14, q=245, p = 65537; database: 2^8 * 2^14 * 2B; keywordlength: 22;" <<std::endl;

    std::cout << std::endl;
    //client keygen & query
    start = std::chrono::high_resolution_clock::now();
    auto s = sampleBinarySparse(n);
    auto [a,b] = Query(n,p,primes,s);
    auto relinearkey = relinearkeygen(n,l,p,primes,a,s);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Done client keygen & query:\n";
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    std::cout << std::endl;
    start = std::chrono::high_resolution_clock::now();
    auto [ra,rb] = server_process(l,k,n,a,b,look_up_table,database,relinearkey);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Done server response:\n";
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    std::cout << std::endl;
    start = std::chrono::high_resolution_clock::now();
    auto result = decrypt(n,p,primes[k-1],s,ra,rb,1);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Done client extract:\n";
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
