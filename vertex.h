#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <vector>
#include "observer.h"
#include "enum.h"

class Builder;
class Edge;
class Tile;
class Vertex : public Observer
{
    int num;
    Builder *builder;
    Colour colour;
    Residence res;
    std::vector<Edge *> adjacentRoad;

public:
    Vertex(int num);
    void setBuilder(Builder *b);
    std::vector<Edge *> getAdjacentRoad();
    Residence getResidence() const;
    void setColour(Colour colour);
    void setEdge(Edge *);
    void setRes(Residence res);
    void updateRes();
    Colour getColour() const override;
    void notify(Subject &whoFrom) override;
    ~Vertex() = default;
};

#endif
