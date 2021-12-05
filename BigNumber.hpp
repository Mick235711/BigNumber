//
// Created by Mick Li on 2019/9/20.
//

#ifndef BIGNUMBER_BIGNUMBER_HPP
#define BIGNUMBER_BIGNUMBER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <compare>

namespace BN
{
    class BigNumber
    {
    public:
        // type definitions
        using size_type = unsigned long long;
        using number_type = long long;
        using class_type = BigNumber;

    private:
        // type & data definitions
        using data_type = std::vector<size_type>;
        static constexpr size_type num_digit = 9;
        static constexpr size_type mod_value = 1e9;

    private:
        // members
        bool m_sign; // 1 -> positive, 0 -> negative
        data_type m_data;

    private:
        // functions
        void clean();
        static size_type digit_of(size_type);

    public:
        // constructors
        explicit(false) BigNumber(number_type n = 0);
        explicit BigNumber(const std::string&);
        BigNumber(const BigNumber&) = default;
        BigNumber(BigNumber&&) noexcept = default;
        ~BigNumber() noexcept = default;

    public:
        // operators
        BigNumber& operator=(const BigNumber&) & = default;
        BigNumber& operator=(BigNumber&&) & noexcept = default;

        // +=, -=, *=, /=, %=
        BigNumber& operator+=(const BigNumber&);
        BigNumber& operator-=(const BigNumber&);
        BigNumber& operator*=(const BigNumber&);
        BigNumber& operator/=(const BigNumber&);
        BigNumber& operator%=(const BigNumber&);

        // <<=, >>=
        BigNumber& operator<<=(const BigNumber&);
        BigNumber& operator>>=(const BigNumber&);

        // ++, --
        BigNumber& operator++();
        BigNumber operator++(int);
        BigNumber& operator--();
        BigNumber operator--(int);

        // +, - (unary)
        BigNumber operator+() const;
        BigNumber operator-() const;

        // == & <
        friend bool operator==(const BigNumber&, const BigNumber&) = default;
        friend std::strong_ordering operator<=>(const BigNumber&, const BigNumber&);

        // boolean operations
        explicit operator bool() const;
        bool operator !() const;

        // convert operations
        [[nodiscard]] number_type value() const;
        explicit operator number_type() const;
        explicit operator const std::string() const;
        [[nodiscard]] const std::string str() const;
        [[nodiscard]] number_type digit() const;

        // swap
        void swap(BigNumber&) noexcept;
    };

    // non-member operators
    std::strong_ordering operator<=>(const BigNumber&, const BigNumber&);
    BigNumber operator+(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator-(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator*(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator/(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator%(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator<<(const BigNumber& b1, const BigNumber& b2);
    BigNumber operator>>(const BigNumber& b1, const BigNumber& b2);
    void swap(BigNumber& b1, BigNumber& b2) noexcept;
    const std::string to_string(const BigNumber& b);

    // stream operators
    std::istream& operator>>(std::istream& in, BigNumber& b);
    std::ostream& operator<<(std::ostream& out, const BigNumber& b);

    // literals
    BigNumber operator ""_bn (const char*);
    BigNumber operator ""_bn (const char*, std::size_t);
} // namespace BN

namespace std
{
    // specializations
    template<>
    void swap<BN::BigNumber>(BN::BigNumber& b1, BN::BigNumber& b2) noexcept;
    // we are not allowed to modify std
#if 0
    std::string to_string(BN::BigNumber& b);
#endif // 0
}

#endif //BIGNUMBER_BIGNUMBER_HPP
