#ifndef BIGINT_H
#define BIGINT_H

#include <cctype>
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>

typedef unsigned char byte;
typedef std::vector<byte> bytes;

class bigint {
public:
    typedef unsigned int Base;//32位的基本元
    typedef std::vector<Base> Data;//数据

public:
    static const bigint ZERO;
    static const bigint ONE;
    static const bigint TWO;
    static const bigint TEN;

public://构造函数，赋值函数
    bigint() : negative(false) { data.push_back(0); }

    bigint(const std::string &bcd) : data(), negative(false) {
        fromBCD(bcd);
        trim();
    }

    bigint(long long num) : negative(false) { fromNumber(num); }

    explicit bigint(Data &data) : data(std::move(data)), negative(false) { trim(); }

    bigint(const bigint &num, bool negative) : data(num.data), negative(negative) {}

    bigint &operator=(std::string &bcd) {
        data.clear();
        negative = false;
        fromBCD(bcd);
        trim();
        return *this;
    }

    bigint &operator=(long long num) {
        data.clear();
        fromNumber(num);
        return *this;
    }

public:
    /**
     * 从Hex构建
     * @param hex Hex格式的字符串
     */
    static bigint fromHex(const std::string &hex) {
        bigint result;
        std::string str(hex);
        //负数
        if (str.length() && str.at(0) == '-') {
            if (str.length() > 1) {
                result.negative = true;
            }
            str = str.substr(1);
        }
        //4字节对齐
        int count = static_cast<int>(8 - (str.length() % 8));
        std::string temp;
        for (int i = 0; i < count; ++i) {
            temp.push_back(0);
        }
        str = temp + str;
        //转换
        for (int i = 0; i < str.length(); i += 8) {
            Base sum = 0;
            for (int j = 0; j < 8; ++j) {
                char ch = str[i + j];
                ch = hex2num(ch);
                sum = ((sum << 4) | (ch));
            }
            result.data.push_back(sum);
        }
        reverse(result.data.begin(), result.data.end());
        return result.trim();
    }

    /**
     * 获取位数
     * @return 位数
     */
    size_t bitLength() const {
        Base highest = data[data.size() - 1];//最高位
        size_t bitLen = data.size() << 5;
        if (highest == 0) {
            bitLen -= 32;
        } else {
            auto temp = static_cast<Base>(1 << (32 - 1));
            while (!(highest & temp)) {
                --bitLen;
                temp = temp >> 1;
            }
        }
        return bitLen;
    }

    /**
     * 测试特定的位是否是1
     * @param pos 位数
     * @return 是否是1
     */
    bool testBit(size_t pos) const {
        std::size_t index = pos >> 5;
        if (index >= data.size()) {
            return false;
        }
        std::size_t off = pos & 0x1F;
        return static_cast<bool>(data[index] & (1 << off));
    }

    /**
     * 转换为十进制字符串
     * @return 十进制字符串
     */
    std::string toString() const {
        if (*this == ZERO) return "0";
        bigint copy(*this, false);
        std::string str;
        while (copy > ZERO) {
            str.push_back((copy % TEN).lowestByte() + '0');
            copy = copy.div(TEN);
        }
        reverse(str.begin(), str.end());
        return (negative ? "-" : "") + str;
    }

    /**
     * 转换为Hex字符串
     * @return Hex字符串
     */
    std::string toHexString() const {
        static char hex[] = {
                '0', '1', '2', '3',
                '4', '5', '6', '7',
                '8', '9', 'A', 'B',
                'C', 'D', 'E', 'F'
        };
        std::string str;
        Base T = 0x0F;
        for (auto ch : data) {
            for (int j = 0; j < 8; ++j) {
                str.push_back(hex[ch & (T)]);
                ch = ch >> 4;
            }
        }
        reverse(str.begin(), str.end());
        str = str.substr(str.find_first_not_of('0'));
        return (negative ? "-" : "") + str;
    }

public://基本的数学运算
    bigint add(const bigint &num) const;

    bigint sub(const bigint &num) const;

    bigint mul(const bigint &num) const;

    bigint div(const bigint &num) const;

    bigint mod(const bigint &num) const;

    bigint leftShift(unsigned int n) const;

    bigint rightShift(unsigned int n) const;

    bool lt(const bigint &num) const;

    bool lte(const bigint &num) const;

    bool gt(const bigint &num) const;

    bool gte(const bigint &num) const;

    bool eq(const bigint &num) const;

    bool neq(const bigint &num) const;

    friend bigint operator+(const bigint &a, const bigint &b);

    friend bigint operator+(const bigint &a, long b);

    friend bigint operator-(const bigint &a, const bigint &b);

    friend bigint operator-(const bigint &a, long b);

    friend bigint operator-(const bigint &num);

    friend bigint operator*(const bigint &a, const bigint &b);

    friend bigint operator*(const bigint &a, long b);

    friend bigint operator/(const bigint &a, const bigint &b);

    friend bigint operator/(const bigint &a, long b);

    friend bigint operator%(const bigint &a, const bigint &b);

    friend bigint operator%(const bigint &a, long b);

    friend bool operator<(const bigint &a, const bigint &b);

    friend bool operator<(const bigint &a, long b);

    friend bool operator<=(const bigint &a, const bigint &b);

    friend bool operator<=(const bigint &a, long b);

    friend bool operator>(const bigint &a, const bigint &b);

    friend bool operator>(const bigint &a, long b);

    friend bool operator>=(const bigint &a, const bigint &b);

    friend bool operator>=(const bigint &a, long b);

    friend bool operator==(const bigint &a, const bigint &b);

    friend bool operator==(const bigint &a, long b);

    friend bool operator!=(const bigint &a, const bigint &b);

    friend bool operator!=(const bigint &a, long b);

    friend bigint operator>>(const bigint &num, unsigned int n);

    friend bigint operator<<(const bigint &num, unsigned int n);

    friend std::ostream &operator<<(std::ostream &out, const bigint &num);

    /**
     * 绝对值
     * @return
     */
    bigint abs() const;

    /**
     * 最大公约数
     * @param num
     * @return
     */
    bigint gcd(const bigint &num) const;

    /**
     * 扩展欧几里得算法求乘法逆元
     * @param num
     * @return 第一个为gcd，第二个为x(逆元)，第三个为y
     */
    std::vector<bigint> xgcd(const bigint &num) const;

    /**
     * 是否互质
     * @param num
     * @return
     */
    bool coprime(const bigint &num) const;

    bigint pow(unsigned long long n) const;

    bigint modPow(const bigint &exp, const bigint &p) const;

    bigint modInverse(const bigint &num) const;

private://辅助函数
    /**
     * 去除高位多余的0
     * @return
     */
    bigint &trim() {
        int count = 0;
        //检查不为0的元素的数量
        for (auto it = data.rbegin(); it != data.rend(); ++it) {
            if ((*it) == 0) {
                ++count;
            } else {
                break;
            }
        }
        //只有零的情况保留
        if (count == data.size()) {
            --count;
        }
        for (int i = 0; i < count; ++i) {
            data.pop_back();
        }
        return *this;
    }

    /**
     * 从BCD字符串构建
     * @param bcd BCD格式的字符串
     */
    void fromBCD(const std::string &bcd) {
        std::string str(bcd);
        //负数
        if (str.length() && str.at(0) == '-') {
            if (str.length() > 1) {
                negative = true;
            }
            str = str.substr(1);
        }
        bigint result(0);
        for (char ch : str) {
            result = result * 10;
            result = result + (ch - '0');
        }
        data = result.data;
    }

    /**
     * 从数字构建
     * @param num 数字
     */
    void fromNumber(long long num) {
        if (num < 0) {
            negative = true;
            num = -num;
        }
        do {
            auto ch = static_cast<Base>(num & 0xFFFFFFFF);
            data.push_back(ch);
            num = num >> 32;
        } while (num);
    }

    /**
     * 将Hex字符转换为数字
     * @param ch Hex字符
     * @return 数字
     */
    static char hex2num(char ch) {
        static char table[] = {
                0x0, 0x1, 0x2, 0x3,
                0x4, 0x5, 0x6, 0x7,
                0x8, 0x9, 0xa, 0xb,
                0xc, 0xd, 0xe, 0xf
        };
        if (isdigit(ch)) {
            ch -= '0';
        } else if (islower(ch)) {
            ch -= 'a' - 10;
        } else if (isupper(ch)) {
            ch -= 'A' - 10;
        }
        return table[ch];
    }

    /**
     * 除法
     * @param dividend 被除数
     * @param divisor  除数
     * @param quotient 商
     * @param remainder 余数
     */
    static void div(const bigint &dividend, const bigint &divisor, bigint &quotient, bigint &remainder) {
        //1.复制dividend,divisor
        bigint copyOfDivisor(divisor, false);
        remainder.negative = false;
        remainder.data = dividend.data;
        while (true) {
            int len = static_cast<int>(remainder.bitLength() - copyOfDivisor.bitLength());
            bigint temp;
            //找到移位的
            while (len >= 0) {
                temp = copyOfDivisor << len;
                if (temp.abs() <= remainder.abs()) {
                    break;
                }
                len--;
            }
            if (len < 0) {
                break;
            }
            Base n = 0;
            while (temp.abs() <= remainder.abs()) {
                remainder = remainder - temp;
                n++;
            }
            bigint kk(n);
            if (len) {
                kk = kk << len;
            }
            quotient = quotient.add(kk);
        }
        quotient.trim();
    }

    /**
     * 取最低字节
     * @return 最低字节
     */
    char lowestByte() const {
        return static_cast<char>(data.empty() ? 0 : data[0] & 0xFF);
    }

private:
    Data data;//数据
    bool negative;//符号位
};

#endif//BIGINT_H