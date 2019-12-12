#include <iostream>

int main(int argc, char* argv[])
{
    using GrayImg = Buf2D<uint8_t>;

    const int im_width = 100;
    const int im_height = 100;
    GrayImg input_buf(im_width, im_height);

    for (int j=0; j<100; j++)
        for (int i=0; i<100; i++)
            if((i/10)%2||(j/10)%2)
                input_buf(i, j) = 255;
            else
                input_buf(i, j) = 0;

    Func hori_blur(input_buf, [](int x, int y){
        return 
        ( 1*input_buf(x-2, y) + 
          4*input_buf(x-1, y) + 
          6*input_buf(x,   y) + 
          4*input_buf(x+1, y) +
          1*input_buf(x+2, y) ) >> 4;
    });

    Func vert_blur(hori_blur, [](int x, int y){
        return 
        ( 1*input_buf(x, y-2) + 
          4*input_buf(x, y-1) + 
          6*input_buf(x, y) + 
          4*input_buf(x, y+1) + 1*input_buf(x, y+2) ) >> 4;
    });

    vert_blur
        .SetOutput(100, 100)
        .Run()
        .WriteToFile("./out.y");

    return 0;
}

template <typename PixelType>
struct Func
{
    Func(Buf2D<PixelType>& in, std::function<void(int, int)> proc_each)
    {
    }

    Func(Func& prev_func, std::function<void(int, int)> proc_each)
    {
        Buf2D<PixelType> out_buf = DeduceOutputBuf(prev_func.GetInputBuf());
        RegisterXRatio(out_buf);
        RegisterYRatio(out_buf);
        RegisterProcEach(proc_each);

    }

    Func& Run()
    {
        for (int j=0; j<out_buf->height; j++)
        {
            for (int i=0; i<out_buf->width; i++)
            {
                out_buf(i, j) = proc_each(
                       OutputXtoInputX(i), 
                       OutputYToInputY(j));
            }
        }
        return *this;
    }

    Func& SetOutput(int w, int h)
    {
        return *this;
    }

    Func& WriteToFile(const std::string& fname)
    {
        return *this;
    }

    Buf2D<PixelType> out_buf;
};
