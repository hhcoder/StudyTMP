#include <iostream>
#include <mutex>

template <typename FuncType,
          typename MuxType,
          typename PtrType>
decltype(auto) 
    LockAndCall(FuncType func, MuxType& mux, PtrType ptr)
{
    std::lock_guard<MuxType> lck(mux);
    return func(ptr);
}

#define MY_NULL (static_cast<void*>(0))

void f1(int* dump)
{
    if(nullptr!=dump) std::cout << *dump << std::endl;
    else std::cout << "Empty pointer!\n";
}

int main(int argc, char* argv[])
{
    int a = 3;
    int* pa = &a;
    std::mutex mx;
    LockAndCall(f1, mx, pa);

    // Following code won't even compile
    // LockAndCall(f1, MY_NULL);

    LockAndCall(f1, mx, nullptr);

    return 0;
}
