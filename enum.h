#ifndef _ENUM_H
#define _ENUM_H
#include <string>

extern unsigned seed;

enum class Resource
{
    BRICK,
    ENERGY,
    GLASS,
    HEAT,
    WIFI,
    PARK
};
enum class Residence
{
    NoRes,
    B,
    H,
    T
};
enum class Colour
{
    NoColour,
    Blue,
    Red,
    Orange,
    Yellow
};
enum class DiceType
{
    Fair,
    Loaded
};

// Helper Functions:
Resource conv2Resource(std::string);

Colour conv2Colour(std::string);

std::string colour2Str(Colour);

std::string resource2Str(Resource);

std::string residence2Str(Residence);

#endif
