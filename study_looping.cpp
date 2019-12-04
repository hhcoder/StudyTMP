#include <iostream>

// int signal1[];
// int signal2[];
//
// convolute(signal1, signal2, signal3)
// {
//     // for (i=0; i<signal3.length; i++)
//         sum = 0;
//         for (j=0; j<signal1.length; j++)
//             sum += signal1(1)*signal2(1+j)
//         signal3(1) = sum;
//         for (j=0; j<signal1.length; j++)
//             sum += signal1(2)*signal2(2+j);
//         signal3(2) = sum;
// }
//
// for (j=0; j<h; j++)
//     for (i=0; i<w; i++)
//         {
//             for (t=-k; t<=k; t++)
//                 for (s=-k; s<=k; s++)
//                 {
//                 }
//             o[j*w+i] = v;
//         }
//
// for (j=0; j<h; j++)
//     for (i=0; i<w; i++)
//     {
//         for (t=-k; t<k; t++)
//         {
//         }
//         o[j*w+i] = v;
//     }
//
// for (j=0; j<h; j++)
//     for (i=0; i<w; i++)
//     {
//         for(s=-k; s<k; s++)
//         {
//         }
//         o[j*w+i] = v;
//     }
//
// template <int total>
//     Loop
// {
//     for (i=0; i<total; i+=inner)
//         InnerLoop(i)
// }
//
// template <int c>
//     InnerLoop
// {
//     Process();
//     InnerLoop<c-1>
// }
//
// template <>
//     InnerLoop(1)
// {
//     Process();
// }

// array[1-9] rand()
// kernel[1-9] = [121242121]
//
// for loop for 4000*3000 times
//
// compare if expanding for loop matters?

#include <array>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <string>
#include <thread>

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

    void Foo(){}

    void Foo2(int a, int b){}

    void Fxn()
    {
        {
            MeasureTime m("Foo Run Time");
            Foo();
        }

        {
            MeasureTime m("Foo2 Run Time");
            Foo2(3, 5);
        }
    }
}

namespace ExpandFor
{
    // template <typename SignalType, typename KernelType>
    // void FilterNormalImplementation(SignalType signal, KernelType kernel, int im_width, int im_height)
    void FilterNormalImplementation(std::array<int, 9>& signal, std::array<int, 9>& kernel, int im_width, int im_height)
    {
        for (int j=0; j<im_height; j++)
            for (int i=0; i<im_width; i++)
            {
                auto sum = 0;
                for (auto j=0; j<3; j++)
                    for (auto i=0; i<3; i++)
                    {
                        sum += signal[j*3+i] * kernel[j*3+i];
                    }
            }
    }

    void FilterPlainExpandInnerLoop(std::array<int, 9>& signal, std::array<int, 9>& kernel, int im_width, int im_height)
    {
        for (int j=0; j<im_height; j++)
            for (int i=0; i<im_width; i++)
            {
                auto sum = signal[0] * kernel[0];
                sum += signal[1] * kernel[1];
                sum += signal[2] * kernel[2];
                sum += signal[3] * kernel[3];
                sum += signal[4] * kernel[4];
                sum += signal[5] * kernel[5];
                sum += signal[6] * kernel[6];
                sum += signal[7] * kernel[7];
                sum += signal[8] * kernel[8];
            }
    }

    void Foo(int idx, int region, int im_width, int im_height, std::array<int,9> signal, std::array<int, 9> kernel)
    {
        for (int j=idx*im_height/region; j<(idx+1)*im_height/region; j++)
            for (int i=0; i<im_width; i++)
            {
                auto sum = signal[0] * kernel[0];
                sum += signal[1] * kernel[1];
                sum += signal[2] * kernel[2];
                sum += signal[3] * kernel[3];
                sum += signal[4] * kernel[4];
                sum += signal[5] * kernel[5];
                sum += signal[6] * kernel[6];
                sum += signal[7] * kernel[7];
                sum += signal[8] * kernel[8];
            }
    }

    void FilterExpandInnerLoopToMultiThreading(const std::array<int, 9> signal, const std::array<int, 9> kernel, int im_width, int im_height)
    {
        std::array<std::thread, 4> th;

        for (int i=0; i<4; i++)
            th[i] = std::thread(Foo, i, 4, im_width, im_height, signal, kernel);

        for (int i=0; i<4; i++)
            th[i].join();
    }

    void Execute()
    {
        std::srand(std::time(nullptr));

        std::array<int, 9> signal;
        for (auto& i : signal)
        {
            int x = RAND_MAX;
            while(x>1024)
                x = std::rand()/(RAND_MAX/1024);
            i = x;
        }

        std::array<int, 9> kernel = {1, 2, 1, 2, 4, 2, 1, 2, 1};

        const int im_width = 4000;
        const int im_height = 3000;

        Profiling::FxnTime("FilterNormalImplementation", FilterNormalImplementation, signal, kernel, im_width, im_height);

        Profiling::FxnTime("FilterPlainExpandInnerLoop", FilterPlainExpandInnerLoop, signal, kernel, im_width, im_height);

        Profiling::FxnTime("FilterExpandInnerLoopToMultiThreading", FilterExpandInnerLoopToMultiThreading, signal, kernel, im_width, im_height);
    }

}


int main(int argc, char* argv[])
{
    ExpandFor::Execute();

    return 0;
}
