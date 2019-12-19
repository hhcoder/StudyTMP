#include <iostream>
#include <vector>

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

namespace TemplateType
{
    template <typename T>
        struct MyContainer : std::vector<T>
        {
            template <typename... Args>
            MyContainer(Args&&... args)
                : std::vector<T>(std::forward<Args>(args)...){}
        };

    template <typename ContainerType>
        struct ContainerCtrl
        {
            ContainerCtrl(const ContainerType& in_c)
            : c(in_c){}

            typename ContainerType::reference operator[](typename ContainerType::size_type s) { return c.at(s); }

            ContainerType c;
        };

    void Exe()
    {
        // ContainerCtrl<int> my_ctrl(3);

        int init[] = {0, 1, 2, 3};
        MyContainer<int> my_container(&init[0], &init[3]);
        ContainerCtrl<MyContainer<int>> my_ctrl(my_container);

        std::cout << "2nd element is: " << my_ctrl[2] << std::endl;
    }
}

namespace TryTypeName
{
    template <typename T>
        struct C
        {
            typedef T value_type;
        };

    struct D : private C<int>
    {
        size_t SizeOfElement() const { return sizeof(value_type); }
        void Set(value_type v){}
    };

    void Exe()
    {
        D d;
        // d.Set("hello world");
        d.Set(100);
    }
}

namespace CanITeamplteTypeSelf
{
    // This is fine
    struct MyStruct
    {
        MyStruct* p_next;
    };

    // Th is fine
    struct MyStruct2
    {
        void DoSomething(MyStruct2& v) { std::cout << v.v << std::endl; }
        int v;
    };

    // This is fine, too
    template <typename T>
        void Dump(const T& t) { std::cout << t << std::endl; }

    template <typename T>
        struct MyStruct3
        {
            T* next;
        };

    void Exe()
    {
        MyStruct2 s;
        s.DoSomething(s);

        Dump(4.355);

        MyStruct3<double> m;

        // Will this even compile?
        // MyStruct3<MyStruct3> mm;
        // NO
        // Following error
        // "expected type, got ‘MyStruct3’"

        // So, you cannot struct template yourself, that's interesting :D
        //  because it's not even "a type" from compiler point of view
    }
}

int main(int argc, char* argv[])
{
    ReturnType::Exe();

    TemplateType::Exe();

    CanITeamplteTypeSelf::Exe();

    return 0;
}
