#include <iostream>

namespace EveryThingInBaseClass
{

    enum ColorFormat
    {
        RGB_INTERLEAVE,
        YUV_NV21,
        YUV_444_Planar
    };

    class Buffer
    {
    public:
        typedef uint8_t T;

        struct Pixel
        {
            Pixel(T ia, T ib, T ic) : a(ia), b(ib), c(ic) {}
            T a; T b; T c;
        };

        struct Position
        {
            Position(int ixo, float ixs, int iyo, float iys)
                : xoffset(ixo), xscale(ixs), yoffset(iyo), yscale(iys)
            {}

            int Offset(int x, int y)
            {
                return floor(x * xscale) * xoffset + floor(y * yscale) * yoffset;
            }

            int xoffset;
            float xscale;
            int yoffset;
            float yscale;
        };

    public:
        Buffer(int iw, int ih, ColorFormat iformat)
            : w(iw), h(ih), format(iformat)
        {
            switch(iformat)
            {
                case RGB_INTERLEAVE:
                    buf = new T[w*h*3];

                    ptr[0] = buf;
                    pos[0].xoffset = 3;
                    pos[0].xscale = 1.0;
                    pos[0].yoffset = w*2;
                    pos[0].yscale = 1.0;

                    ptr[1] = buf+1;
                    pos[1].xoffset = 3;
                    pos[1].xscale = 1.0;
                    pos[1].yoffset = w*2;
                    pos[1].yscale = 1.0;

                    ptr[2] = buf+2;
                    pos[2].xoffset = 3;
                    pos[2].xscale = 1.0;
                    pos[2].yoffset = w*2;
                    pos[2].yscale = 1.0;
                break;

                case YUV_NV21:
                    buf = new T[w*h*3/2];
                break;
            }
        }

        Pixel GetPixel(int x, int y)
        {
            T a = *(ptr[0] + pos[0].Offset(x, y));
            T b = *(ptr[1] + pos[1].Offset(x, y));
            T c = *(ptr[2] + pos[2].Offset(x, y));

            return Pixel(a, b, c);
        }

        void SetPixel(int x, int y, const Pixel& p)
        {
            *(ptr[0] + pos[0].Offset(x, y)) = p.a;
            *(ptr[1] + pos[1].Offset(x, y)) = p.b;
            *(ptr[2] + pos[2].Offset(x, y)) = p.c;
        }

        bool Write(const std::string& ofname)
        {
            return true;
        }

        bool Read(const std::string& ifname)
        {
            return true;
        }

        ~Buffer()
        {
            delete[] buf;
        }
    private:
        int w;
        int h;
        ColorFormat format;
        T* buf;
        T* ptr[3];
        int offset[3];
        int stride[3];
    };

    bool Convert(Buffer& A, Buffer& B, Buffer& C)
    {
        return true;
    }
}

namespace Advanced
{
template
    class Buffer<typename Pixel, typename Container=std::vector<Pixel>>
    {
    }
}

int main(int argc, char* argv[])
{
    return 0;
}
