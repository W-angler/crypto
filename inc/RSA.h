#include <utility>

#include <utility>

#include <utility>

#include <utility>

#ifndef RSA_H
#define RSA_H

#include "bigint.h"

class RSA {
private:
    static const bigint DEFAULT_E;

public:
    class PublicKey {
    public:
        bigint n;
        bigint e;
    public:
        PublicKey(bigint n, bigint e) : n(std::move(n)), e(std::move(e)) {}
    };

    class PrivateKey {
    public:
        bigint n;
        bigint p;
        bigint q;
        bigint d;
        bigint dP;
        bigint dQ;
        bigint qInv;
    public:
        PrivateKey(bigint n, bigint p, bigint q, bigint d,
                   bigint dP, bigint dQ, bigint qInv)
                : n(std::move(n)), p(std::move(p)), q(std::move(q)), d(std::move(d)),
                  dP(std::move(dP)), dQ(std::move(dQ)), qInv(std::move(qInv)) {}
    };

    class KeyPair {
    public:
        PrivateKey privateKey;
        PublicKey publicKey;
    public:
        KeyPair(PrivateKey privateKey, PublicKey publicKey)
                : privateKey(std::move(privateKey)), publicKey(std::move(publicKey)) {}
    };

public:
    static RSA::KeyPair generate(size_t bits);

    static bigint encrypt(const bigint &m, const RSA::PublicKey &publicKey);

    static bigint decrypt(const bigint &c, const RSA::PrivateKey &privateKey);

};

#endif //RSA_H
