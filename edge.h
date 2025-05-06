#ifndef _EDGE_H_
#define _EDGE_H_
#include <vector>
#include "enum.h"

class Vertex;

class Edge
{
    int num;
    std::vector<Vertex *> twoVertices;
    Colour colour;

public:
    Edge(int num);
    std::vector<Vertex *> getVertices() const;
    void setColour(Colour colour);
    Colour getColour() const;
    void setVertex(Vertex *);
    ~Edge() = default;
};

#endif
