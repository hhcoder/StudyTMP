namespace DumpTuple
{
    std::tuple<int, int, int, std::string> global_tuple{ 1, 3, 5, "hello" };

    template <std::size_t N>
    void dump()
    {
        std::cout << std::get<N - 1>(global_tuple);
        dump<N - 1>();
    }
    template <>
    void dump<0>()
    {
        std::cout << std::get<0>(global_tuple);
    }

    template <typename T>
    void dump_impl(T& t)
    {
        dump<std::tuple_size<T>::value>();
    }

    void Exe()
    {
        dump_impl(global_tuple);
    }
}