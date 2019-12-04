#include <iostream>
#include <iomanip>
#include <type_traits>

namespace UnscopedEnum
{

namespace Scope
{
enum ImageType
{
    YUV420_8BIT_PLANAR = 0x00A0,
    YUV420_8BIT_NV21,
    YUV420_10BIT_NV21,
    RGB_8BIT_INT,
    RGB_8BIT_PLANAR
};

enum ColorType
{
    RRR,
    GGG,
    BBB
};
}

void ImageProcess(Scope::ImageType itype)
{
    std::cout << "========" << "Unscoped Enum" << "=========" << std::endl;
    std::cout << "Processing type: 0x";
    std::cout << std::setw(8) << std::setfill('0') << std::hex << itype << std::endl;

    std::cout << "Original type of Scope::ImageType is: " << 
        typeid(Scope::ImageType).name() << std::endl;
    std::cout << "Underlying type of Scope::ImageType is: " << 
        typeid(std::underlying_type_t<Scope::ImageType>).name() << std::endl;
    std::cout << "Original type of Scope::ColorType is: " << 
        typeid(Scope::ColorType).name() << std::endl;
    std::cout << "Underlying type of Scope::ColorType is: " << 
        typeid(std::underlying_type_t<Scope::ColorType>).name() << std::endl;

    std::cout << "j means unsigned" << std::endl;
    std::cout << "typeid(unsigned int).name(): " <<  typeid(unsigned int).name() << std::endl;
    std::cout << "typeid(int).name(): " <<  typeid(int).name() << std::endl;
    std::cout << "typeid(unsigned).name(): " <<  typeid(unsigned).name() << std::endl;
    std::cout << "typeid(char).name(): " <<  typeid(char).name() << std::endl;
    std::cout << "typeid(unsigned char).name(): " <<  typeid(unsigned char).name() << std::endl;

    // Can I say unsigned float?
    // unsigned float p;
    // okay, you'll got compile error with above line
}

void Exe()
{
    ImageProcess(Scope::YUV420_8BIT_NV21);
}

};

namespace ScopedEnum
{

// okay, even I assign underlying type, it still won't work
enum class ImageType : std::int32_t
{
    YUV420_8BIT_PLANAR = 0x00A0,
    YUV420_8BIT_NV21,
    YUV420_10BIT_NV21,
    RGB_8BIT_INT,
    RGB_8BIT_PLANAR
};

void ImageProcess(ImageType itype)
{
    std::cout << "========" << "Scoped Enum" << "=========" << std::endl;

    std::cout << "ImageProcess" << std::endl;

    std::cout << "typeid(std::underlying_type<decltype(itype)>::type).name(): " <<
        typeid(std::underlying_type<decltype(itype)>::type).name() << std::endl;

    std::cout << "typeid(std::underlying_type_t<decltype(itype)>).name(): " <<
        typeid(std::underlying_type_t<decltype(itype)>).name() << std::endl;

    std::cout << "typeid(itype).name(): " <<
        typeid(itype).name() << std::endl;

    std::cout << "Processing type: 0x" <<
              std::setw(8) << std::setfill('0') << std::hex << 
              static_cast<std::underlying_type_t<decltype(itype)>>(itype) << std::endl;

    // So this is interesting, the scoped enum is much more type strong than unscoped enum
}

// Therefore we can write a template function to convert enum type to cout acceptable type

template <typename EnumType>
decltype(auto) ToCout(EnumType e)
{
    return static_cast<std::underlying_type_t<EnumType>>(e);
}

template <typename EnumType>
decltype(auto) GetUnderlyingName(EnumType e)
{
    return typeid(std::underlying_type_t<decltype(e)>).name();
}

void ImageProcess2(ImageType itype)
{
    std::cout << "========" << "Scoped Enum" << "=========" << std::endl;
    std::cout << "ImageProcess2" << std::endl;
    std::cout << "Processing type: 0x" <<
              std::setw(8) << std::setfill('0') << std::hex << 
              ToCout(itype) << std::endl;
}

enum class EnumFromInt : std::int32_t
{
};

enum class EnumFromUint : std::uint32_t
{
};

enum class EnumFromChar : std::uint8_t
{
};

void ImageProcess3()
{
    std::cout << "========" << "Scoped Enum" << "=========" << std::endl;
    std::cout << "ImageProcess3" << std::endl;
    EnumFromInt ei;

    std::cout << "GetUnderlyingName(ei): " << ToCout(ei) << std::endl;
    std::cout << "Interestingly, value of enum without and declaration is 0" << std::endl;
    std::cout << "GetUnderlyingName(ei): " << GetUnderlyingName(ei) << std::endl;
    EnumFromChar ec;
    std::cout << "GetUnderlyingName(ec): " << GetUnderlyingName(ec) << std::endl;
    EnumFromUint eu;
    std::cout << "GetUnderlyingName(eu): " << GetUnderlyingName(eu) << std::endl;
};


void Exe()
{
    ImageProcess(ImageType::YUV420_8BIT_NV21);
    ImageProcess2(ImageType::YUV420_8BIT_PLANAR);
    ImageProcess3();
}

}


int main(int argc, char* argv[])
{
    UnscopedEnum::Exe();

    ScopedEnum::Exe();

    return 0;
}
