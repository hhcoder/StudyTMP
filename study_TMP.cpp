#include <iostream>
#include <type_traits>
#include <vector>
#include <cmath>

template <typename T> 
    struct SizeOfType
    // Notice that we cannot do this:
    //   struct SizeOfType : std::integral_constant<T, sizeof(T)>{}; 
    {
        static constexpr size_t value = sizeof(T);
    };

template <int N> struct Sum { 
    static constexpr int value = N + Sum<N-1>::value;
};

template <> struct Sum <1> { 
    static constexpr int value = 1; 
};

constexpr int product(int x, int y)
{
    return x*y;
}

template <int select> struct Op
{
    static constexpr int value = 30; 
};

template <> struct Op<2>
{
    static constexpr int value = 10; 
};

template <> struct Op<3>
{
    static constexpr int value = 50; 

};

template <typename T>
struct SizeTest
{
    constexpr static int value = sizeof(T);
    static_assert(value, "Size is > 2");
};

constexpr int factorial(int n)
{
    if (n==0)
        return 1;
    return n * factorial(n-1);
}

template <int N>
    struct ABSD 
    {
        int operator()(int a){ return abs(N-a); }
    };

namespace TryOutDecltype
{
    template <typename T>
        struct SizeOf : std::integral_constant<T, sizeof(T)>{ };

    inline constexpr unsigned int pow(const unsigned int base, const unsigned int exponent)
    {
        return (exponent==0) ? 1 : (base * pow(base, exponent-1));
    }

    template <typename T>
        struct CapacityOf : std::integral_constant<int, pow(2, 8*SizeOf<T>::value)> 
    {
    };

    template <typename PixelType>
    struct Histogram : std::vector<unsigned int>
    {
        Histogram()
            : std::vector<unsigned int>(CapacityOf<PixelType>::value)
        {
            std::cout << "Size of histogram: " << size() << std::endl;
        }

        void Clear()
        {
            for(auto i : this)
                i = 0;
        }

        void Calculate(const PixelType* in_buf, const size_t len)
        {
            Clear();

            size_t i = len;
            const PixelType* p = in_buf;
            while( i!=0)
            {
                at(*p)++;
                p++;
                i--;
            }
        }
    };

    struct MyTest
    {
        MyTest(int in_a) : a(in_a){}
        int a;
    };

    void Exe()
    {
        Histogram<uint8_t> hist_a;

        std::cout << "pow(3,3) : " << pow(2, 8) << std::endl;

        // Okay, first usage is easy
        decltype(hist_a) hist_b;

        // What if I don't have a default constructor?
        MyTest t1(9);

        // Okay, following statement won't pass since decltype(t1) is equivalient to say the keyword "MyTest"
        // decltype(t1) t2;
        decltype(t1) t2(0);

        // can I use unknown value for constexpr declaration?
        // int a, b;
        // std::cin >> a;
        // std::cin >> b;
        // std::cout << "pow(a,b) : " << pow(a, b) << std::endl;
        int a;
    }
}

namespace StudyCppCon15
{
    template <typename T> struct HasSort
    {
        using yes = char;
        // Notice that the typedef version is like this: typedef char no[2];
        using no = char[2];
        template <typename U> static auto test(void*)->decltype(&T::sort, yes());
        template <typename U> static no& test(...);

        enum{ value = (
                       sizeof(HasSort::test<T>(NULL)) == 
                       sizeof(HasSort::yes)
                       ) };

    };

    struct Point
    {
        int x;
        int y;
    };


    // Perhaps the primary use for decltype is declaring function templates where the 
    //  function's return type depends on its parameter type.
    // Okay, what does this mean? I of course can return parameter type with T?
    // The type returned by a container's operator[] depends on the container
    template <typename Container, typename Index>
        decltype(auto) authAndAccess(Container& c, Index i)
            // -> decltype(c[i]) // <- we don't need this line after C++ 14
        {
            // authenticateUser();
            return c[i];
        };
    // In here, you have to use "decltype" in order to say to the compiler: "the return type
    // of this method is the return type of method [] of object Container
    // The "auto" keyword used to say to the compiler: "the return type of this function is declared at the end".
    //  ^
    //  |-- the "trailing return type" syntax

    struct MyStrangeContainer
    {
        int& operator[](int idx){ return a; }
        int a;
    };

    void Exe()
    {
        // std::cout << "size of yes: " << sizeof(StudyCppCon15::HasSort<int>::yes) << std::endl;
        // std::cout << "size of no: " << sizeof(StudyCppCon15::HasSort<int>::no) << std::endl;

        // std::cout << test() << std::endl;

        // std::cout << StudyCppCon15::HasSort<int>::value << std::endl;

        // No, I cannot
        // decltype(int a) can_I_do_this;

        // However, I can still do this:
        // So, what is decltype, exactly? Why does compiler says it's an expression?
        decltype(Point::x) xx;

        std::vector<int> v;
        // decltype(std::vector<int>::size_type);

        for (int i=0; i<10; i++)
            v.push_back(i);

        authAndAccess(v, 3) = 50;
        // std::cout << "index 30 of v is: " << i << std::endl;

        for (auto it : v)
            std::cout << it << std::endl;

        MyStrangeContainer ms;

        auto is = authAndAccess(ms, 1024);
        std::cout << "index 1024 of ms is: " << is << std::endl;

        std::cout << "typeid(is).name: " << typeid(ms).name() << std::endl;
        std::cout << "typeid(authAndAccess).name(): " << 
        typeid(authAndAccess<std::vector<int>, size_t>).name() << std::endl;

        auto it = v.begin();
        std::cout << "typeid(it).name: " << typeid(it).name() << std::endl;
        std::cout << "typeid(*it).name: " << typeid(*it).name() << std::endl;
        std::cout << "typeid(&it).name: " << typeid(&it).name() << std::endl;

        // Notice that we cannot declare this:
        // decltype(int) xb;

        int array[] = {1, 3, 5, 7, 9};
        int j=0;
        for (auto i=array; j<5; i++, j++)
            std::cout << i << std::endl;

        const int aaa = 3;
        auto bbb = aaa;

        std::cout << "typeid(aaa).name(): " << typeid(aaa).name() << std::endl;
        std::cout << "typeid(bbb).name(): " << typeid(bbb).name() << std::endl;

        bbb = 5; // Notice that here bbb is, in fact, int, not const int

        decltype(aaa) ccc = aaa;

        // Following line won't compile since decltype ... DOES WHAT? :(
        // ccc = 5;
    }
}


namespace TakeALookOnRLValue
{
    // lvalue
    // rvalue
    // glvalue  <- new
    // xvalue   <- new
    // prvalue  <- new

    // the result of every C++ expression is either a lvalue or a rvalue
    // lvalue: lvalue and glvalue
    // glvalue: Generalized lvalue (something could be either xvalue or lvalue)
    // rvalue: values that's can be on the Right side of an assignment (so expression is rvalue)
    // rvalue: refer to things either xvalue or prvalue
    // lvalue: things that can be on the Left side of an assignment
    //          for example, name of a variable (???)
    //          A l; <- l is an lvalue of type A
    //          A* pl; <- where *pl is an lvalue of type A
    //      Is it something like: *pl = A; ?
    //          A& r1 = v1;
    //          &r1 is an lvalue, v1 is an rvalue
    struct B{
        int a;
        int b;
    };
    // lvalue:
    //          B b;
    //          b.b; <- accessing a member of an object where the object expression is 
    //                  an lvalue is also an lvalue
    //          B& c = b;       // you cannot do this B& c; since no initialization 

    // string literal are lvalues

    B& f2(){ 
        static B local_b;
        return local_b; 
    }

    int f3(int& v){ return v; }

    void Exe()
    {
        B& c = f2();
        // this is lvalue
        const char* hello = "hello";
        // so we can re-assign it
        hello = "world";
        // how about this?
        const char world[] = "world";
        // Notice that we cannot assign hello to world:
        // hello = world; // compilation error

        // A named object declared with an rvalue reference is also an lvalue

        // Okay, so what?
        
        // 1. const and lvalue/rvalue are two separate concepts
        // 2. static constant value cannot have its reference - no, it cannot!
        // int a = 2;
        // int b = f3(a);

    }
}


int main(int argc, char* argv[])
{
    std::cout << "size of int: " << SizeOfType<int>::value << std::endl;

    std::cout << "Adding 1 to 10: " << Sum<10>::value << std::endl;

    std::cout << "product of 35 and 98: " << product(35, 98) << std::endl;

    int a = 66;
    int b = 99;

    // std::cin >> a;
    // std::cin >> b;
    std::cout << "product of a and b: " << product(a, b) << std::endl;

    //wait... so you can write if-else with template?

    std::cout << "1: " << Op<1>::value << std::endl;
    std::cout << "2: " << Op<2>::value << std::endl;
    std::cout << "3: " << Op<3>::value << std::endl;

    SizeTest<char> my_char;

    // So following line will introduce compilation time error
    //SizeTest<int> my_int;

    std::cout << "factorial of 10: " << factorial(10) << std::endl;

    ABSD<10>()(6);

    TryOutDecltype::Exe();

    StudyCppCon15::Exe();

    TakeALookOnRLValue::Exe();

    return 0;
}
