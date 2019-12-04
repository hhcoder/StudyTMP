#include <iostream>
#include <string>

namespace ImplicitConversion
{
    class Foo
    {
        public:
        Foo(double v) : a(v) { std::cout << "double ctor" << std::endl; }
        Foo(std::string t) :str(t) { std::cout << "string ctor" << std::endl; }
        void Print(){ std::cout << "(" << a << "," << a*a << "," << a*a*a << ")" << std::endl; }
        Foo(int v) : a(v) { std::cout << "int ctor" << std::endl; }
        private:
        double a;
        std::string str;

    };

    struct Foo2
    {
        explicit Foo2(int v) : a(v) {};
        int a;
        void Print() { std::cout << a << std::endl; }
    };

    void FxnTakeFoo(Foo f)
    {
        f.Print();
    }

    void FxnTakeFoo2(Foo2 f)
    {
        f.Print();
    }

    void Exe()
    {
        Foo f(1.2);
        f.Print();

        Foo fint(3);
        fint.Print();

        FxnTakeFoo(f);

        FxnTakeFoo(4.4); //!! It's implicitly converted from int to Foo

        // Okay, so implicit conversion applies only to strictly defined type 
        FxnTakeFoo(std::string("helloworld")); 

        // it means, this will work
        FxnTakeFoo({"helloworld"});

        Foo2 f2(3.45);
        f2.Print();
        // Following code leads to compile error
        // FxnTakeFoo2(3.45);
    }
}

int main(int argc, char* argv[])
{
    ImplicitConversion::Exe();

    return 0;
}
