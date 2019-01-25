#ifndef CRYPTO_SM2_H
#define CRYPTO_SM2_H

#include "ecc.h"

class SM2 {
public:
    class KeyPair {
    public:
        bigint privateKey;
        ECPoint publicKey;
    public:
        KeyPair(bigint privateKey, ECPoint publicKey)
                : privateKey(std::move(privateKey)), publicKey(std::move(publicKey)) {}
    };

public:
    SM2::KeyPair generate(const bigint &k);

    bigint encrypt(const bigint &plaintext, const ECPoint &publicKey);

    bigint decrypt(const bigint &cipher, const bigint &privateKey);

public:
    //n=FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF 7203DF6B 21C6052B 53BBF409 39D54123
//    ECCurve curve = ECCurve(
//            bigint("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF"),
//            bigint("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"),
//            bigint("28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"),
//            ECPoint(
//                    bigint("32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"),
//                    bigint("BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0")
//            )
//    );

    ECCurve curve = ECCurve(
            bigint::fromHex("8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3"),
            bigint::fromHex("787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498"),
            bigint::fromHex("63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A"),
            ECPoint(
                    bigint::fromHex("421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"),
                    bigint::fromHex("0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2")
            )
    );
};

#endif //CRYPTO_SM2_H
