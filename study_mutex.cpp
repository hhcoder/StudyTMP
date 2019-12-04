#include <mutex>
#include <iostream>
#include <thread>
#include <stdexcept> //?
#include <utility>

void print_even(int x)
{
    if(x%2==0) 
    {
        std::cout << x << "is even" << std::endl;
    } else {
        throw (std::logic_error("not even"));
    }
}

// Notice that following code will give compile error since
//          mutex(const mutex&) = delete;
// 
// template <typename MutexType>
// struct MyMutex
// {
//     MyMutex(const MutexType& m): mtx(m) { mtx.lock(); }
//     ~MyMutex(){ mtx.unlock(); }
//     MutexType mtx;
// };
//
// Then, why we can say
// std::lock_guard<std::mutex> lck(mtx); 
// How can standard library get rid of this?

void print_thread_id(int id, int* ret_id, int* ret2, int* ret3)
{
    try{
        // std::mutex mtx;
        // std::timed_mutex mtx;
        // std::recursive_mutex mtx;
        std::recursive_timed_mutex mtx;
        // std::lock_guard<decltype(mtx)> lck(mtx);
        std::unique_lock<decltype(mtx)> lck(mtx);
        // MyMutex<std::mutex> lck(mtx);
        // MyMutex m;
        print_even(id);
        *ret_id=id+2;
    }
    catch (std::logic_error&)
    {
        std::cout << "[exception caught]" << std::endl;
    }
}

template <typename T>
    struct MyType
    {
        MyType(T& it) : t(it) { std::cout  << "struct T: " << t << std::endl; }
        T t;
    };

template <typename T>
    void MyFunc(T& t) { std::cout << "func T: " << t << std::endl; }

static int v = 2;
void try_race_condition(bool direction)
{
    while(1)
    {
        if (direction)
            v++;
        else
            v--;
    }
};

int main(int argc, char* argv[])
{
    std::thread threads[10];
    int ret_id;

    for (int i=0; i<10; i++)
        threads[i] = std::thread(print_thread_id, i+1, &ret_id, &ret_id, &ret_id);

    // This is interesting! it's like exit() in c?
    // std::terminate();

    for (auto& th: threads) th.join();

    std::cout << "ret_id: " << ret_id << std::endl;

    // How is this possible?
    // threads[i] = std::thread(print_thread_id, i+1, &ret_id, &ret_id, &ret_id);
    // Okay, here it is:
    // template <class Fn, class... Args>
    //  explicit thread (Fn&& fn, Args&&... args);
    // Remember template type deduction?
    // No, I cannot do it? why?
    auto a = 3;
    auto b = 4.88;
    MyFunc(a);
    // Notice the following code won't work in C++11/C++14
    // MyType m(a);
    // enable -std=c++17 in command
    // MyType<decltype(a)> m(a);
    // std::pair p(2.4, 3.3);

    std::thread th1(try_race_condition, true);
    std::thread th2(try_race_condition, false);
    std::thread th3(try_race_condition, false);
    std::thread th4(try_race_condition, true);
    while(v!=0)
    {
        std::cout << v << std::endl;
    }
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    return 0;
}
