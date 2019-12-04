#include <string>
#include <fstream>
#include <vector>
#include <iostream>

namespace StudyBuffer
{

template <typename T>
class placement_memory_allocator
{
    void* pre_allocated_memory;
    public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    template<typename _Tp1>
        struct rebind
        {
            typedef placement_memory_allocator<_Tp1> other;
        };

    pointer allocate(size_type n, const void *hint=0)
    {
        char* p = new(pre_allocated_memory) char [n * sizeof(T)];
        return static_cast<T*>(p);
    }

    void deallocate(pointer p, size_type n)
    {
        std::cout << "deallocate does nothing" << std::endl;
    }

    placement_memory_allocator(T* p = 0) 
        : pre_allocated_memory(p) 
    { std::cout << "Hello allocator!" << std::endl; }
    placement_memory_allocator(const placement_memory_allocator &a) 
    {
        pre_allocated_memory = a.pre_allocated_memory;
    }
    ~placement_memory_allocator() throw() { }
};

template <typename T, typename AllocatorType = std::allocator<T>>
class Buf2D : public std::vector<T, AllocatorType> // default allocator
{
public:
    using size_type = typename std::vector<T, AllocatorType>::size_type;

public:
	Buf2D(const int in_width, const int in_height, const int in_stride, T* in_ext_buf)
		: width(in_width), 
          height(in_height), 
          stride(in_stride),
          allocator(in_ext_buf),
          std::vector<T, AllocatorType>(allocator)
	{}

    Buf2D(const int in_width, const int in_height)
		: width(in_width), 
          height(in_height), 
          stride(width),
          std::vector<T, AllocatorType>(PixelCount())
    {}

	~Buf2D() {}

	int Write(const char* out_fname)
	{
		// FILE* fp = fopen(out_fname, "wb");
		// if (NULL == fp)
		// 	return GeneralError::Fail;
		// fwrite(buf, sizeof(T), PixelCount(), fp);
		// fclose(fp);
        return 1;
	}

public:
	inline int Width() const { return width; }
	inline int Height() const { return height; }
	inline int Stride() const { return stride; }
    inline size_type PixelCount() const { return stride*height; }
    inline decltype(auto) Begin() { return buf; }

protected:
	int width;
	int height;
	int stride;
	T* buf;
    AllocatorType allocator;
};

void Exe()
{
    Buf2D<uint16_t> internal_buf(640, 480);
    double* ptr = new double[640*480];
    // Buf2D<double> external_buf(640, 480, 640, ptr);

    placement_memory_allocator<int> pl(nullptr);
    std::vector<int, placement_memory_allocator<int>> my_vec(pl);
}

}

namespace StudyPlacementNew
{
    void Exe()
    {
        int a[] = {1, 3, 5, 7, 9};

        {
            int* pa = new (a) int [5];

            for (int i=0; i<5; i++)
                std::cout << pa[i] << std::endl;

            std::cout << "a: " << a << std::endl;
            std::cout << "pa: " << pa << std::endl;

            // delete[] pa;
            ::operator delete[](pa, 5);
        }
    }
}

namespace StudyScope
{
    // template <typename T>
    // struct MyVector : std::vector<T>
    // {
    // };

    template <typename T>
    struct BaseContainer
    {
        BaseContainer(int len)
        {
        }

        typedef size_t size_type;
    };

    struct MyContainer : BaseContainer<int>
    {
        size_type length;
    };

    void Exe()
    {
    }
}

int main(int argc, char* argv[])
{
    StudyPlacementNew::Exe();

    StudyScope::Exe();
    
    return 0;
}
