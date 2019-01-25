#include <cmath>
#include "sieve.h"

std::vector<int> sieve::simple(int limit) {
    std::vector<int> prime;
    bool *mark = new bool[limit + 1];
    for (int i = 0; i < limit + 1; i++) {
        mark[i] = true;
    }
    for (int p = 2; p * p < limit; p++) {
        if (mark[p]) {
            for (int i = p * 2; i < limit; i += p) {
                mark[i] = false;
            }
        }
    }
    for (int p = 2; p < limit; p++) {
        if (mark[p]) {
            prime.push_back(p);
        }
    }
    delete[] mark;
    return prime;
}

std::vector<int> sieve::segmented(int n) {
    int limit = (int) (floor(sqrt(n)) + 1);
    std::vector<int> prime = simple(limit);
    int low = limit;
    int high = 2 * limit;
    while (low < n) {
        if (high >= n) {
            high = n;
        }
        bool *mark = new bool[limit + 1];
        for (int i = 0; i < limit + 1; i++) {
            mark[i] = true;
        }
        for (int aPrime : prime) {
            int loLim = low / aPrime * aPrime;
            if (loLim < low) {
                loLim += aPrime;
            }
            for (int j = loLim; j < high; j += aPrime) {
                mark[j - low] = false;
            }
        }
        for (int i = low; i < high; i++) {
            if (mark[i - low]) {
                prime.push_back(i);
            }
        }
        low = low + limit;
        high = high + limit;
        delete[] mark;
    }
    return prime;
}
