#include <cassert>
#include <ctime>
#include "sm2.h"
#include "prime.h"

using namespace std;

int main() {
    SM2 sm2;
    bigint k = bigint::fromHex("4C62EEFD6ECFC2B95B92FD6C3D9575148AFA17425546D49018E5388D49DD7B4F");
    clock_t start = clock();
    auto keyPair = sm2.generate(k);
    clock_t end = clock();
    cout << (end - start) << "ms" << endl;
    cout << "Public: " << keyPair.publicKey << endl;
    cout << "Private: " << keyPair.privateKey.toHexString() << endl;
    return 0;
}