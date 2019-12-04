#include <iostream>

namespace FundamentalExplain
{
    // int& foo(){int a; return a;}
    void Exe()
    {
        int a;  // a is an l value
        a = 345; // 345 is an r value
        int* pa = &a; // a and pa are both lvalue
        int& ra = a;  // ra and pa are both lvalue

        std::cout << "ra: " << ra << std::endl;
        std::cout << "pa: " << pa << std::endl;

        // What does this mean? int& foo(); ???
        int& foo();

        foo() = 42;

    }
}

int main(int argc, char* argv[])
{
    FundamentalExplain::Exe();

    return 0;
}
