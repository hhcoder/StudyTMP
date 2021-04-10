#include <utility>
#include <vector>
#include <string>

namespace Case1PerfectForwardingInitializationList
{
    template <typename ItemType>
    struct my_list
    {
        template <typename... Arg>
        my_list(Arg&&... params)
            : ct(std::forward<Arg>(params) ... )
        {
        }

        std::vector<ItemType> ct;
    };

    void Exe()
    {
        int arr[] = {1, 2, 3, 4};
        my_list<int> int_list(&arr[0], &arr[4]);

        // However, following code wouldn't work
        // my_list<int> int_list_2{1,2,3,4,5};
    }

    template <typename ItemType>
    struct my_list2
    {
        template <typename... Arg>
        my_list2(Arg&&... params)
            : ct(std::forward<Arg>(params) ... )
        {
        }

        my_list2(std::initializer_list<ItemType> ilist)
            : ct(ilist){}
        std::vector<ItemType> ct;
    };

    void Exe2()
    {
        // Now it works
        // so perfect forwarding cannot dedude initialization list parameter
        // don't know why...
        my_list2<int> int_list{1,2,3,4,5};
    }
};

int main(int argc, char* argv[])
{
    Case1PerfectForwardingInitializationList::Exe();

    return 0;
}