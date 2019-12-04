#include <iostream>
#include <type_traits>
#include <vector>
#include <limits>

void separator()
{
    std::cout << "=================================" << std::endl;
}

void linechange()
{
    std::cout << std::endl;
}

// How to check if all typename in template is convertible.
// template<typename T, typename ...Args>
// struct AllInts<T, Args...> {
//     static const bool value = std::is_convertible<T, int>::value && AllInts<Args...>::value;
// };

// How to use Numeric Limits
// template <typename T>
//     T FindMax(const std::vector<T>& vec)
// {
//     T largest = std::numeric_limits<T>::min();
//
//     for (auto i : vec)
//         if (*i>largest)
//             largest = *i;
//     return largest;
// }

// Using Trait concept to implement 
// is_void, is_pointer, is_convertible

namespace understand_trait_ver1
{
    // is_void
    template <typename T>
        struct is_void : std::false_type {};

    template <> 
        struct is_void<void> : std::true_type {};

    // is_pointer
    template <typename T>
        struct is_pointer : std::false_type {};

    template <typename T>
        struct is_pointer<T*> : std::true_type {};

    template <typename T>
        struct is_double : std::false_type {};

    template <>
        struct is_double<double> : std::true_type {};

    void execute()
    {
        separator();
        std::cout << "Implement trait with boolean" << std::endl;
        linechange();
        std::cout << std::boolalpha; 
        std::cout << "is_void<int> : " << is_void<int>::value << std::endl;
        std::cout << "is_void<void> : " << is_void<void>::value << std::endl;
        std::cout << "is_pointer<int>: " << is_pointer<int>::value << std::endl;
        std::cout << "is_pointer<int*>: " << is_pointer<int*>::value << std::endl;
        std::cout << "is_pointer<double*>: " << is_pointer<double*>::value << std::endl;
        std::cout << "std::is_floating_point<double>: " << std::is_floating_point<double>::value << std::endl;
        std::cout << "std::is_floating_point<float>: " << std::is_floating_point<float>::value << std::endl;
        std::cout << "std::is_floating_point<int>: " << std::is_floating_point<int>::value << std::endl;
        std::cout << "is_double<double>: " << is_double<double>::value << std::endl;
        std::cout << "is_double<float>: " << is_double<float>::value << std::endl;
        std::cout << "how about const? " << std::endl;
        std::cout << "is_double<const double>: " << is_double<const double>::value << std::endl;
        std::cout << "how about const in std lib? " << std::endl;
        std::cout << "std::is_floating_point<const double>: " << std::is_floating_point<const double>::value << std::endl;
        std::cout << "okay, how does STD library not affect by const? " << std::endl;
        std::cout << "we'll address this in ver6." << std::endl;
    }
};

namespace understand_trait_ver2
{
    template <typename T>
        struct is_my_data_type : std::false_type {};

    // is_my_data_type
    struct MyDataType{
        MyDataType() : a(3) {}
        int a;
    };

    template <>
        struct is_my_data_type<MyDataType> : std::true_type {};

    void execute()
    {
        separator();
        std::cout << "Implement trait with enum" << std::endl;
        linechange();
        std::cout << "is_my_data_type(int): " << is_my_data_type<int>::value << std::endl;
        std::cout << "is_my_data_type(MyDataType): " << is_my_data_type<MyDataType>::value << std::endl;
    }
}

namespace understand_trait_ver3
{
    enum select
    {
        halide_version,
        hvx_version,
        neon_version
    };

    template <select s>
        struct select_algo {
            static std::string implement(){ return "fall back to default"; }
        };

    template <>
        struct select_algo<halide_version>
        {
            static std::string implement(){ return "halide"; }
        };

    template <>
        struct select_algo<hvx_version>
        {
            static std::string implement(){ return "hvx"; }
        };

    void execute()
    {
        separator();
        std::cout << "Implement with enum" << std::endl;
        linechange();
        std::cout << "select_algo<halide_version>::implement(): " << select_algo<halide_version>::implement() << std::endl;
        std::cout << "select_algo<hvx_version>::implement(): " << select_algo<hvx_version>::implement() << std::endl;
        std::cout << "select_algo<neon_version>::implement(): " << select_algo<neon_version>::implement() << std::endl;
    }
}

namespace understand_trait_ver4
{
    // declaration session
    template <typename T>
    struct is_opt_supported : std::false_type {};

    template<bool b>
    struct algo_selector 
    {
        template <typename T>
            static std::string implement(T& a){ return a.normal_implementation(); }
    };

    template<> 
    struct algo_selector<true>
    {
        template <typename T>
            static std::string implement(T& a){ return a.optimized_implementation(); }
    };


    template <typename T>
    std::string run_algo(T& t)
    {
        return algo_selector<is_opt_supported<T>::value>::implement(t);
    }

    // implementation session
    struct HvxImplement 
    {
        static std::string optimized_implementation(){ return "optimized hvx"; }
    };

    struct ArmImplement
    {
        static std::string normal_implementation(){ return "un-optimized arm"; }
    };

    template <>
    struct is_opt_supported<HvxImplement> : std::true_type {};

    void execute()
    {
        separator();
        std::cout << "experiment on algo select" << std::endl;
        HvxImplement hvx;
        std::cout << "use HvxImplement object: " << run_algo(hvx) << std::endl;
        ArmImplement arm;
        std::cout << "use ArmImplement object: " << run_algo(arm) << std::endl;
    }
}

namespace understand_trait_ver5
{
    //Q: how to implement is_array?

    template <typename T>
        struct is_array : std::false_type {};

    template <typename T>
        struct is_array <T[]> : std::true_type {};

    void execute()
    {
        separator();
        std::cout << "is_array<int[]>::value: " << is_array<int[]>::value << std::endl;
        std::cout << "is_array<int*>::value: " << is_array<int*>::value << std::endl;
    }
}

namespace understand_trait_ver6
{
    template <typename T> 
        struct remove_const { typedef const T type; };

    template <typename T>
        struct is_float : std::false_type {};

    template <>
        struct is_float<float> : std::true_type {};

    template <>
        struct is_float<remove_const<float>::type> : std::true_type {};

    void execute()
    {
        separator();
        std::cout << "is_float<float>::value: " << is_float<float>::value << std::endl;
        std::cout << "is_float<const float>::value: " << is_float<const float>::value << std::endl;
    }
}

namespace understand_trait_ver7
{
    struct HelloWorld
    {
        int a, b, c;
    };

    template <typename T1, typename T2>
        decltype(auto) PrintIfSameType(T1 t1, T2 t2)
        {
            std::cout << 
            std::boolalpha <<
            std::is_same<
                decltype(t1),
                decltype(t2)>::value <<
            std::endl;
        }

    void execute()
    {
        separator();

        HelloWorld hw;

        unsigned a;
        PrintIfSameType(hw, a);

        unsigned b;
        PrintIfSameType(a, b);

        unsigned c;
        unsigned int d;
        PrintIfSameType(c, d);

        HelloWorld& rhw = hw;
        PrintIfSameType(rhw, hw);

        HelloWorld* phw = &hw;
        PrintIfSameType(phw, hw);

        const HelloWorld* cphw = &hw;
        PrintIfSameType(cphw, &hw);

        PrintIfSameType(cphw,
                static_cast<const HelloWorld*>(&hw));

    }
}

namespace understand_trait_ver8
{
    struct A
    {
        int a;
        int b;
        int c;
    };

    struct B
    {
        B(){}
    };

    struct C
    {
        C(int a, int b){}
    };
    struct D
    {
        int array[100];
        int a[33];
        double d[29];
    };
    struct E
    {
        virtual void PureVirtual() = 0;
    };
    struct F
    {
        F()=delete;
        ~F()=delete;
        void Set(int i) { a = i; }
        int a;
    };
    struct G : F
    {
        int b;
    };
    void execute()
    {
        separator();

        std::cout << std::boolalpha <<
        "is_trivial<A>::value: " << std::is_trivial<A>::value << std::endl <<
        "is_trivial<B>::value: " << std::is_trivial<B>::value << std::endl <<
        "is_trivial<int>::value: " << std::is_trivial<int>::value << std::endl <<
        "is_trivial<C>::value: " << std::is_trivial<C>::value << std::endl <<
        "is_trivial<D>::value: " << std::is_trivial<D>::value << std::endl <<
        "is_trivial<E>::value: " << std::is_trivial<E>::value << std::endl <<
        "is_trivial<F>::value: " << std::is_trivial<F>::value << std::endl <<
        "is_trivial<G>::value: " << std::is_trivial<G>::value << std::endl <<
        std::endl; 
    }
}

int main(int argc, char* argv[])
{
    understand_trait_ver1::execute();

    understand_trait_ver2::execute();

    understand_trait_ver3::execute();

    understand_trait_ver4::execute();

    understand_trait_ver5::execute();

    understand_trait_ver6::execute();

    understand_trait_ver7::execute();

    understand_trait_ver8::execute();
    return 0;
}
