#include <iostream>
#include <vector>
#include <memory>

namespace TemplateStaticInit
{
    template <typename T>
    struct Controller
    {
        using It = std::unique_ptr<T>;
        // using It = T*;
        using Ct = std::vector<It>;

        template <typename ...Args>
        Controller(Args&&... args)
        {
            container.emplace_back(new T(std::forward<Args>(args)...));
        }
        static Ct container;
    };

    template <typename T> 
        typename Controller<T>::Ct Controller<T>::container;

    void Exe()
    {
        Controller<double> c;
    }

};


int main(int argc, char* argv[])
{
    array(a);

    return 0;
}
