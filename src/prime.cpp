#include "prime.h"

const std::vector<int> prime::PRIMES = sieve::segmented(PRIMES_RANGE);

std::random_device prime::rd;
std::mt19937 prime::random(rd());

/**
 * 确定性判断素数(非常慢)
 *
 * @param n 待测试的数
 * @return 是否是素数
 */
bool prime::isPrime(const bigint &n) {
    if (n < bigint::ONE) {
        return false;
    } else if (n == bigint::TWO) {
        return true;
    } else if (n == 4) {
        return false;
    }
    for (bigint i = bigint::TWO; i < (n >> 1); i = i + bigint::ONE) {
        if (n % i == bigint::ZERO) {
            return false;
        }
    }
    return true;
}

/**
 * Miller-Rabin素性测试
 *
 * @param n 待测试的数
 * @return 是否是素数
 */
bool prime::millerRabin(const bigint &n) {
    if (n <= bigint::ONE || !n.testBit(0)) {
        return false;
    }
    if (n == bigint::TWO) {
        return true;
    }
    bigint base = getLessThan(n);
    bigint thisMinusOne = n - bigint::ONE;
    bigint m = thisMinusOne;
    while (!m.testBit(0)) {
        m = m >> 1;
        bigint z = base.modPow(m, n);
        if (z == thisMinusOne) {
            break;
        }
        if (z != bigint::ONE) {
            return false;
        }
    }
    return true;
}

/**
 * 费马素性测试
 *
 * @param n 待测试的数
 * @return 是否是素数
 */
bool prime::fermat(const bigint &n) {
    if (n <= bigint::ONE || !n.testBit(0)) {
        return false;
    }
    if (n == bigint::TWO) {
        return true;
    }
    bigint base = getLessThan(n);
    return base.modPow(n, n) == base;
}

/**
 * 素性测试
 *
 * @param n      待测试的数
 * @param times  测试次数
 * @param tester 使用的测试方法
 * @return 在输入的测试次数下，是否是素数
 */
bool prime::test(const bigint &n, size_t times, bool(*tester)(const bigint &)) {
    for (size_t i = 0; i < times; i++) {
        if (!tester(n)) {
            return false;
        }
    }
    return true;
}

/**
 * 随机生成一个质数
 *
 * @param bits 质数的位长（8的倍数）
 * @return 随机生成的质数
 */
bigint prime::generate(size_t bits) {
    bigint result = getCandidate(bits);
    choose:
    while (true) {
        //选取前面的素数，如果可以整除，那么不是素数
        for (auto prime : PRIMES) {
            if ((result % prime) == (bigint::ZERO)) {
                //如果可以整除，加2继续测试
                result = result + bigint::TWO;
                goto choose;
            }
        }
        //如果可以通过前面的素数的测试，继续用Miller Rabin测试
        if (!test(result, TEST_TIMES, prime::millerRabin)) {
            //否则加2继续测试
            result = result + bigint::TWO;
        } else {
            //找到一个素数了
            break;
        }
    }
    return result;
}

bigint prime::getLessThan(const bigint &n) {
    bigint result;
    do {
        result = getCandidate(n.bitLength());
    } while (result >= n);
    return result;
}

/**
 * 获取候选的大奇数
 *
 * @param bits 位数
 * @return 候选的大奇数
 */
bigint prime::getCandidate(size_t bits) {
    size_t len = bits / 32 + (bits % 32 ? 1 : 0);
    std::vector<bigint::Base> num;
    //随机数生成
    for (size_t i = 0; i < len; i++) {
        num.push_back(random());
    }
    //如果最高位不是1，设置为1
    if ((num[len - 1] & 0x80000000) != 0x80000000) {
        num[len - 1] |= 0x80000000;
    }
    //如果是偶数，加1
    if ((num[0] & 1) == 0) {
        num[0] |= 1;
    }
    return bigint(num);
}
