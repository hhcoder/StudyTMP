#include <iostream>
#include <type_traits>

namespace StudyPerfectForward
{

namespace Try1
{
    template <typename T, typename Arg>
        T Create(Arg& a)
        { return T(a);
        }

    void Exe()
    {
        std::cout << "=== Try1 ===" << std::endl;
        int five = 5;
        int myFive = Create<int>(five);
        std::cout << "myFive: " << myFive << std::endl;

        // int myFive2 = Create<int>(5);
        // Compilation error: "cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’ "
    }
}

namespace Try2
{
    template <typename T, typename Arg>
        T Create(Arg& a)
        {
            return T(a);
        }

    template <typename T, typename Arg>
        T Create(const Arg& a)
        {
            return T(a);
        }

    void Exe()
    {
        std::cout << "=== Try2 ===" << std::endl;
        int five = 5;
        int myFive = Create<int>(five);
        std::cout << "myFive: " << myFive << std::endl;

        int myFive2 = Create<int>(5);
        std::cout << "myFive2: " << myFive << std::endl;

        // The problem is, 
        // 1) What if I have N args? each arg could be const or non-const, I have to have a pair of const v.s. non-const 
        //    for each arg
        // 2) What if the constructor needs an rvalue? <- what does it mean?
    }
}

namespace Try3
{
    template <typename T, typename Arg>
        T Create(Arg&& a)
        {
            return T(std::forward<Arg>(a));
        }
    // Universal reference:
    //  Arg&& a;
    // Okay, We need to read Chapter 5 of the book

    void Exe()
    {
        std::cout << "=== Try3 ===" << std::endl;
        int five = 5;
        int myFive = Create<int>(five);
        std::cout << "myFive: " << myFive << std::endl;

        int myFive2 = Create<int>(5);
        std::cout << "myFive2: " << myFive << std::endl;
    }
}

namespace Try4
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
        std::cout << "=== Try4 ===" << std::endl;
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

};

#include <array>

namespace HandleTypeCast
{
    template <typename T, typename ... Args>        // <- parameter pack
        T Create(Args&& ... args)
        {
            return T(std::forward<Args>(args) ... );
        }

    template <typename T>        // <- parameter pack
        struct Ctor
        {
            template <typename ... Args>
            static T Create(Args&& ... args)
            {
                return T(std::forward<Args>(args) ... );
            }
        };

    class MyType
    {
        public:
            MyType(const int ia)
                : a{ia, ia*ia, ia*ia*ia, ia*ia*ia*ia} {}

            void Print()
            {
                for (auto i:a)
                    std::cout << i << ",";
                std::cout << std::endl;
            }
        private:
            std::array<int, 4> a;
    };

    template <int Id, typename ObjType>
    struct ObjectWrapper
    {
        template <typename ...Args>
        static void* Create(Args&& ... args)
        { 
            ObjectWrapper* o = new ObjectWrapper;
            o->id = Id;
            o->obj = new ObjType(std::forward<Args>(args)...);

            return static_cast<void*>(o);
        }

        static bool IdCorrect(void* h)
        {
            ObjectWrapper* o = static_cast<ObjectWrapper*>(h);
            if (o->id == Id)
                return true;
            else
                return false;
        }

        static ObjType* CastToObj(void* h)
        {
            if (IdCorrect(h))
            {
                ObjectWrapper* o = static_cast<ObjectWrapper*>(h);
                return o->obj;
            }
            else
            {
                return nullptr;
            }
        }

        static void Destroy(void* h)
        {
            if (IdCorrect(h))
            {
                ObjectWrapper* o = static_cast<ObjectWrapper*>(h);
                
                delete o->obj;
                delete o;
            }
        }

        int id;
        ObjType* obj;
    };

    // Okay, after think it through, I believe this class is simply redundant
    //  I don't see any value to having this kind of class

    void Exe()
    {
        int a = Create<int>(4);

        int b = Ctor<int>::Create(3);

        MyType c = Ctor<MyType>::Create(5);
        c.Print();

        std::pair<int, double> d = Ctor<std::pair<int, double>>::Create(3, 3.1415);
        std::cout << "(" << d.first << "," << d.second << ")" << std::endl;

        void* h_mytype = ObjectWrapper<1024, MyType>::Create(6);
        MyType* m = ObjectWrapper<1024, MyType>::CastToObj(h_mytype);
        m->Print();
        ObjectWrapper<1024, MyType>::Destroy(h_mytype);

        int value = 1024;
        void* test = static_cast<void*>(&value);
        void* p = ObjectWrapper<1024, MyType>::CastToObj(h_mytype);
        if (nullptr==p)
            std::cout << "Something is wrong!" << std::endl;
    }
}

namespace SimplerWay
{
    // It's so hard to remember this... :(
    template <typename T, typename... Arg>
        void DoSomething(Arg&&... params)
        {
            T t(std::forward(params...));
        }

    void Exe()
    {
    }
}

int main(int argc, char* argv[])
{
    StudyPerfectForward::Try1::Exe();
    StudyPerfectForward::Try2::Exe();
    StudyPerfectForward::Try3::Exe();
    StudyPerfectForward::Try4::Exe();

    HandleTypeCast::Exe();

    return 0;
}

