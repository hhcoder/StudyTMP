#include <iostream>
#include <array>

namespace Fundamentals
{
    void ReplaceOriginalArray()
    {
        int c_style_array[5] = {1, 4, 6, 4, 1};

        std::cout << "C style" << std::endl;
        for (int i=0; i<5; i++)
            std::cout << c_style_array[i] << ",";
        std::cout << std::endl;

        std::array<int, 5> std_array = { 1, 4, 6, 4, 1};
        std::cout << "STD Lib style" << std::endl;
        for (auto i:std_array)
            std::cout << i << ",";
        std::cout << std::endl;

        std::cout << "self: " << typeid(std::array<int, 5>).name() << std::endl;
        std::cout << "reference of self: " << typeid(std::array<int, 5>&).name() << std::endl;
        std::cout << "value_type: " << typeid(std::array<int, 5>::value_type).name() << std::endl;
        std::cout << "size_type: " << typeid(std::array<int, 5>::size_type).name() << std::endl;
        std::cout << "reference: " << typeid(std::array<int, 5>::value_type&).name() << std::endl;

        std::cout << "========================" << std::endl;
    }

    template <typename value_type, size_t size>
        void CoutArray(std::array<value_type, size>& a)
        {
            for (auto i : a)
                std::cout << i << std::endl;
        }

    template <typename value_type>
       void CoutArray<typename value_type, 3>
       {
           // std::cout << "Ha ha ha, you cannot have an array of 3 items!" << std::endl;
       }

    void PassUnknownSizeArray()
    {
        std::array<int, 3> a = {1, 2, 3};
        CoutArray(a);

        std::array<double, 9> b = {0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.09, 1.23, 4.55};
        CoutArray(b);
    }

    void Exe()
    {
        ReplaceOriginalArray();

        PassUnknownSizeArray();
    }
}

int main(int argc, char* argv[])
{
    Fundamentals::Exe();

    return 0;
}
