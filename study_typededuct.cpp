#include <iostream>

namespace ReturnType
{

decltype(auto) NotSureIfThisWork(int a, int b)
{
    return a+b;
}

template <typename T1, typename T2>
void Func(T1& a, T2& b)
{
    a = a*b;
}

template <typename T1, typename T2>
void DoSomething(T1 a, T2 b)
{
    Func(a, b);
}

template <typename T1, typename T2>
void DoSomething2(T1& a, T2& b)
{
    Func(a, b);
}


template <typename T1, typename T2>
decltype(auto) DoSomethingToTwoTypes(T1&& a, T2&& b)
{
    return std::forward<T1>(a) * std::forward<T2>(b);
}

void Exe()
{
    std::cout << "178 + 399 = " << NotSureIfThisWork(178, 399) << std::endl;
    // Following line won't work
    std::cout << "199 * 33.678 = " << DoSomethingToTwoTypes(199, 33.678) << std::endl;
    int a = 3;
    int b = 5;
    std::cout << "a: " << a << std::endl;
    DoSomething(a, b);
    std::cout << "a: " << a << std::endl;
    DoSomething2(a, b);
    std::cout << "a: " << a << std::endl;
}

};

int main(int argc, char* argv[])
{
    ReturnType::Exe();

    return 0;
}
