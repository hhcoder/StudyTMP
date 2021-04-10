#include <iostream>
#include <vector>
#include <string>
#include <map>

int main(int argc, char* argv[])
{
    struct my_struct
    {
        int v;
        std::string s;
        void print(){ std::cout << "[" << v << "," << s << "]" << std::endl;}
    };

    std::vector<my_struct> list_struct = {{1, "hello"}, {2, "happy"}, {3, "health"}};

    auto print_func = [&](const std::vector<my_struct>& t){ for (auto i:t) i.print(); };

    print_func(list_struct);

    std::cout << "=============" << std::endl;

    auto find_str = [&](const std::string& s) {
        for (auto & i : list_struct)
            if (i.s == s)
                return &(i);
        return nullptr;
    };

    auto i = find_str(list_struct);
    i.v = 909;

    print_func(list_struct);

    return 0;
}