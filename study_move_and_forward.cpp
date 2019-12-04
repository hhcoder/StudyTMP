#include <iostream>
#include <type_traits>

// 1. Move
//      std::move constructor and move assignment operators semantics gives control over the semantics of moving 
//      Q: what is move constructor?
//      Q: what is move assignment operator?
// 2. Perfect forwarding

// rvalue references are the glue ties these two features together (WHY???)


// A parameter is always an lvalue, even if its type is an rvalue reference
// Widget& is an rvalue <- no!
// lvalue is an address that can have a value assigned to it
// rvalue is an address that cannot have a value assigned to it

struct Widget{};
void f(Widget&& w){ }

// An lvalue designates a function or an object
// An xvalue (an "eXpiring" value) refers to an object, usually near the end of its lifetime
//      n xvalue is the result of certain kinds of expressions involving rvalue reference
// A glvalue (an "GeneraLized" lvaalue) is an lvalue or an xvalue
// An rvalue is an xvalue, a temporary object or subobject thereof, 
//      or a value that is not associated with an object

namespace Forward1 
{
    template <typename T, typename ... Args>        // <- parameter pack
        T Create(Args&& ... args)
        {
            return T(std::forward<Args>(args) ... );
        }
    
    // if the ellipse are left of Args, the parameter pack will be packed
    // if the ellipse are right of Args, the parameter pack will be unpacked.
    // Q: Does it has anything to do with lvalue and rvalue?


    void Exe()
    {
        std::cout << "=============" << "Forward1" << "==========" << std::endl;
        int five = 5;
        int myFive = Create<int>(five);
        std::cout << "myFive: " << myFive << std::endl;

        int myFive2 = Create<int>(5);
        std::cout << "myFive2: " << myFive2 << std::endl;

        using Pair = std::pair<int, double>;
        Pair myFive3 = Create<Pair>(3, 3.141597);
        std::cout << "myFive3: " << "(" << myFive3.first << "," << myFive3.second << ")" << std::endl;

        std::string hello_world_str("Hello World");
        std::string myStr1 = Create<std::string>(hello_world_str);
        std::cout << "myStr1: " << myStr1 << std::endl;

        // Ah, my, god!
        // this is my dreaming functionality!
        std::string myStr2 = Create<std::string>("Hello Perfect Forwarding");
        std::cout << "myStr2: " << myStr2 << std::endl;
    }
}

namespace Forward2
{
    template <typename T>
        decltype(auto)
        move(T&& param)
        {
            // what does it mean, 
            // remove_reference, then &&?
            // okay, if you don't remove reference, how do you know it's one & or two &s?
            // so this is a common operation. remove reference first, then &&
            using ReturnType = typename std::remove_reference_t<T>&&;

            return static_cast<ReturnType>(param);
        }

    void Exe()
    {
        std::cout << "=============" << "Forward2" << "==========" << std::endl;

        const int a = move(30);
        std::cout << "a: " << a << std::endl;

        int b = move(a);
        std::cout << "b: " << b << std::endl;

        int c = 40;
        int d = move(c);
        std::cout << "d: " << d << std::endl;
    }

}

int main(int argc, char* argv[])
{
    Forward1::Exe();

    Forward2::Exe();

    return 0;
}
