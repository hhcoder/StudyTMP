#include <iostream>
#include <stdexcept>

class SensorConnect
{
public:
    SensorConnect(int in_id)
        : buf(new void*[640*480]), id(in_id)
    {
        if( false == Open(id) )
            throw std::logic_error("Error in opening id: ");
    }

    void StartStreaming()
    {
        for (int i=0; i<10; i++)
            std::cout << i << ",";
        std::cout << std::endl;
    }

    ~SensorConnect()
    {
        free(buf);
    }

private:
    bool Open(int in_id)
    {
        if(in_id%2)
            return true;
        else
            return false;
    }

private:
    int id;
    void* buf;
};

int main(int argc, char* argv[])
{
    try
    {
        SensorConnect s(2);

        s.StartStreaming();

    }
    catch( std::logic_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
