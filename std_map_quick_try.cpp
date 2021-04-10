    enum MyEnum
    {
        MyEnum1,
        MyEnum3,
        MyEnum9
    };

    std::map<int, std::string> number_to_str = 
    {
        {MyEnum::MyEnum1, "one"}, 
        {MyEnum::MyEnum3, "three"}, 
        {MyEnum::MyEnum3, "nine"}
    };

    std::cout << number_to_str[4] << std::endl;

    std::map<std::string, std::string> str_to_str = 
    {
        {"1", "one"}, 
        {"3", "three"}, 
        {"9", "nine"}
    };

    std::cout << str_to_str["9"] << std::endl;

