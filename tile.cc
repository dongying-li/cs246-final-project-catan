#include "tile.h"

Tile::Tile(int num, Resource resource, int value) : num{num}, resource{resource}, value{value} {}

int Tile::getNum() const
{
    return num;
}

Resource Tile::getResourceType() const
{
    return resource;
}

int Tile::getResourceNum() const
{
    return value;
}

void Tile::setState(Resource resource, int value)
{
    this->resource = resource;
    this->value = value;
}
