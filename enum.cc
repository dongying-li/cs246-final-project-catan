#include <string>
#include "enum.h"
using namespace std;

unsigned seed = 0;

Resource conv2Resource(string s)
{
    if (s == "BRICK")
    {
        return Resource::BRICK;
    }
    else if (s == "GLASS")
    {
        return Resource::GLASS;
    }
    else if (s == "HEAT")
    {
        return Resource::HEAT;
    }
    else if (s == "WIFI")
    {
        return Resource::WIFI;
    }
    else
    {
        return Resource::ENERGY;
    }
}

Colour conv2Colour(string colour)
{
    if (colour == "BLUE" || colour == "blue" || colour == "Blue")
    {
        return Colour::Blue;
    }
    else if (colour == "RED" || colour == "red" || colour == "Red")
    {
        return Colour::Red;
    }
    else if (colour == "ORANGE" || colour == "orange" || colour == "Orange")
    {
        return Colour::Orange;
    }
    else
    {
        return Colour::Yellow;
    }
}

string colour2Str(Colour colour)
{
    if (colour == Colour::NoColour)
    {
        return "NoColour";
    }
    else if (colour == Colour::Blue)
    {
        return "Blue";
    }
    else if (colour == Colour::Red)
    {
        return "Red";
    }
    else if (colour == Colour::Orange)
    {
        return "Orange";
    }
    else
    {
        return "Yellow";
    }
}

string resource2Str(enum Resource resource)
{
    if (resource == Resource::BRICK)
    {
        return "BRICK";
    }
    else if (resource == Resource::GLASS)
    {
        return "GLASS";
    }
    else if (resource == Resource::HEAT)
    {
        return "HEAT";
    }
    else if (resource == Resource::WIFI)
    {
        return "WIFI";
    }
    else
    {
        return "ENERGY";
    }
}

string residence2Str(Residence residence)
{
    switch (residence)
    {
    case Residence::B:
        return "B";
    case Residence::H:
        return "H";
    case Residence::T:
        return "T";
    default:
        return "";
    }
}
