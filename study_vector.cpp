#include <memory>
#include <vector>
#include <iostream>
#include <numeric>

namespace VectorAllocator
{
    struct MyType
    {
        int a;
    };
    void Execute()
    {
        std::vector<MyType> t1;
        std::vector<MyType> t2 = t1;

        std::cout << "How about t1 == t2? :" << (t1 == t2) << std::endl;
        // Ah, my, they fixed this problem in c++17!
        // Following line won't compile if using C++17
        // typedef std::vector<int, std::allocator<double>> MyStrangeVector;
        typedef std::vector<int, std::allocator<int>> MyStrangeVector;
        MyStrangeVector my_strange_vector;

        my_strange_vector.push_back(3);
        my_strange_vector.push_back(5);
        my_strange_vector.push_back(11.8);

        for (auto i:my_strange_vector)
            std::cout << i << std::endl;

        auto p = &(my_strange_vector[2]);

        double s = static_cast<double>(*p);

        std::cout << "s: " << s << std::endl;

        std::cout << "size: " << my_strange_vector.size() << std::endl;

        std::cout << "value_type: " << sizeof(MyStrangeVector::value_type) << std::endl;
        std::cout << "sizeof(int): " << sizeof(int) << std::endl;
        std::cout << "sizeof(double): " << sizeof(double) << std::endl;

        double double_array[10];
        std::allocator<double>::pointer alloc_ptr = &(double_array[0]);
        // type_trait -> alloc_traits -> __pointer_type 
        // First,
        // typedef typename __pointer_type<value_type, allocator_type>::type pointer;
        // Second,
        // template <class _Tp, class _Dp, bool = __has_pointer_type<_Dp>::value>
        // struct __pointer_type
        // {
        //    typedef _LIBCPP_NODEBUG_TYPE typename _Dp::pointer type;
        // };
        // MyStrangeVector::pointer vec_ptr = &(double_array[0]);

        // Okay, although I don't really want to implement like this
        //  but here is something I don't understand...
        std::allocator<double>::pointer a;
        std::vector<int, std::allocator<int>>::pointer b;

        std::vector<int>::difference_type diff_vec;
        ptrdiff_t diff_default;

        diff_vec = diff_default;

        // a = b;

        std::vector<int> numbers_a = {1, 2, 3, 4, 5};

        // Don't know how to compilie in C++17?
        std::cout << "accumulate: " << std::accumulate(numbers_a.begin(), numbers_a.end(), 0) << std::endl;
        std::cout << "accumulate: " << std::accumulate(begin(numbers_a), end(numbers_a), 0) << std::endl;

        std::vector<int> numbers_b = {};
        std::cout << "accumulate: " << std::accumulate(numbers_a.begin(), numbers_a.end(), 0) << std::endl;

        // Let's take a look on iterator_traits

        int* ptr_int;
        std::iterator_traits<int*>::pointer iter_int;
        
        // which means, I shouldn't use int* any more? 
        // Should I always use iterator_traits<T>::pointer from now on?

        iter_int = ptr_int;

        // So, there's "iterator" 

        std::vector<int> numbers_c = {1, 2, 4, 6, 4, 2, 1};
        std::vector<int> numbers_d = {1, 2, 4, 6, 4, 2, 1};

        std::cout << std::boolalpha;
        std::cout << "what's the result numbers_c == numbers_d? :" << (numbers_c == numbers_d) << std::endl;
    }
}

namespace OverloadTest
{

    template <typename T>
    class myvector
    {
        typedef T value_type;
        typedef value_type& reference;
        typedef value_type* pointer;
        typedef const value_type const_pointer;
        typedef const value_type& const_reference;
        typedef value_type* iterator;
        typedef const value_type* const_iterator;
        // what is iterator traits

        struct MyType
        {
            int* a;
            int b;
            double c;
            // friend bool operator==(const MyType& lhs, const MyType& rhs);
        };

        bool operator==(const MyType& lhs, const MyType& rhs){ return true; }
    }
}

int main(int argc, char* argv[])
{
    VectorAllocator::Execute();

    return 0;
}
