#ifndef RSA_ECC_H
#define RSA_ECC_H

#include<iostream>
#include "bigint.h"

/**
 * 仿射坐标点
 */
class ECPoint {
public:
    bigint x;
    bigint y;

    ECPoint() : x(bigint::ZERO), y(bigint::ZERO) {}

    ECPoint(bigint x, bigint y) : x(std::move(x)), y(std::move(y)) {}

    friend std::ostream &operator<<(std::ostream &out, const ECPoint &p);

    friend bool operator==(const ECPoint &p1, const ECPoint &p2);
};

struct ECCurve {
public:
    bigint P;//阶
    bigint a;
    bigint b;
    ECPoint G;//生成元
public:
    ECCurve(bigint P, bigint a, bigint b, ECPoint G)
            : P(std::move(P)), a(std::move(a)), b(std::move(b)), G(std::move(G)) {}

public:
    //点逆
    ECPoint neg(const ECPoint &p);

    //点加
    ECPoint add(const ECPoint &p1, const ECPoint &p2);

    //点倍
    ECPoint twice(const ECPoint &p);

    //点乘
    ECPoint multiply(const ECPoint &p, int k);

    //点乘
    ECPoint multiply(int k, const ECPoint &p);

    //点乘
    ECPoint multiply(const ECPoint &p, const bigint &k);

    //点乘
    ECPoint multiply(const bigint &k, const ECPoint &p);
};

#endif //RSA_ECC_H
