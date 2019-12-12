#include <iostream>

namespace
{
    void PrintHelloWorld()
    {
        std::cout << "hello world" << std::endl;
    }

    struct ThisStaticCannotDo
    {
        ThisStaticCannotDo(const int v)
            : a(v/2) {}

        void Print() { std::cout << a << std::endl; }
        int a;
    };
};

int main(int argc, char* argv[])
{
    PrintHelloWorld();

    ThisStaticCannotDo s(105);
    s.Print();

    return 0;
}
