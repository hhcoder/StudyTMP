#include <iostream>

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
};

template <int n>
struct factorial : integral_constant<int, n * factorial<n-1>::value> {};

template <>
struct factorial<0> : integral_constant<int, 1> {};

int main(int argc, char* argv[])
{
    std::cout << "factorial<5>::value: " << factorial<5>::value << std::endl;

    // Following code won't work since it has to be constant
    // int my_try;
    // std::cin >> my_try;
    // std::cout << "fatorial<my_try>::value: " << factorial<my_try>::value << std::endl;

    return 0;
}
