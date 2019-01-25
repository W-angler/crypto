#ifndef SIEVE_H
#define SIEVE_H

#include <vector>

class sieve {
public:
    static std::vector<int> simple(int limit);

    static std::vector<int> segmented(int n);
};


#endif //SIEVE_H
