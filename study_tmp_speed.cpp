#include <iostream>
#include <chrono>

struct MeasureTime 
{
    MeasureTime(const std::string& in_title)
        : start(std::chrono::system_clock::now()),
          title(in_title){};
    ~MeasureTime()
    {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << title << ": " << diff.count() << " s" << std::endl;
    }
    decltype(std::chrono::system_clock::now()) start;
    std::string title;
};

namespace TemplateFxn
{
    template <int base, int exp>
    int Power()
    {
        auto result = 1;
        for (int i=0; i<exp; i++) result *= base;
        return result;
    }

    template <int base>
        int Power<base, 1>(){return 1;}
}

namespace TemplateStruct
{
    template <int base, int exp>
        struct Power : std::integral_constant<
           int, 
           base*Power<base, exp-1>::value> {};
    template <int base>
        struct Power<base, 0> : std::integral_constant<
           int, 1> {};
}

int main(int argc, char* argv[])
{
    {
        MeasureTime m("Template Function");
        int v;
        for (auto i=0; i<100000; i++)
            v = TemplateFxn::Power<3, 10>();
    }

    {
        MeasureTime m("Template Struct");
        int v;
        for (auto i=0; i<100000; i++)
            v = TemplateStruct::Power<3, 10>::value;
    }
    return 0;
}
