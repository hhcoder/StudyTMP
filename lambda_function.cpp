#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>

// one of the most exciting feature of c++ 11 is ability to create lambda functions

namespace BasicLambdaExpr
{
    void Exe()
    {
        auto lambda_func = []() { std::cout << "hello lambda" << std::endl; };

        lambda_func();

        // what an interesting to write a function!
        auto lambda_func_2 = [](int a, int b) 
        { 
            if (a>2)
                return a/2*b;
            else
                return 0;
                // return std::string("wrong!");
        };

        // No, you cannot write lambda like this
        // then, what is the type of lambda function?
        // int lambdat_func_3 = [](int a, int b) { return a*b; };

        std::cout << "3/2*5 = " << lambda_func_2(3, 5) << std::endl;

        // okay, first of all, there's no "return" type thanks to c++11 type deduction properties
    }
}

template <typename Container>
void PrintAll(Container& c)
{
    std::cout << "(";
    for (auto i : c)
        std::cout << i << ",";
    std::cout << ")" << std::endl;
}

// Variable capture is the real secret sauce that makes a lambda function great (what does that mean?)
namespace VariableCapture
{
    void Exe(std::vector<int>& my_vec)
    {
        PrintAll(my_vec);

        auto find_mean = [](std::vector<int> vec)
        { 
            std::sort(vec.begin(), vec.end());
            return vec[vec.size()/2];
        };

        auto mean = find_mean(my_vec);

        mean = mean * 3/2;

        // I thought this is considered bad coding exercise comparing 
        //   sending an mean into this function
        auto remove_mean = [&](std::vector<int>& vec) //&: variable capture
        {
            for (auto &i:vec)
                if(i<mean) i=0;
        };

        remove_mean(my_vec);

        PrintAll(my_vec);
    }

}

namespace LambdaAndForEach
{
    template <typename Container, typename Functor>
        void ForEach(Container& c, Functor f)
        {
            for (auto &i : c)
                i = f(i);
        }

    void Exe(std::vector<int>& my_vec)
    {
        PrintAll(my_vec);

        auto mean = 512;

        ForEach(
                my_vec,
                [&](int v){
                    if (v < mean)
                        return 0;
                    else
                        return v;
                });

        PrintAll(my_vec);
    }
};

namespace SomeExperiments
{
    // Can I write lambda in a class? what does this mean?
    struct MyLambdaTest
    {
        MyLambdaTest()
            : my_lambda([](){ std::cout << "my lambda test" << std::endl; }){}

        std::function<void()> my_lambda;
    };

    struct MyFunctor
    {
        int operator()(int a, int b) { return a-b; }
    };

    int Add(int a, int b) { return a+b; }

    void Exe()
    {
        // First of all, I can do this:
        [](){ std::cout << "I thought it's [](), isn't it? " << std::endl; };

        // I can also omit the () since there's no input parameter for this lambda fxn 
        []{ int a = 3; };

        // Then, I can directly call this lambda function by this, which is a very weird way to write code!
        []{ 
            std::cout << "just do something" << std::endl; 
        }();

        // So, I can write the following confusing code
        [](int a, int b) { std::cout << "(a: " << a << "," << "b: " << b << ")" << std::endl; }( 13, 29);

        // What the...

        // What type is a Lambda expression?

        // 1. std::function

        std::function<int (int, int)> func;
        if(func)
            std::cout << "func is declared!" << std::endl;
        else
            std::cout << "func is null" << std::endl;

        func = [](int a, int b){ return a*b; };

        std::cout << "func(3, 5) = " << func(3, 5) << std::endl;

        MyFunctor my_functor;
        func = my_functor;
        
        std::cout << "func(3, 5) = " << func(3, 5) << std::endl;

        // How!?
        // How come same std::function<int (int,int)> can take 
        // a) lambda function
        // b) functor
        // c) fxn ptr

        func = Add;
        std::cout << "func(3, 5) = " << func(3, 5) << std::endl;

        // (x,y)->(x^2+y^2)

        // Computable functions are fundamental concept within computer science and mathematics. 
        // 1) functions are anonymous
        // 2) uses functions of a single input 
        //   (this method, known as carraying, transforms a function that takes multiple arguments 
        //    into a chian of functions each with a single argument)
    }
}

// So, how are lambda closures implemented? 
// lambdas are implemented as a small class, and it overloads the operator()


int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr));

    std::vector<int> my_vec;
    for (int i=0; i<20; i++)
       my_vec.push_back(rand()%1024); 

    // BasicLambdaExpr::Exe();

    // VariableCapture::Exe(my_vec);

    LambdaAndForEach::Exe(my_vec);

    SomeExperiments::Exe();

    return 0;
}
