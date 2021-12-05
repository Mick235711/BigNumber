#include <iostream>
#include <chrono>
#include <limits>
#include "BigNumber.cpp"

using std::cout, std::string;
using std::chrono::duration_cast, std::chrono::system_clock, std::chrono::microseconds;
using BN::BigNumber, BN::operator ""_bn;

BigNumber quick_pow(const BigNumber& a, const BigNumber& b)
{
    if (b < 0) return 0;
    if (b == 0) return 1;
    if (b == 1) return a;
    BigNumber res = quick_pow(a, b / 2);
    res *= res;
    if (b % 2 == 1) res *= a;
    return res;
}

int main()
{
    BigNumber b1, b2 = "2342399999999999999999934"_bn, b{0};
    b1 = 3445345345ULL;
    cout << "b1 = " << b1 << '\n';
    cout << "b2 = " << b2 << '\n';
    cout << "b1 + b2 = " << b1 + b2 << '\n';
    cout << "b1 - b2 = " << b1 - b2 << '\n';
    cout << "b1 * b2 = " << b1 * b2 << '\n';
    cout << "b2 / b1 = " << b2 / b1 << ", b2 % b1 = " << b2 % b1 << '\n';
    cout << "b1 / b2 = " << b1 / b2 << ", b1 % b2 = " << b1 % b2 << '\n';
    cout << "1 << 444 = " << (1 << 444_bn) << '\n';
    cout << "93845405873409587340985730489570394857043987543 / 32423423 = "
         << (93845405873409587340985730489570394857043987543_bn / 32423423) << '\n';
    cout << std::boolalpha << "b1 < b2: " << (b1 < b2) << '\n';
    cout << "(bool)b1 = " << static_cast<bool>(b1) << '\n';
    cout << "!b2 = " << !b2 << '\n';
    cout << "!0 = " << !b << '\n';
    cout << static_cast<long long>((BigNumber(1) << 63) - 1) << " <> " << std::numeric_limits<long long>::max() << '\n';
    try
    {
        (void)static_cast<long long>((BigNumber(1) << 64));
    }
    catch (const std::exception& e)
    {
        cout << "Exception: " << e.what() << '\n';
    }
    auto start = system_clock::now();
    (void)(93845405873409587340985730489570394857043987543_bn / 32423423);
    auto end = system_clock::now();
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << "us" << '\n';
    cout << string().max_size() << '\n';
    cout << (1_bn << 1000) << '\n';
    cout << quick_pow(2, 1000) << '\n';
    start = system_clock::now();
    (void)(1_bn << 100000);
    end = system_clock::now();
    cout << "Time #1: " << duration_cast<microseconds>(end - start).count() << "us" << '\n';
    start = system_clock::now();
    (void)quick_pow(2, 100000);
    end = system_clock::now();
    cout << "Time #2: " << duration_cast<microseconds>(end - start).count() << "us" << '\n';
    cout << std::boolalpha << "Result: " << ((1_bn << 100000) == quick_pow(2, 100000)) << '\n';
    start = system_clock::now();
    (void)quick_pow(2, 1000000);
    end = system_clock::now();
    cout << "Time for 2^1000000 = " << duration_cast<microseconds>(end - start).count() << "us" << '\n';
    return 0;
}
