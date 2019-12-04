#include <iostream>
#include <array>
#include <cmath>
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
    // Can I use auto in struct declaration?
    // static auto start;
    // The code above wont' compile
    // But, following code will
    decltype(std::chrono::system_clock::now()) start;
    std::string title;
};

namespace UseStdArray
{
    void Execute()
    {
        // Looks like it's just like a static array
        std::array<float, 10> cos_array;
        for (auto i=0; i<cos_array.size(); i++)
        {
            float v = static_cast<float>(i)/static_cast<float>(cos_array.size());
            cos_array[i] = cos(v*2*M_PI);
        }
        for (auto i:cos_array)
            std::cout << i << std::endl;

        // Can I do this?
        // float* p = &cos_array;
        // No, I cannot
        // But following line works
        float* p = &cos_array[0];
        for (int i=0; i<10; i++)
        {
            std::cout << *p << std::endl;
            p++;
        }
        // Of course, there's no point go back writing in C-Style 
    }
}

namespace UseConstExprVariable
{
    struct MyType
    {
        int a;
    };
    void Execute()
    {
        const auto v = 10;
        constexpr auto v2 = 10;
        std::array<MyType, v> my_type_array;
        std::array<MyType, v2> my_type_array_2;
        // Simply put, all constexpr objects are const, 
        //  but not all const objects are constexpr

        // If you want compilers to gaurantee that a variable has a ivalue
        //  that can be used in context requiring compile-time constants, the
        //  tool to reach for is constexpr.
    }
}

namespace UseConstExprFunc
{
    // template <int base, unsigned exp>
    //     struct power_struct : std::integral_constant<
    //        int,
    //        0 ? 1 : base * power_struct<base, exp-1>::value>
    // {};
    //
    // Okay, not sure how to write this part of code
    // constexpr int power_template(int base, unsigned exp)
    // {
    //     // return (exp==0 ? 1 : base * power(base, exp-1));
    //     return power_struct<base, exp>::value;
    // }

    constexpr int power_constexpr_func(int base, unsigned exp) noexcept
    {
        auto result = 1;
        for (unsigned i=0; i<exp; i++) result *= base;
        return result;
    }

    int power_runtime_func(int base, unsigned exp)
    {
        auto result = 1;
        for (unsigned i=0; i<exp; i++) result *= base;
        return result;
    }

    double SquareRunTime(double a)
    {
        return a*a;
    }

    constexpr double SquireConstExpr(double a) noexcept
    {
        return a*a;
    }

    void Execute()
    {
        int repeated_times = 500;
        {
            MeasureTime m("Time running SquareRunTime");
            int a = 1354;
            for (int i=0; i<repeated_times; i++)
                SquareRunTime(a);
        }

        {
            MeasureTime m("Time ruuning SquireConstExpr");
            for (int i=0; i<repeated_times; i++)
                SquireConstExpr(1354);
        }

        {
            MeasureTime m("Time running power_constexpr_func");
            for (int i=0; i<repeated_times; i++)
                power_constexpr_func(5, 100);
        }

        {
            MeasureTime m("Time running power_runtime_func");
            for (int i=0; i<repeated_times; i++)
                power_runtime_func(5, 100);
        }

        // {
        //     MeasureTime m("Time running power_template");
        //     for (int i=0; i<repeated_times; i++)
        //         power_template(5, 10);
        // }
    }

}

namespace PointConstExpr
{
    class Point
    {
        public:
            constexpr Point(double ix=0, double iy=0) noexcept : x(ix), y(iy){}
            constexpr double X() const noexcept { return x; }
            constexpr double Y() const noexcept { return y; }
            constexpr void setX(double ix) { x = ix; }
            constexpr double DistanceWithConstExpr() const noexcept { return std::sqrt(x*x+y*y); }
            double DistanceCalc() const { return std::sqrt(x*x+y*y); }
        private:
            double x;
            double y;
    };

    // constexpr 
    //     Point MidPoint(const Point& p1, const Point& p2) 
    //     {
    //         return { (p1.X()+p2.X())/2 + (p1.Y()+p2.Y()) / 2};
    //     }


    void Execute()
    {
        constexpr Point p(3.14, 2*3.14);

        // Following code will raise compiliation error flag
        // double d1;
        // double d2;
        // constexpr Point p2(d1, d2);

        constexpr auto t = p.X();
        const auto c = p.Y();
        // what does this mean?
        // constexpr const auto t = p.X();
        // okay, it won't let you compile :D
        {
            MeasureTime m("Distance with Const Expr - constexpr auto" );
            // constexpr Point p2;
            // p2.setX(3.45);
            for (int i=0; i<1000000; i++)
               constexpr auto dist = p.DistanceWithConstExpr();
            // std::cout << dist << std::endl;
        }

        {
            MeasureTime m("Distance with Const Expr - to auto" );
            // constexpr Point p2;
            // p2.setX(3.45);
            double dist;
            for (int i=0; i<1000000; i++)
               dist = p.DistanceWithConstExpr();
            std::cout << dist << std::endl;
        }

        {
            MeasureTime m("Distance with Calculation - to auto" );
            for (int i=0; i<1000000; i++)
                auto dist2 = p.DistanceCalc();
            // std::cout << dist2 << std::endl;
        }

        {
            MeasureTime m("Distance with Calculation - to const auto" );
            for (int i=0; i<1000000; i++)
                const auto dist2 = p.DistanceCalc();
            // std::cout << dist2 << std::endl;
        }
    }
}

namespace TemplateFxnConstExpr
{
    template <int N>
        struct GetSquare : std::integral_constant<
         int,
         N*N> {};

    constexpr int CalcDistance (int a, int b) 
    {
        return (a+b);
    }

    void Execute()
    {
        // constexpr int a;
        // std::cin >> a;
        // std::cout << "Square of " << a << "is: " << GetSquare<a>::value << std::endl;
        std::cout << GetSquare<CalcDistance(3, 3)>::value;
    }
}

template <typename FxnType, typename... Args>
    void MeasureFxnTime(const std::string& str, FxnType f, Args&&... args)
{
    MeasureTime m(str);
    auto iterations = 1000000;
    for (unsigned i=0; i<iterations; i++) 
        f(std::forward<decltype(args)>(args)...);
}

namespace DifferentRecursiveImplement 
{
    // Run time calculation implementation
    int FactorialCalc(int a)
    {
        if (a==0) return 1;
        else return FactorialCalc(a-1);
    }

    // Add constexpr in the front
    constexpr int FactorialConstExpr(int a) 
    {
        if (a==0) return 1;
        else return FactorialConstExpr(a-1);
    }

    // structure template default parameter implementation
    template <int a>
        struct FactorialStruct : std::integral_constant<
            int,
            a * FactorialStruct<a-1>::value>{};

    template <>
        struct FactorialStruct<0> : std::integral_constant<int, 1> {};

    // function template default parameter implementation
    template <int a>
        int FactorialTmpFxn()
        {
            return a * FactorialTmpFxn<a-1>();
        }

    template <>
        int FactorialTmpFxn<0>()
        {
            return 1;
        }

    void Execute()
    {
        MeasureFxnTime("FactorialCalc", FactorialCalc, 10);
        MeasureFxnTime("FactorialConstExpr", FactorialCalc, 10);

        {
            MeasureTime m("FactorialStruct");
            auto iterations = 1000000;
            int v;
            for (unsigned i=0; i<iterations; i++) 
                v = FactorialStruct<10>::value;
            std::cout << v;
            // wait.. why this one is so much faster?
        }

        {
            MeasureTime m("FactorialTmpFxn");
            int v;
            auto iterations = 1000000;
            for (unsigned i=0; i<iterations; i++) 
                v = FactorialTmpFxn<10>();
            std::cout << v; 
        }

        constexpr int a = 10;
        FactorialTmpFxn<a>();
    }
}

int main(int argc, char* argv[])
{
    UseConstExprVariable::Execute();

    UseConstExprFunc::Execute();

    PointConstExpr::Execute();

    TemplateFxnConstExpr::Execute();

    DifferentRecursiveImplement::Execute();

    return 0;
}
