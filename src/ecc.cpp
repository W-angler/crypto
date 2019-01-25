
#include <ecc.h>

#include "ecc.h"

bool operator==(const ECPoint &p1, const ECPoint &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

std::ostream &operator<<(std::ostream &out, const ECPoint &p) {
    out << "(" << p.x.toHexString() << "," << p.y.toHexString() << ")";
    return out;
}

ECPoint ECCurve::neg(const ECPoint &a) {
    return ECPoint(a.x, (-a.y) % P);
}

static bool timesOf(const bigint &a, const bigint &b) {
    return a < b ? b % a == bigint::ZERO : a % b == bigint::ZERO;
}

ECPoint ECCurve::add(const ECPoint &p1, const ECPoint &p2) {
    //仿射坐标
    //P1无穷远点
    if (p1.x == bigint::ZERO && p1.y == bigint::ZERO) {
        return p2;
    }
    //P2无穷远点
    if (p2.x == bigint::ZERO && p2.y == bigint::ZERO) {
        return p1;
    }
    if (p1.y + p2.y == P) {
        return ECPoint();
    }
    bigint lambda = (p1.x != p2.x)
                    ? ((p2.y - p1.y) * (((p2.x - p1.x) % P).modInverse(P))) % P
                    : ((3 * p1.x * p1.x + a) * (((2 * p1.y) % P).modInverse(P))) % P;
    bigint x3 = (lambda * lambda - p1.x - p2.x) % P;
    bigint y3 = (lambda * (p1.x - x3) - p1.y) % P;
    return ECPoint(x3, y3);
//    //射影坐标
//    //P1无穷远点
//    if (p1.x == bigint::ZERO && p1.z == bigint::ZERO) {
//        return p2;
//    }
//    //P2无穷远点
//    if (p2.x == bigint::ZERO && p2.z == bigint::ZERO) {
//        return p1;
//    }
//    //P1==P2
//    if (timesOf(p1.x, p2.x) && timesOf(p1.y, p2.y) && timesOf(p1.z, p2.z)) {
//        bigint lambda1 = (3 * p1.x * p1.x + a * p1.z * p1.z) % P;//lambda1 = 3*x1^2+a*z1^2
//        bigint lambda2 = (p1.y * p1.z) % P;//lambda2 = 2*y1*z1
//        bigint lambda3 = (p1.y * p1.y) % P;//lambda3 = y1^2
//        bigint lambda4 = (lambda3 * p1.x * p1.z) % P;//lambda4 = lambda3*x1*z1
//        bigint lambda5 = (lambda2 * lambda2) % P;//lambda5 = lambda2^2
//        bigint lambda6 = (lambda1 * lambda1 - 8 * lambda4) % P;//lambda6 = lambda1^2−8*lambda4
//
//        //x3 = lambda2*lambda6
//        bigint x3 = (p1.x * p2.z) % P;
//        //y3 = lambda1*(4*lambda4−lambda6)−2*lambda5*lambda3
//        bigint y3 = (lambda1 * (4 * lambda4 - lambda6) - 2 * lambda5 * lambda3) % P;
//        bigint z3 = (lambda2 * lambda5) % P;//z3=lambda2 * lambda5
//        return ECPoint(x3, y3, z3);
//    } else {
//        bigint lambda1 = (p1.x * p2.z) % P;//lambda1 = x1*z2
//        bigint lambda2 = (p2.x * p1.z) % P;//lambda2 = x2*z1
//        bigint lambda3 = (lambda1 - lambda2) % P;//lambda3 = lambda1-lambda2
//        bigint lambda4 = (p1.y * p2.z) % P;//lambda4 = y1*z2
//        bigint lambda5 = (p2.y * p1.z) % P;//lambda5 = y2*z1
//        bigint lambda6 = (lambda4 - lambda5) % P;//lambda6 = lambda4-lambda5
//        bigint lambda7 = (lambda1 + lambda2) % P;//lambda7 = lambda1+lambda2
//        bigint lambda8 = (p1.z * p2.z) % P;//lambda8 = z1*z2
//        bigint lambda9 = (lambda3 * lambda3) % P;//lambda9 = lambda3*lambda3
//        bigint lambda10 = (lambda3 * lambda9) % P;//lambda10 = lambda3*lambda9
//        //lambda11 = lambda8*lambda6*lambda6-lambda7*lambda9
//        bigint lambda11 = (lambda8 * lambda6 * lambda6 - lambda7 * lambda9) % P;
//
//        //x3 = lambda3*lambda11
//        bigint x3 = (lambda3 * lambda11) % P;
//        //y3 = lambda6*(lambda9*lambda1-lambda11)-lambda4*lambda10
//        bigint y3 = (lambda6 * (lambda9 * lambda1 - lambda11) - lambda4 * lambda10) % P;
//        //z3 = lambda10*lambda8
//        bigint z3 = (lambda10 * lambda8) % P;
//        return ECPoint(x3, y3, z3);
//    }
}

ECPoint ECCurve::twice(const ECPoint &p) {
    return add(p, p);
}

ECPoint ECCurve::multiply(const ECPoint &p, int k) {
    if (k == 0) {
        return ECPoint();
    }
    if (k == 1) {
        return p;
    }
    ECPoint result;
    ECPoint base = G;
    while (k) {
        if (k & 1) {
            result = add(result, base);
        }
        base = twice(base);
        k >>= 1;
    }
    return result;
}

ECPoint ECCurve::multiply(int k, const ECPoint &p) {
    return multiply(p, k);
}

//点乘
ECPoint ECCurve::multiply(const ECPoint &p, const bigint &k) {
    if (k == bigint::ZERO) {
        return ECPoint();
    }
    if (k == bigint::ONE) {
        return p;
    }
    ECPoint result;
    ECPoint base = G;
    bigint n(k);
    while (n != bigint::ZERO) {
        if (n.testBit(0)) {
            result = add(result, base);
        }
        base = twice(base);
        n = n.rightShift(1);
    }
    return result;
}

//点乘
ECPoint ECCurve::multiply(const bigint &k, const ECPoint &p) {
    return multiply(p, k);
}