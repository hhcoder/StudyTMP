#include <iostream>
#include <cmath>
#include <vector>

template <typename Container>
    void PrintAllElements(Container& c)
{
    std::cout << std::endl << "(";
    for (auto i = c.begin(); i!=c.end(); i++)
        std::cout << *i << ",";
    std::cout << ")" << std::endl;
}

#include <chrono>
#include <utility>

namespace Profiling
{
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

    template <typename FxnType, typename... Args>
        void FxnTime(const std::string& str, FxnType f, Args&&... args)
    {
        MeasureTime m(str);
        f(std::forward<decltype(args)>(args)...);
    }
}

// Pay attention to the "ForEach" syntax, it works for both FxnPtr and Functor!
template <typename InputContainer, typename OutputContainer, typename Operation, typename... Args>
    void ForEach(InputContainer& in_value, OutputContainer& out_value, Operation& op, Args&&... args)
{
    for (auto i = in_value.begin(), j=out_value.begin(); 
              i!=in_value.end(); 
              i++, j++)
        *j = op(*i, std::forward<decltype(args)>(args)...);
}

namespace FxnPtrApproach
{
    int MultiplyFloat(int ia, const float ratio)
    {
        return static_cast<int>(std::round(ia * ratio));
    }

    void Exe(std::vector<int>& in_value, std::vector<int>& out_value, const float ratio)
    {
        PrintAllElements(in_value);

        ForEach(in_value, out_value, MultiplyFloat, ratio);

        PrintAllElements(out_value);
    }
};

namespace FunctorApproach
{
    struct MultiplyFloatLut
    {
        MultiplyFloatLut(const float ratio)
            : lut(new int[1024])
        {
            for (int i=0; i<1024; i++)
                lut[i] = static_cast<int>(std::round(i*ratio));
        }

        ~MultiplyFloatLut()
        {
            delete[] lut;
        }

        int operator()(int in)
        {
            return lut[in];
        }

        int* lut;
    };

    void Exe(std::vector<int>& in_value, std::vector<int>& out_value, const float ratio)
    {
        PrintAllElements(in_value);

        MultiplyFloatLut lut(ratio);

        ForEach(in_value, out_value, lut);

        PrintAllElements(out_value);
    }
}


// How about Lambda function?

int main(int argc, char* argv[])
{
    std::vector<int> a = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

    std::vector<int> b(a.size());

    const float ratio = 2.37808;

    Profiling::FxnTime("Function Pointer Approach", FxnPtrApproach::Exe, a, b, ratio);

    Profiling::FxnTime("Functor Approach", FunctorApproach::Exe, a, b, ratio);

    // Can I do this?
    int sum = 0;
    for (auto i=0, j=0; i<1024; i++)
        sum += i;

    return 0;
}
