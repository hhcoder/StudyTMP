#include <cstddef>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>

namespace UnderstandingBasics 
{
    template <class T, class... Args>
    struct allocator
    {
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef size_t          size_type;
        // typedef std::true_ty    propagate_on_cotainer_move_assignment; //typo?
        // template<typename U> struct rebind { typedef allocator<U> other; };

        pointer allocate(size_type n) { return static_cast<pointer>(malloc(sizeof(T)*n)); }
        // in fact, I don't understand why do we need a size_type n as deallocate input as well?
        void deallocate(pointer p, size_type n) { free(p); }

        void construct(T* b, Args&& ... args) { b = new T(std::forward<args>(args)...); }
    };

    template <class VecType>
        void Foo(VecType& vec)
        {
            // You can do this, but, in fact, it will declare two functions
        }

    void Exe()
    {
        // 1. Some form of dynamic memory allocation is required due to capability of changing size on-the-fly
        std::allocator<double> my_allocator;
        std::allocator<double>::size_type count = 36;

        double* my_array = my_allocator.allocate(count);

        // for (int i=0; i<5; i++)
        //     my_array[i] = i*i;

        const double pi = std::acos(-1);

        for (int i=0; i<count; i++)
            my_allocator.construct(&my_array[i], std::sin(i*pi/36));

        std::cout << "=========array===============" << std::endl;
        for (int i=0; i<count; i++)
            std::cout << "index: " << i << " value: " << my_array[i] << std::endl;

        my_allocator.deallocate(my_array, count);

        // okay, now, I am really confused. then how could I use construct on vector?
        // yes, the basic question is: why does a container need allocator?o

        std::vector<double>::size_type len = 36;
        std::vector<double> my_vector(len);

        for (int i=0; i<count; i++)
            my_vector[i] = std::sin(i*pi/36.00);

        std::cout << "=========vector===============" << std::endl;
        for (auto i : my_vector)
            std::cout << i << std::endl;

        // 1
        std::vector<int, allocator<int>> my_vec(3);
        // 2
        std::vector<int, std::allocator<int>> std_vec(3);

        Foo(my_vec);

        Foo(std_vec);
    }
}
// okay, it looks like not so easy to implement a self-written version allocator since a lot of
//   things need to be considered.

namespace StudyPolymorphismAllocator
{
    // This allows you to have container which use specific, customised allocation, but which
    // are still of a common type. -> how???

    // the customization of allocator behavior is done by giving the allocator a 
    // std::memory_resource
}

#include <memory>
#include <stdio.h>
namespace StudyPlacementAllocator
{
    template <typename T>
        class placement_allocator
        {
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;
            typedef std::true_type 
                propagate_on_container_move_assignment; //???
            template <typename U> struct rebind { typedef placement_allocator<U> other; }; //???
            typedef std::true_type is_always_equal; //???
        };
}

int main(int argc, char* argv[])
{
    UnderstandingBasics::Exe();

    return 0;
}
