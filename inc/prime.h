#ifndef PRIME_H
#define PRIME_H

#include <vector>
#include <random>
#include "bigint.h"
#include "sieve.h"

class prime {
private:
    static const int PRIMES_RANGE = 1000;
    static const std::vector<int> PRIMES;
    static const int TEST_TIMES = 4;
    static std::random_device rd;
    static std::mt19937 random;
public:
    static bool millerRabin(const bigint &n);

    static bool fermat(const bigint &n);

    static bool test(const bigint &n, size_t times, bool(*tester)(const bigint &));;

    static bigint generate(size_t bits);

    static bigint getCandidate(size_t bits);

    static bool isPrime(const bigint &n);

private:
    static bigint getLessThan(const bigint &n);
};


#endif //PRIME_H
