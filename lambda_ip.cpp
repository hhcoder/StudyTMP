#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <functional>
#include <queue>
#include <iomanip>
#include <memory>

namespace Playground
{
    template <typename ContainerType>
    struct Buf2D 
    {
        Buf2D(int w, int h)
            : width(w), height(h), c(width*height) {}

        using value_type = typename ContainerType::value_type;
        using reference = typename ContainerType::reference;
        using size_type = typename ContainerType::size_type;

        // Unbelievable confusing code :D
        template <typename T> decltype(auto) Clamp(const T& t_min, const T& t, const T& t_max)
        {
            return [](const T& t_min, const T& t, const T& t_max)
            {
                return
                    [](const T& t, const T& t_max){ return t>t_max ? t_max : t; } (
                    [](const T& t_min, const T& t){ return t<t_min ? t_min : t; }(t_min, t), 
                 t_max);
            }(t_min, t, t_max);
        // Normal version implementation
        //     if(t<t_min) return t_min;
        //     if(t>t_max) return t_max;
        //     return t;
        }

        decltype(auto) operator()(int x, int y) 
        {
                return at(x, y);
        }

        decltype(auto) operator[](size_type s) { return c[s]; }

        decltype(auto) at(int x, int y) 
        {
                auto idx_y = Clamp(0, y, height-1);
                auto idx_x = Clamp(0, x, width-1);
                return c.at(idx_y*width + idx_x);
        }

        decltype(auto) size() const { return c.size(); }

        void Print()
        {
            for (int y=0; y<height; y++)
            {
                for (int x=0; x<width; x++)
                {
                    std::cout << std::setw(4) << static_cast<int>(operator()(x, y)) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        int width;
        int height;
        ContainerType c;
    };

    template <typename ContainerType>
    decltype(auto) WriteToFile(const std::string& out_path, ContainerType& c)
    {
        std::ofstream fout(out_path, std::ios::out | std::ios::binary);
        return fout.write(
                reinterpret_cast<const char*>(&c[0]),
                c.size()*sizeof(typename ContainerType::value_type));
    }

    template <typename BufType>
    struct FuncImpl
    {
        using PixelType = typename BufType::value_type;
        using PixelRef = typename BufType::reference;
        using LambdaFxn = std::function<PixelType(BufType&, int, int)>;

        FuncImpl(const std::string& in_name, BufType& in_buf, LambdaFxn in_proc_each)
            : name(in_name),
              input_buf(&in_buf),
              output_buf(nullptr),
              proc_each(in_proc_each)
        {
            // Register(this);
        }

        FuncImpl(BufType& buf){}

        ~FuncImpl()
        {
            if(output_buf!=nullptr)
                delete output_buf;
        }
    //
    //     FuncImpl(FuncImpl& in_prev_FuncImpl, std::FuncImpltion<void(int, int)> in_proc_each)
    //         : prev_FuncImpl(in_prev_FuncImpl), proc_each(in_proc_each)
    //     {
    //         RegisterFuncImplList();
    //     }
    //
    //     FuncImpl& SetOutput(int w, int h)
    //     {
    //         Buf2D<PixelType> out_buf(w, h);
    //
    //         RegisterXRatio(out_buf);
    //         RegisterYRatio(out_buf);
    //
    //         return *this;
    //     }
    //
        FuncImpl& Run()
        {
            auto ConvertXFromOutToIn=[](int v){ return v; };
            auto ConvertYFromOutToIn=[](int v){ return v; };

            output_buf = new BufType(input_buf->width, input_buf->height);

            for (int j=0; j<output_buf->height; j++)
            {
                for (int i=0; i<output_buf->width; i++)
                {
                    output_buf->at(i, j) = proc_each(
                            *input_buf,
                            ConvertXFromOutToIn(i), 
                            ConvertYFromOutToIn(j));
                }
            }
            return *this;
        }

        FuncImpl& Print()
        {
            output_buf->Print();
            return *this;
        }

        FuncImpl& WriteToFile(const std::string& fname)
        {
            WriteToFile(fname, *output_buf);
            return *this;
        }

        std::string name;
        BufType* input_buf; 
        BufType* output_buf;
        LambdaFxn proc_each;

        // static std::vector<unique_ptr<FuncImpl>> FuncImpl_queue;
        // static void Register(unique_ptr<FuncImpl>& in) { FuncImpl_queue.push_back(in); }
    };

    template <typename BufferType>
    struct Func
    {
        template <typename ...Args>
        Func(Args&&... args)
        {
            // PtrType p(new ItemType(std::forward<Args>(args)...));
            c.emplace_back( FuncImplType(std::forward<Args>(args)...) );
        }

        static decltype(auto) Run()
        {
            for (auto i=std::begin(c); i!=std::end(c); i++)
                i->Run();

            return *std::rbegin(c);
        }

        using FuncImplType = FuncImpl<BufferType>;
        using ContainerType = std::vector<FuncImplType>;
        // using ContainerType = std::vector<FuncImpl<BufferType>>;

        static ContainerType c;
    };

    template <typename BufferType>
       typename Func<BufferType>::ContainerType Func<BufferType>::c;
}

int main(int argc, char* argv[])
{
    using namespace Playground;

    const int im_width = 10;
    const int im_height = 10;
    using GrayImg = Buf2D<std::vector<uint8_t>>;
    GrayImg input_buf(im_width, im_height);

    // for (int j=0; j<im_height; j++)
    //     for (int i=0; i<im_width; i++)
    //         input_buf(i, j) = i+j;

    // WriteToFile("./g.y", input_buf);

    // input_buf.Print();

    // FuncImpl<GrayImg> FuncImpl_img(input_buf);
    //

    const int grid_width = 3;
    const int grid_height = 2;

    Func<GrayImg> grid_src(
            std::string("Grid Source"),
            input_buf, 
            [grid_width, grid_height](GrayImg& in_buf, int x, int y){
                if( (x/grid_width)%2+(y/grid_height)%2 == 1 )
                    return 255;
                else
                    return 0;
            });

    // grid_src.Run()
    //         .Print();

    Func<GrayImg>::Run().Print();

    //
    // Func hori_blur(grid_src, [](int x, int y){
    //     return 
    //     ( 1*in_buf(x-2, y) + 
    //       4*in_buf(x-1, y) + 
    //       6*in_buf(x,   y) + 
    //       4*in_buf(x+1, y) +
    //       1*in_buf(x+2, y) ) >> 4;
    // });
    //
    // Func vert_blur(hori_blur, [](int x, int y){
    //     return 
    //     ( 1*in_buf(x, y-2) + 
    //       4*in_buf(x, y-1) + 
    //       6*in_buf(x, y) + 
    //       4*in_buf(x, y+1) + 
    //       1*in_buf(x, y+2) ) >> 4;
    // });
    //
    // vert_blur
    //     .SetOutput(100, 100)
    //     .Run()
    //     .WriteToFile("./out.y");

    return 0;
}

