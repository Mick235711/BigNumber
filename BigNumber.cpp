//
// Created by Mick Li on 2019/9/20.
//

#include "BigNumber.hpp"

namespace BN
{
    inline void BigNumber::clean()
    {
        auto p = m_data.cbegin();
        while (p != m_data.cend() && *p == 0) ++p;
        m_data.erase(m_data.cbegin(), p);
        if (m_data.empty()) *this = BigNumber();
    }
    inline BigNumber::size_type BigNumber::digit_of(size_type n)
    {
        if (n == 0) return 1;
        size_type res = 0;
        while (n)
        {
            n /= 10;
            ++res;
        }
        return res;
    }
    inline BigNumber::BigNumber(number_type n)
    : BigNumber(std::to_string(n))
    {}
    BigNumber::BigNumber(const std::string& str)
    {
        if (str.empty())
        {
            m_sign = true;
            m_data.assign({0});
            return;
        }
        std::string t;
        if (str[0] == '-')
        {
            t = str.substr(1);
            m_sign = false;
        } else
        {
            t = str;
            m_sign = true;
        }
        // store num_digit digits in each element of m_data
        size_type len = t.size();
        m_data.resize(len / num_digit + 1);
        if (len < num_digit)
        {
            m_data[0] = std::stoull(t);
            return;
        }
        size_type i;
        for (i = len - num_digit; i >= num_digit; i -= num_digit)
            m_data[i / num_digit + 1] = std::stoull(t.substr(i, num_digit));
        m_data[1] = std::stoull(t.substr(i, num_digit));
        if (i == 0)
        {
            m_data.erase(m_data.cbegin());
            return;
        }
        m_data[0] = std::stoull(t.substr(0, i));
        clean();
    }

    BigNumber& BigNumber::operator+=(const BigNumber& b)
    {
        if (!b.m_sign) return *this -= -b;
        if (!m_sign)
        {
            *this = -*this;
            *this -= b;
            return *this = -*this;
        }
        BigNumber t(b);
        if (*this < t) swap(t);
        if (m_data.size() > t.m_data.size())
            t.m_data.insert(t.m_data.cbegin(), m_data.size() - t.m_data.size(), 0);
        size_type carry = 0;
        for (size_type i = m_data.size() - 1; i >= 0 && i != (size_type) - 1; --i)
        {
            m_data[i] += t.m_data[i] + carry;
            carry = m_data[i] / mod_value;
            m_data[i] %= mod_value;
        }
        if (carry) m_data.insert(m_data.cbegin(), carry);
        clean();
        return *this;
    }

    BigNumber& BigNumber::operator-=(const BigNumber& b)
    {
        if (!b.m_sign) return *this += -b;
        if (!m_sign)
        {
            *this = -*this;
            *this += b;
            return *this = -*this;
        }
        if (*this < b)
            return *this = -(b - *this);
        BigNumber t(b);
        if (m_data.size() > t.m_data.size())
            t.m_data.insert(t.m_data.cbegin(), m_data.size() - t.m_data.size(), 0);
        size_type borrow = 0;
        for (size_type i = m_data.size() - 1; i >= 0 && i != (size_type) - 1; --i)
            if (m_data[i] < borrow + t.m_data[i])
            {
                m_data[i] = mod_value + m_data[i] - borrow - t.m_data[i];
                borrow = 1;
            }
            else
            {
                m_data[i] -= borrow + t.m_data[i];
                borrow = 0;
            }
        clean();
        return *this;
    }

    BigNumber& BigNumber::operator*=(const BigNumber& b)
    {
        if (!b.m_sign)
            return *this = (-*this) * (-b);
        BigNumber t;
        t.m_data.resize(m_data.size() + b.m_data.size());
        for (size_type i = m_data.size() - 1; i >= 0 && i != (size_type)-1; --i)
            for (size_type j = b.m_data.size() - 1; j >= 0 && j != (size_type)-1; --j)
            {
                t.m_data[i + j + 1] += m_data[i] * b.m_data[j];
                t.m_data[i + j] += t.m_data[i + j + 1] / mod_value;
                t.m_data[i + j + 1] %= mod_value;
            }
        t.clean();
        swap(t);
        return *this;
    }

    BigNumber& BigNumber::operator/=(const BigNumber& b)
    {
        if (!b.m_sign)
            return *this = (-*this) / (-b);
        BigNumber c, f;
        c.m_data.resize(m_data.size());
        for (size_t i = 0; i < m_data.size(); ++i)
        {
            f *= mod_value;
            f += m_data[i];
            if (f < b) continue;
            BigNumber temp(b);
            size_type pow = 1;
            while (10 * temp <= f)
            {
                temp *= 10;
                pow *= 10;
            }
            while (pow > 0)
            {
                while (f >= temp)
                {
                    f -= temp;
                    c.m_data[i] += pow;
                }
                size_type carry = 0;
                for (auto& j : temp.m_data)
                {
                    j += carry;
                    carry = (j % 10) * mod_value;
                    j /= 10;
                }
                temp.clean();
                pow /= 10;
            }
            if (i > 0)
            {
                auto res = c.m_data[i] / mod_value;
                c.m_data[i] %= mod_value;
                c.m_data[i - 1] += res;
            }
        }
        c.clean();
        swap(c);
        return *this;
    }

    inline BigNumber& BigNumber::operator%=(const BigNumber& b)
    {
        return *this -= (*this / b) * b;
    }

    BigNumber& BigNumber::operator<<=(const BigNumber& b)
    {
        for (BigNumber i = 1; i <= b; ++i) *this *= 2;
        return *this;
    }

    BigNumber& BigNumber::operator>>=(const BigNumber& b)
    {
        for (BigNumber i = 1; i <= b; ++i) *this /= 2;
        return *this;
    }
    inline BigNumber& BigNumber::operator++() { return *this += 1; }
    inline BigNumber BigNumber::operator++(int) { BigNumber b = *this; operator++(); return b; }
    inline BigNumber& BigNumber::operator--() { return *this -= 1; }
    inline BigNumber BigNumber::operator--(int) { BigNumber b = *this; operator--(); return b; }

    inline BigNumber BigNumber::operator+() const {return *this;}
    inline BigNumber BigNumber::operator-() const
    {
        BigNumber b(*this);
        b.m_sign = !b.m_sign;
        return b;
    }

    inline BigNumber::operator bool() const
    {
        return !(operator !());
    }
    inline bool BigNumber::operator !() const
    {
        return m_data.size() == 1 && m_data[0] == 0;
    }

    inline BigNumber::number_type BigNumber::value() const { return std::stoll(str()); }
    inline BigNumber::operator number_type() const { return value(); }
    inline BigNumber::operator const std::string() const { return str(); }
    const std::string BigNumber::str() const
    {
        std::string str;
        for (const auto a : m_data)
            str += std::string(num_digit - digit_of(a), '0') + std::to_string(a);
        size_type i = 0;
        while (i < str.size() && str[i] == '0') ++i;
        if (i == str.size()) return "0";
        if (!m_sign) return '-' + str.substr(i);
        return str.substr(i);
    }
    BigNumber::number_type BigNumber::digit() const
    {
        auto s = str();
        auto res = s.size();
        if (s[0] == '-') --res;
        return res;
    }

    void BigNumber::swap(BigNumber& b) noexcept
    {
        using std::swap;
        swap(m_sign, b.m_sign);
        swap(m_data, b.m_data);
    }

    std::strong_ordering operator<=>(const BigNumber& b1, const BigNumber& b2)
    {
        if (!b1.m_sign && b2.m_sign) return std::strong_ordering::less;
        if (b1.m_sign && !b2.m_sign) return std::strong_ordering::greater;
        if (!b1.m_sign && !b2.m_sign) return -b2 <=> -b1;
        if (b1.m_data.size() < b2.m_data.size()) return std::strong_ordering::less;
        if (b1.m_data.size() > b2.m_data.size()) return std::strong_ordering::greater;
        return b1.m_data <=> b2.m_data;
    }
    inline BigNumber operator+(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res += b2;
        return res;
    }
    inline BigNumber operator-(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res -= b2;
        return res;
    }
    inline BigNumber operator*(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res *= b2;
        return res;
    }
    inline BigNumber operator/(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res /= b2;
        return res;
    }
    inline BigNumber operator%(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res %= b2;
        return res;
    }
    inline BigNumber operator<<(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res <<= b2;
        return res;
    }
    inline BigNumber operator>>(const BigNumber& b1, const BigNumber& b2)
    {
        BigNumber res(b1); res >>= b2;
        return res;
    }
    inline void swap(BigNumber& b1, BigNumber& b2) noexcept
    {
        b1.swap(b2);
    }
    inline const std::string to_string(const BigNumber& b)
    {
        return b.str();
    }
    std::istream& operator>>(std::istream& in, BigNumber& b)
    {
        std::string str;
        in >> str;
        b = BigNumber(str);
        return in;
    }
    inline std::ostream& operator<<(std::ostream& out, const BigNumber& b)
    {
        return out << b.str();
    }
    inline BigNumber operator ""_bn (const char* ch)
    {
        return BigNumber{ch};
    }
    inline BigNumber operator ""_bn (const char* ch, std::size_t len)
    {
        return BigNumber{std::string{ch, len}};
    }
} // namespace BN

namespace std
{
    template<>
    void swap<BN::BigNumber>(BN::BigNumber& b1, BN::BigNumber& b2) noexcept
    {
        b1.swap(b2);
    }
#if 0
    std::string to_string(BN::BigNumber& b)
    {
        return b.str();
    }
#endif // 0
}
