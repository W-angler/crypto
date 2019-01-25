#include<cassert>
#include <ctime>
#include"bigint.h"

const bigint bigint::ZERO(0);
const bigint bigint::ONE(1);
const bigint bigint::TWO(2);
const bigint bigint::TEN(10);

//加法
bigint bigint::add(const bigint &num) const {
    bigint copy(*this);
    //同号
    if (copy.negative == num.negative) {
        Data &result = copy.data;
        int len = static_cast<int>(num.data.size() - copy.data.size());
        //填充0
        while ((len--) > 0) {
            result.emplace_back(0);
        }
        //按位相加
        Base carry = 0;
        for (size_t i = 0; i < num.data.size(); i++) {
            Base temp = result[i];
            result[i] = result[i] + num.data[i] + carry;
            carry = temp > result[i] ? 1 : temp > (temp + num.data[i]) ? 1 : 0;
        }
        for (size_t i = num.data.size(); i < copy.data.size() && carry != 0; i++) {
            Base temp = result[i];
            result[i] = (result[i] + carry);
            carry = temp > result[i] ? 1 : 0;
        }
        //如果还有进位则增加一位
        if (carry != 0) {
            result.push_back(static_cast<Base>(carry));
        }
        copy.trim();
    } else {//异号
        bool negative;
        auto absThis = this->abs();
        auto absNum = num.abs();
        if (absThis < absNum) {//绝对值小于num
            negative = num.negative;
        } else if (absThis == absNum) {//绝对值等于num
            negative = false;
        } else {//绝对值大于num
            negative = copy.negative;
        }
        copy.negative = num.negative;
        copy = copy.sub(num);
        copy.negative = negative;
    }
    return copy;
}

//减法
bigint bigint::sub(const bigint &num) const {
    bigint copy(*this);
    if (num.negative == copy.negative) {//同号
        Data &result = copy.data;
        if (!(this->abs() < num.abs())) {//绝对值大于num
            Base carry = 0;//借位
            //大数减小数
            for (std::size_t i = 0; i < num.data.size(); ++i) {
                bigint::Base temp = result[i];
                result[i] = (result[i] - num.data[i] - carry);
                carry = temp < result[i] ? 1 : temp < num.data[i] ? 1 : 0;
            }
            for (std::size_t i = num.data.size(); i < copy.data.size() && carry != 0; ++i) {
                bigint::Base temp = result[i];
                result[i] = result[i] - carry;
                carry = temp < carry ? 1 : 0;
            }
            copy.trim();
        } else {//绝对值小于num
            copy.data = num.sub(copy).data;
            copy.negative = !copy.negative;
        }
    } else {//异号
        bool negative = copy.negative;
        copy.negative = num.negative;
        copy = copy.add(num);
        copy.negative = negative;
    }
    return copy.abs() == ZERO ? ZERO : copy;
}

//乘法
bigint bigint::mul(const bigint &num) const {
    if (*this == (bigint::ZERO) || *this == bigint::ZERO) {
        return bigint::ZERO;
    }
    //TODO 非常慢的算法
//    bigint result(0);
//    bigint absThis = this->abs();
//    bigint absNum = num.abs();
//    const bigint &big = absThis > absNum ? absThis : absNum;
//    const bigint &small = (&big) == (&absThis) ? num : absThis;
//    for (size_t i = 0; i < small.bitLength(); i++) {
//        if (small.testBit(i)) {
//            result = result + (big << i);
//        }
//    }
//    result.negative = this->negative != num.negative;
//    return result;

    //借鉴了Java的BigInteger的实现
    Data x(this->data);
    Data y(num.data);
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());

    size_t xLen = x.size();
    size_t yLen = y.size();

    int xStart = static_cast<int>(xLen - 1);
    int yStart = static_cast<int>(yLen - 1);

    std::vector<Base> resultData(xLen + yLen);

    Base carry = 0;
    for (int j = yStart, k = yStart + 1 + xStart; j >= 0; j--, k--) {
        long long product = static_cast<long long>(y[j]) * x[xStart] + carry;
        resultData[k] = static_cast<Base>(product);
        carry = static_cast<Base>(product >> 32);
    }
    resultData[xStart] = carry;
    for (int i = xStart - 1; i >= 0; i--) {
        carry = 0;
        for (int j = yStart, k = yStart + 1 + i; j >= 0; j--, k--) {
            long long product = static_cast<long long>(y[j]) * x[i] + resultData[k] + carry;
            resultData[k] = static_cast<Base>(product);
            carry = static_cast<Base>(product >> 32);
        }
        resultData[i] = carry;
    }
    std::reverse(resultData.begin(), resultData.end());
    bigint result(resultData);
    result.negative = this->negative != num.negative;
    result.trim();
    return result;
}

//除法
bigint bigint::div(const bigint &num) const {
    assert(num != (bigint::ZERO));
    if (*this == num) {//绝对值相等
        return (this->negative == num.negative) ? bigint(1) : bigint(-1);
    } else if (this->abs() < num.abs()) {//绝对值小于
        return bigint::ZERO;
    } else {
        bigint quotient, remainder;
        div(*this, num, quotient, remainder);
        quotient.negative = this->negative != num.negative;
        return quotient.abs() == ZERO ? ZERO : quotient;
    }
}

//模运算
bigint bigint::mod(const bigint &num) const {
    assert(num != bigint::ZERO);
    if (*this == num) {
        return bigint::ZERO;
    } else if (this->abs() < num.abs()) {
        bigint result = *this;
        if (this->negative != num.negative) {
            if (this->negative && !num.negative) {
                result = result + num;
            }
            if (!this->negative && num.negative) {
                result = result - num;
            }
        }
        return result;
    } else {
        bigint quotient, remainder;
        div(*this, num, quotient, remainder);
        if (this->negative != num.negative) {
            if (this->negative && !num.negative) {
                remainder = remainder - num;
            }
            if (!this->negative && num.negative) {
                remainder = remainder + num;
            }
        }
        remainder.negative = num.negative;
        return remainder.abs() == ZERO ? ZERO : remainder;
    }
}

//左移
bigint bigint::leftShift(const unsigned int n) const {
    bigint copy(*this);
    int bases = n >> 5;//除以32
    int offset = n & 0x1F;//除32的余数
    //增加的位数
    int increment = (offset == 0) ? bases : 1 + bases;
    for (int i = 0; i < increment; ++i) {
        copy.data.push_back(0);
    }
    //如果需要增加位，则先向左移位增加位数
    if (bases) {
        int k = (offset == 0) ? 1 : 2;
        for (int i = static_cast<int>(copy.data.size() - k); i >= bases; --i) {
            copy.data[i] = copy.data[i - bases];
        }
        for (int i = 0; i < bases; ++i) {
            copy.data[i] = 0;
        }
    }
    //左移非Base整数倍的位数
    if (offset) {
        Base mask = 0xFFFFFFFF << (32 - offset);
        Base carry = 0;
        for (auto &item : copy.data) {
            Base temp = item;
            item = (temp << offset) | carry;
            carry = (temp & mask) >> (32 - offset);//最高offset位
        }
    }
    copy.trim();
    return copy;
}

//右移
bigint bigint::rightShift(const unsigned int n) const {
    bigint copy(*this);
    int bases = n >> (5);//除以32
    int offset = n & (0x1F);//除32的余数
    //如果需要减少位，则先向右移位减少位数
    if (bases) {
        for (int i = 0; i > bases; ++i) {
            copy.data[i] = copy.data[i + bases];
        }
        for (int i = 0; i < bases; ++i) {
            copy.data.pop_back();
        }
        if (copy.data.empty()) {
            copy.data.push_back(0);
        }
    }
    //右移非Base整数倍的位数
    if (offset) {
        Base mask = 0xFFFFFFFF >> (32 - offset);
        Base carry = 0;
        for (int i = static_cast<int>(copy.data.size() - 1); i >= 0; --i) {
            Base temp = copy.data[i];
            copy.data[i] = (temp >> offset) | carry;
            carry = (temp & mask) << (32 - offset);//最低offset位
        }
    }
    copy.trim();
    return copy;
}

//小于
bool bigint::lt(const bigint &num) const {
    if (this->negative != num.negative) {
        return this->negative;
    }
    bool absCompare = false;
    if (data.size() == num.data.size()) {
        bool finished = true;
        for (auto it1 = data.rbegin(), it2 = num.data.rbegin(); it1 != data.rend(); it1++, it2++) {
            if ((*it1) != (*it2)) {
                absCompare = (*it1) < (*it2);
                finished = false;
                break;
            }
        }
        if (finished) {
            return false;
        }
        //absCompare = finished ? false : absCompare;//相等
    } else {
        absCompare = data.size() < num.data.size();
    }
    return absCompare == !this->negative;
}

//小于等于
bool bigint::lte(const bigint &num) const {
    if (this->negative != num.negative) {
        return this->negative;
    }
    bool absCompare = false;
    if (data.size() == num.data.size()) {
        bool finished = true;
        for (auto it1 = data.rbegin(), it2 = num.data.rbegin(); it1 != data.rend(); ++it1, ++it2) {
            if ((*it1) != (*it2)) {
                absCompare = (*it1) < (*it2);
                finished = false;
                break;
            }
        }
        if (finished) {//相等
            return true;
        }
    } else {
        absCompare = data.size() < num.data.size();
    }
    return absCompare == !this->negative;
}

//大于
bool bigint::gt(const bigint &num) const {
    return !this->lte(num);
}

//大于等于
bool bigint::gte(const bigint &num) const {
    return !this->lt(num);
}

//等于
bool bigint::eq(const bigint &num) const {
    return (this->negative == num.negative) && this->data == num.data;
}

//不等于
bool bigint::neq(const bigint &num) const {
    return (this->negative != num.negative) || this->data != num.data;
}

bigint operator+(const bigint &a, const bigint &b) {
    return a.add(b);
}

bigint operator+(const bigint &a, const long b) {
    bigint t(b);
    return a.add(t);
}

bigint operator-(const bigint &a, const bigint &b) {
    return a.sub(b);
}

bigint operator-(const bigint &a, const long b) {
    bigint t(b);
    return a.sub(t);
}

bigint operator-(const bigint &num) {
    bigint copy(num);
    copy.negative = !num.negative;
    return copy;
}

bigint operator*(const bigint &a, const bigint &b) {
    return a.mul(b);
}

bigint operator*(const bigint &a, const long b) {
    bigint t(b);
    return a.mul(t);
}

bigint operator/(const bigint &a, const bigint &b) {
    return a.div(b);
}

bigint operator/(const bigint &a, const long b) {
    bigint t(b);
    return a.div(t);
}

bigint operator%(const bigint &a, const bigint &b) {
    return a.mod(b);
}

bigint operator%(const bigint &a, const long b) {
    bigint t(b);
    return a.mod(t);
}

bool operator<(const bigint &a, const bigint &b) {
    return a.lt(b);
}

bool operator<(const bigint &a, const long b) {
    bigint t(b);
    return a.lt(t);
}

bool operator<=(const bigint &a, const bigint &b) {
    return a.lte(b);
}

bool operator<=(const bigint &a, const long b) {
    bigint t(b);
    return a.lte(t);
}

bool operator>(const bigint &a, const bigint &b) {
    return a.gt(b);
}

bool operator>(const bigint &a, const long b) {
    bigint t(b);
    return a.gt(t);
}

bool operator>=(const bigint &a, const bigint &b) {
    return a.gte(b);
}

bool operator>=(const bigint &a, const long b) {
    bigint t(b);
    return a.gte(t);
}

bool operator==(const bigint &a, const bigint &b) {
    return a.eq(b);
}

bool operator==(const bigint &a, const long b) {
    bigint t(b);
    return a.eq(t);
}

bool operator!=(const bigint &a, const bigint &b) {
    return a.neq(b);
}

bool operator!=(const bigint &a, const long b) {
    bigint t(b);
    return a.neq(t);
};

bigint operator>>(const bigint &num, unsigned int n) {
    return num.rightShift(n);
}

bigint operator<<(const bigint &num, unsigned int n) {
    return num.leftShift(n);
}

std::ostream &operator<<(std::ostream &out, const bigint &num) {
    out << num.toString();
    return out;
}

//取绝对值
bigint bigint::abs() const {
    bigint copy(*this);
    copy.negative = false;
    return copy;
}

//求最大公约数
bigint bigint::gcd(const bigint &num) const {
    bigint a(*this);
    bigint b(num);
    bigint temp(0);
    while (b != bigint::ZERO) {
        temp = b;
        b = a % (b);
        a = temp;
    }
    return a;
}

//扩展欧几里得算法
std::vector<bigint> bigint::xgcd(const bigint &num) const {
    if (*this < ZERO || num < ZERO) {
        return std::vector<bigint>();
    }
    bigint a(*this), b(num);
    bigint temp;
    bigint q;
    bigint x1 = ONE;
    bigint y1 = ZERO;
    bigint x2 = ZERO;
    bigint y2 = ONE;
    while (b != ZERO) {
        q = a / b;
        temp = x2;
        x2 = x1 - q * x2;
        x1 = temp;
        temp = y2;
        y2 = y1 - q * y2;
        y1 = temp;
        //求GCD
        temp = b;
        b = a % b;
        a = temp;
    }
    std::vector<bigint> result{a, x1, y1};
    return result;
}

//互质
bool bigint::coprime(const bigint &num) const {
    return gcd(num) == ONE;
}

//幂
bigint bigint::pow(unsigned long long n) const {
    bigint result(1), base(*this);
    while (n) {
        if (n & 1) {
            result = result * base;
        }
        base = base * base;
        n >>= 1;
    }
    return result;
}

//Montgomery模幂
bigint bigint::modPow(const bigint &exp, const bigint &p) const {
    bigint a(*this), b(exp), m(p);
    bigint result = ONE;
    for (a = a % m; b != ZERO; b = (b >> 1)) {
        if (b.testBit(0)) {
            result = result * a % m;
        }
        a = a * a % m;
    }
    return result;
}

//模逆
bigint bigint::modInverse(const bigint &num) const {
    assert(!num.negative);//m为正数
    //如果不互质，则没有模逆元素
    //assert(coprime(num));
    bigint x = xgcd(num)[1];
    //取最小正整数
    while (x < ZERO) {
        x = x + num;
    }
    return x;
}

