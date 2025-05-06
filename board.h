#ifndef _BOARD_H
#define _BOARD_H
#include "tile.h"
#include "vertex.h"
#include "edge.h"
#include "geese.h"
#include <vector>
#include <string>

class Board
{
    std::vector<Tile> tileList;
    std::vector<Vertex> vertexList;
    std::vector<Edge> edgeList;
    Geese geese;

    void loadWithLayout(std::vector<std::pair<Resource, int>> layoutVector);
    std::string checkResidence(int) const;
    std::string checkRoad(int) const;
    std::string checkResourceName(int) const;
    std::string checkResourceNum(int) const;
    void observerListInit();
    void verEdgInit();
    void verEdgInitHelper(int, int, int = 1);

public:
    Board();
    ~Board() = default;

    void updateResOnTile(int tileNum); // checks if the matching tile has geese on it. Send presents if not, otherwise does nothing.
    void display() const;
    Geese &getGeese();
    void setGeese(Geese geese);
    
    void colourVertex(int vertexNum, Builder *builder, bool start);
    void upgradeVertex(int vertexNum, Builder *builder);
    void colourEdge(int edgeNum, Builder *builder);

    void save(const std::string fileName);
    std::string loadFromFile(const std::string fileName);
    void load(std::string layoutStr);
    std::string loadRandomBoard();
    
    std::vector<Tile> getTileList() const;
    std::vector<Vertex> &getVertexList();
    std::vector<Edge> &getEdgeList();
    
};

#endif
