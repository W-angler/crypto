#include <iostream>
#include <ctime>
#include <cassert>
#include "prime.h"

using namespace std;

int main(int argc, char **argv) {
    {
        bigint prime1("3347807169895689878604416984821269081770479498371376856891"
                      "2431388982883793878002287614711652531743087737814467999489");
        bigint prime2("3674604366679959042824463379962795263227915816434308764267"
                      "6032283815739666511279233373417143396810270092798736308917");
        assert(prime::millerRabin(prime1));
        assert(prime::fermat(prime1));
        assert(prime::millerRabin(prime2));
        assert(prime::fermat(prime2));
    }
    {
        clock_t start, end;
        size_t bits = 1024;
        for (size_t i = 0; i < 10; i++) {
            cout << "finding prime number... " << endl;
            start = clock();
            bigint prime = prime::generate(bits);
            end = clock();
            cout << "found! cost " << (end - start) << "ms: " << prime << endl;
        }
    }
}
