#include <vector>
#include "tile.h"
#include "vertex.h"
#include "builder.h"
#include <iostream>
using namespace std;

Vertex::Vertex(int num) : num{num}, colour{Colour::NoColour}, res{Residence::NoRes} {}

void Vertex::updateRes()
{
    if (res == Residence::NoRes)
    {
        res = Residence::B;
    }
    else if (res == Residence::B)
    {
        res = Residence::H;
    }
    else if (res == Residence::H)
    {
        res = Residence::T;
    }
    else
    {
    }
}

void Vertex::setColour(Colour colour)
{
    this->colour = colour;
}

void Vertex::setBuilder(Builder *b)
{
    this->builder = b;
}

vector<Edge *> Vertex::getAdjacentRoad()
{
    return adjacentRoad;
}

Colour Vertex::getColour() const
{
    return colour;
}

Residence Vertex::getResidence() const
{
    return res;
}

void Vertex::notify(Subject &whoFrom)
{
    if (colour == Colour::NoColour) // builder would be nullptr
    {
        return;
    }
    Resource resource = whoFrom.getResourceType();
    builder->updateInventory(resource, res);
}

void Vertex::setEdge(Edge *e) {
    adjacentRoad.emplace_back(e);
}

void Vertex::setRes(Residence res) {
    this->res = res;
}
