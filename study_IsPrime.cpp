#include <chrono>
#include <iostream>
#include <type_traits>

namespace TimeIsPrime
{
    namespace TMP
    {
        template <int p, int i> 
            struct IsPrimeImpl : std::integral_constant<int,
            ( (p%i) && IsPrimeImpl<p, i-1>::value )>{};

        template <int p>
            struct IsPrimeImpl<p, 1> : std::integral_constant<int,
            1>{};

        template <int i>
            struct IsPrime : std::integral_constant<int,
            IsPrimeImpl<i, i-1>::value>{};
    }

    namespace Func
    {
        int IsPrimeImpl(int p, int i)
        {
            if (i==1)
                return 1;
            else return (p%(i)) && IsPrimeImpl(p, i-1);
        }

        int IsPrime(int i)
        {
            return IsPrimeImpl(i, i-1);
        }
    }

    void ExecMetaProg()
    {
        auto start = std::chrono::system_clock::now();
        for (int i=0; i<50000; i++)
            TMP::IsPrime<331>::value;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to run TMP::IsPrime: " << diff.count() << " s" << std::endl;
    }

    void ExecFunc()
    {
        auto start = std::chrono::system_clock::now();
        for (int i=0; i<50000; i++)
            Func::IsPrime(331);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to run Func::IsPrime: " << diff.count() << " s" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    TimeIsPrime::ExecFunc();
    TimeIsPrime::ExecMetaProg();

    std::cout << "can you believe this? TMP is 1000 times faster!" << std::endl;
}
