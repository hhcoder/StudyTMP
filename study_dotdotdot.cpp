#include <iostream>
#include <string>
#include <vector>

class Proc
{
public:
    Proc(){}
    ~Proc(){}
public:
    Proc& Parallel(const int v)
    {
        std::cout << "Parallel: " << v << ",";
        return *this;
    }

    Proc& Vectorize(const int v)
    {
        std::cout << "Vectorize: " << v << ",";
        return *this;
    }
};

void SerialDotDot()
{
    Proc proc;

    std::cout << "Calling Parallel only:" << std::endl;
    proc.Parallel(3);
    std::cout << std::endl;


    std::cout << "Calling Vectorize only:" << std::endl;
    proc.Vectorize(9);
    std::cout << std::endl;

    std::cout << "We can cascade the call like this: " << std::endl;

    proc.Parallel(18).Vectorize(33);

    std::cout << std::endl;

    std::cout << "Notice the writing sequence is the calling sequence: " << std::endl;

    proc.Vectorize(45).Parallel(67);

    std::cout << std::endl;
}


template <typename T>
class Pixel 
{
public:
    Pixel(T* in_v)
        : v(in_v)
    {}

    Pixel& operator=(const T& in_v) { *v = in_v; return *this; }

    friend std::ostream& operator<<(std::ostream& stream, const Pixel& p)
    {
        stream << *(p.v);
        return stream;
    }

private:
    T* v;
};

template <typename T>
class Buf2D 
{
public:
    Buf2D(const int in_width, const int in_height)
        : width(in_width), height(in_height)
    {
        buf = (T*)malloc(sizeof(T)*width*height);
    }

    ~Buf2D()
    {
        free(buf);
    }

    int32_t Width() const { return width; }
    int32_t Height() const { return height; }

    Pixel<T> operator()(const int32_t x, const int32_t y) { return Pixel<T>(buf + y*width + x); }

private:
    T* buf;
    int width;
    int height;
};

void AssignPixel()
{
    const int w = 10;
    const int h = 10;
    Buf2D<int> buf(w, h);

    for (int j=0; j<h; j++)
        for (int i=0; i<w; i++)
            buf(i, j) = i+j;

    for (int j=0; j<h; j++)
    {
        for (int i=0; i<w; i++)
        {
            std::cout << buf(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

int main(const int argc, const char* argv[])
{
    SerialDotDot();

    AssignPixel();

    return 0;
}
