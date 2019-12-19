#include <memory>
#include <iostream>
#include <vector>

// You couldn't use auto_ptr in most of the containers

// So, what is this "unique" meaning?
// When you create a unique_ptr object, you are declaring that you are going to have exactly one copy of this pointer.
// You can't indavertently make copies of the pointer (!?)
// So you won't make a duplication of the pointer - the compiler prohibits it

namespace Introduction
{
    struct Item
    {
        Item(const Item& it)
            : roll(it.roll), yaw(it.yaw), pitch(it.pitch)
        {
            DumpValue();
        }

        Item(const double ir, const double iy, const double ip)
            : roll(ir), yaw(iy), pitch(ip)
        {
            DumpValue();
        }

        ~Item()
        {
            std::cout << "Item destructor" << std::endl;
        }

        void DumpValue() { std::cout << "Item Constructor: " << "(" << roll << "," << yaw << "," << pitch << ")" << std::endl; }

        double roll;
        double yaw;
        double pitch;
    };

    template <typename T>
    struct ItemCtrl
    {
        ItemCtrl()
        {
        }

        template <typename ... Args>
        void Add(Args&&... args)
        {
            // So, the conclusion is that the push_back is not as simple as I thought
            // anyway, perfect forwarding is, well, perfect 
            c.push_back(std::forward<Args>(args) ... );
        }

        std::vector<T> c;
    };

    // That's a strange way to write a function :D
    template <typename T>
        decltype(auto) ReturnSelf(T t) { return t; }

    void MakeDouble(double* d){ *d = 2*(*d); }

    template <typename T>
        decltype(auto) DoSomethingToUniquePtr(T t)
        {
            *t = *t * 100;
            return t;
        }

    void OperateOnItem(std::unique_ptr<Item>& p)
    {
        p->roll *= 2;
    }

    void Exe()
    {
        std::unique_ptr<Item> p(new Item(2.2, 3.3, 4.4));

        // ItemCtrl<std::unique_ptr<Item>> it;
        // it.Add(std::move(p));

        std::vector<std::unique_ptr<Item>> iv;
        iv.push_back(std::move(p));

        if(p==nullptr)
            std::cout << "p become null" << std::endl;

        std::unique_ptr<double> uni_double(new double(3.1415));

        // So, when moving between lvalues, we need "move"
        uni_double = ReturnSelf(std::move(uni_double));

        // when moving between rvalues, no "move" is necessary
        std::unique_ptr<double> p2(ReturnSelf(new double(3.44)));

        std::cout << *p2 << std::endl;

        // Okay, get is not a very good way to do it
        MakeDouble(p2.get());

        // Release is much better
        MakeDouble(p2.release());

        if(p2!=nullptr)
            std::cout << *p2 << std::endl;
        else
            std::cout << "You've lost the ownership to p2" << std::endl;

        std::unique_ptr<float> p3(new float(1.975));

        p3 = DoSomethingToUniquePtr(std::move(p3));

        std::cout << *p3 << std::endl;

        std::unique_ptr<Item> p_item(new Item(2.2, 4.4, 8.8));
        p_item->DumpValue();
        OperateOnItem(p_item);
        p_item->DumpValue();
        // So unique_ptr is basically a ptr manager without copy and assignment operator
        // Since copying pointer will lead to resource management problem, the new design
        //  forces user to have only one ownership each time accessing the pointer.
    }
};

int main(int argc, char* argv[])
{
    Introduction::Exe();

    return 0;
}
