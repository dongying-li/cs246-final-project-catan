#ifndef _TILE_H_
#define _TILE_H_
#include "subject.h"
#include "enum.h"

class Tile : public Subject
{
    int num;
    Resource resource;
    int value;

public:
    Tile(int num, Resource resource, int value);
    int getNum() const;
    Resource getResourceType() const override;
    int getResourceNum() const;
    void setState(Resource resource, int value);
};

#endif
