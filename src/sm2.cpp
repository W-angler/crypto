#include "sm2.h"

SM2::KeyPair SM2::generate(const bigint &k) {
    return SM2::KeyPair(k, curve.multiply(k, curve.G));
}

bigint SM2::encrypt(const bigint &plaintext, const ECPoint &publicKey) {
    return 0;
}

bigint SM2::decrypt(const bigint &cipher, const bigint &privateKey) {
    return 0;
}