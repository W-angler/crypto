#include "RSA.h"
#include "prime.h"

const bigint RSA::DEFAULT_E(65537);

/**
 * RSA密钥生成
 *
 * @param bits 密钥长度
 * @return 密钥对
 */
RSA::KeyPair RSA::generate(size_t bits) {
    size_t len = (bits + 1) / 2;
    bigint N;
    bigint p;
    bigint q;
    while (true) {
        p = prime::generate(len);
        q = prime::generate(len);
        N = p * q;
        if (N.bitLength() == bits) {
            break;
        }
    }
    auto pSubtractOne = p - bigint::ONE;
    auto qSubtractOne = q - bigint::ONE;
    bigint d = DEFAULT_E.modInverse(pSubtractOne * qSubtractOne);
    PublicKey publicKey(N, DEFAULT_E);
    // e·dP = 1 (mod (p–1))
    // e·dQ = 1 (mod (q–1))
    // q·qInv = 1 (mod p)
    PrivateKey privateKey(N, p, q, d,
                          DEFAULT_E.modInverse(pSubtractOne), DEFAULT_E.modInverse(qSubtractOne), q.modInverse(p));
    return KeyPair(privateKey, publicKey);
}

bigint RSA::encrypt(const bigint &m, const RSA::PublicKey &publicKey) {
    return m.modPow(publicKey.e, publicKey.n);
}

bigint RSA::decrypt(const bigint &c, const RSA::PrivateKey &privateKey) {
    return c.modPow(privateKey.d, privateKey.n);
}
