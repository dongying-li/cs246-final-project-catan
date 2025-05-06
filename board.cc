#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "board.h"
#include "builder.h"
using namespace std;

// Helper Functions:

// checkB(b) checks if builder has enough resources to build a basement.
bool checkB(Builder *b)
{
    Inventory inv = b->getInventory();
    return (inv.curInv[Resource::BRICK] >= 1 && inv.curInv[Resource::ENERGY] >= 1 && inv.curInv[Resource::GLASS] >= 1 && inv.curInv[Resource::WIFI] >= 1);
}

// checkH(b) checks if builder has enough resources to build a house.
bool checkH(Builder *b)
{
    Inventory inv = b->getInventory();
    return (inv.curInv[Resource::GLASS] >= 2 && inv.curInv[Resource::HEAT] >= 3);
}

// checkT(b) checks if builder has enough resources to build a tower.
bool checkT(Builder *b)
{
    Inventory inv = b->getInventory();
    return (inv.curInv[Resource::BRICK] >= 3 && inv.curInv[Resource::ENERGY] >= 2 && inv.curInv[Resource::GLASS] >= 2 && inv.curInv[Resource::WIFI] >= 1 && inv.curInv[Resource::HEAT] >= 2);
}

// checkR(b) checks if builder has enough resources to build a road.
bool checkR(Builder *b)
{
    return (b->getInventory().curInv[Resource::HEAT] >= 1 && b->getInventory().curInv[Resource::WIFI] >= 1);
}

// consumeB(b) updates builder's inventory after building a basement.
void consumeB(Builder *b)
{
    b->getInventory().curInv[Resource::BRICK] -= 1;
    b->getInventory().curInv[Resource::ENERGY] -= 1;
    b->getInventory().curInv[Resource::GLASS] -= 1;
    b->getInventory().curInv[Resource::WIFI] -= 1;
    b->getInventory().reconcile();
}

// consumeH(b) updates builder's inventory after building a house.
void consumeH(Builder *b)
{
    b->getInventory().curInv[Resource::GLASS] -= 2;
    b->getInventory().curInv[Resource::HEAT] -= 3;
    b->getInventory().reconcile();
}

// consumeT(b) updates builder's inventory after building a tower.
void consumeT(Builder *b)
{
    b->getInventory().curInv[Resource::BRICK] -= 3;
    b->getInventory().curInv[Resource::ENERGY] -= 2;
    b->getInventory().curInv[Resource::GLASS] -= 2;
    b->getInventory().curInv[Resource::WIFI] -= 1;
    b->getInventory().curInv[Resource::HEAT] -= 2;
    b->getInventory().reconcile();
}

// consumeR(b) updates builder's inventory after building a road.
void consumeR(Builder *b)
{
    b->getInventory().curInv[Resource::HEAT] -= 1;
    b->getInventory().curInv[Resource::WIFI] -= 1;
    b->getInventory().reconcile();
}

// Main Functions ( Public Methods )

Board::Board() {}

void Board::upgradeVertex(int vertexNum, Builder *builder) {
    Colour bColour = builder->getColour();
    Vertex &tVertex = vertexList[vertexNum];
    Colour vColour = tVertex.getColour();
    if (vColour == bColour) // residence upgrade
    {
        // check residence type and resources onhand
        if (tVertex.getResidence() == Residence::B && checkH(builder))
        {
            tVertex.updateRes();
            builder->updateRes(vertexNum);
            builder->updatePoint(1);
            consumeH(builder);
            cout << colour2Str(bColour) << " upgraded " << vertexNum << endl;
        }
        else if (tVertex.getResidence() == Residence::H && checkH(builder))
        {
            tVertex.updateRes();
            builder->updateRes(vertexNum);
            builder->updatePoint(1);
            consumeT(builder);
            cout << colour2Str(bColour) << " upgraded " << vertexNum << endl;
        }
        else
        {
            cout << "Either the player's resources are not sufficient, or the player already has a tower on the vertex." << endl;
        }
    }
    else
    {
        cout << "You don't have a Residence on this Vertex: " << vertexNum << endl; 
    }
}

void Board::colourVertex(int vertexNum, Builder *builder, bool start)
{
    Colour bColour = builder->getColour();

    if (!start) // initialize the very first two residences for each player
    {
        Vertex &tVertex = vertexList[vertexNum];
        Colour vColour = tVertex.getColour();
        if (vColour != Colour::NoColour)
        {
            cout << vertexNum << " is occupied." << endl; // 1)
        }
        else // build basement
        {
            if (!checkB(builder)) // check resources on hand to build a basement
            {
                cout << "You do not have enough resources" << endl;
            }
            else
            {
                vector<Edge *> arl = tVertex.getAdjacentRoad();
                for (auto ar : arl)
                {
                    if (ar->getColour() == builder->getColour())
                    {
                        tVertex.setColour(bColour);
                        tVertex.setBuilder(builder);
                        tVertex.updateRes();
                        builder->updateRes(vertexNum);
                        builder->updatePoint(1);
                        consumeB(builder);
                        cout << colour2Str(bColour) << " built " << vertexNum << endl;
                        return;
                    }
                }
                cout << vertexNum << " You don't have a road nearby." << endl; // 1)
            }
        }
    }
    else // if at least one player has not picked their initial two vertices
    {
        if (builder->getResList().size() < 2) // double-check
        {
            cout << "Builder " << colour2Str(builder->getColour()) << ": You have basements at: " << endl;
            for (auto r : builder->getResList())
            {
                cout << r.first << " " << residence2Str(r.second) << endl;
            }

            cout << "Builder " << colour2Str(builder->getColour()) << ", where do you want to build a basement?" << endl << "> ";
            while (true)
            {
                if (cin >> vertexNum)
                {
                    if ((vertexNum > 53) || (vertexNum < 0)) {
                        cout << "Enter a valid vertex" << endl;
                        continue;
                    }
                    Vertex &tVertex = vertexList[vertexNum];
                    Colour vColour = tVertex.getColour();
                    if (vColour != Colour::NoColour)
                    {
                        cout << vertexNum << " is occupied by another player." << endl;
                    }
                    else
                    {
                        tVertex.setColour(bColour);
                        tVertex.setBuilder(builder);
                        tVertex.updateRes();
                        builder->updateRes(vertexNum);
                        builder->updatePoint(1);
                        cout << colour2Str(bColour) << " built " << vertexNum << endl;
                        break;
                    }
                }
                if (cin.eof()) {
                    throw true;
                }
                cin.clear();
                cin.ignore();
            }
        }
    }
}

void Board::colourEdge(int edgeNum, Builder *builder)
{
    Edge &tEdge = edgeList[edgeNum];
    Colour bColour = builder->getColour();
    if (tEdge.getColour() != Colour::NoColour) // check if edge is already taken
    {
        cout << "There's already a road on this edge." << endl;
    }
    else if (!checkR(builder)) // check if builder has enough resources to build a road
    {
        cout << "You do not have enough resources" << endl;
    }
    else
    {
        for (auto v : tEdge.getVertices())
        {
            if (v->getColour() == bColour) // an adjacent residence has been built by the same builder
            {
                tEdge.setColour(bColour);
                builder->updateRoad(edgeNum);
                consumeR(builder);
                cout << "You have built a road on edge " << edgeNum << endl;
                return;
            }
            for (auto e : v->getAdjacentRoad())
            {
                if (e->getColour() == bColour && v->getColour() == Colour::NoColour) // an adjacent road has been built by the same builder
                {
                    tEdge.setColour(bColour);
                    builder->updateRoad(edgeNum);
                    consumeR(builder);
                    cout << "You have built a road on edge " << edgeNum << endl;
                    return;
                }
            }
        }
        cout << "Can't build a road on edge " << edgeNum << endl;
    }
}

void Board::updateResOnTile(int tileNum)
{
    int geeseNum = this->getGeese().getTileNumber();
    for (auto t : tileList)
    {
        if (t.getResourceNum() == tileNum)
        {
            if (t.getNum() == geeseNum) // Geese on the matched tile
            {
                cout << "Geese on the matched tile # " << t.getNum() << endl;
                cout << "Hence, tile # " << t.getNum() << " does not distribute resources." << endl;
            }
            else // send presents
            {
                t.notifyObservers();
            }
        }
    }
    // need to print messages in roll()   *** After each print, call reconcile() on builder's inventory ***
}

// helper functions for display():

// stdNum returns the string form of num, and add a space before num if it has only one digit
string stdNum(int num)
{
    if (num < 10)
    {
        return " " + std::to_string(num);
    }
    return std::to_string(num);
}

// checkResidence(vertexNum) returns the string representation for vertex
string Board::checkResidence(int vertexNum) const
{
    Colour colour = vertexList[vertexNum].getColour();

    // no residence build on vertex
    if (colour == Colour::NoColour)
    {
        return "|" + stdNum(vertexNum) + "|";
    }

    // get string representation for colour
    string strColour;
    if (colour == Colour::Blue)
    {
        strColour = "B";
    }
    else if (colour == Colour::Red)
    {
        strColour = "R";
    }
    else if (colour == Colour::Orange)
    {
        strColour = "O";
    }
    else /* if (colour = Colour::Yellow) */
    {
        strColour = "Y";
    }

    // get string representation for residence
    string strResidence;
    Residence residence = vertexList[vertexNum].getResidence();
    if (residence == Residence::B)
    {
        strResidence = "B";
    }
    else if (residence == Residence::H)
    {
        strResidence = "H";
    }
    else
    {
        strResidence = "T";
    }

    return "|" + strColour + strResidence + "|";
}

// checkRoad(edgeNum) returns the string representation for edge
string Board::checkRoad(int edgeNum) const
{
    Colour colour = edgeList[edgeNum].getColour();

    // no road build on edge
    if (edgeList[edgeNum].getColour() == Colour::NoColour)
    {
        return stdNum(edgeNum);
    }

    // get string representation for colour
    string strColour;
    if (colour == Colour::Blue)
    {
        strColour = "B";
    }
    else if (colour == Colour::Red)
    {
        strColour = "R";
    }
    else if (colour == Colour::Orange)
    {
        strColour = "O";
    }
    else /* if (colour = Colour::Yellow) */
    {
        strColour = "Y";
    }

    return strColour + "R";
}

// checkResourceName(tileNum) returns the string representation for the resource
string Board::checkResourceName(int tileNum) const
{
    Resource resource = tileList[tileNum].getResourceType();
    if (resource == Resource::BRICK)
    {
        return "  BRICK  ";
    }
    else if (resource == Resource::ENERGY)
    {
        return " ENERGY  ";
    }
    else if (resource == Resource::GLASS)
    {
        return "  GLASS  ";
    }
    else if (resource == Resource::HEAT)
    {
        return "  HEAT   ";
    }
    else if (resource == Resource::WIFI)
    {
        return "  WIFI   ";
    }
    else
    {
        return "  PARK   ";
    }
}

// checkResourceNum(tileNum) returns the string representation for the number of resource
string Board::checkResourceNum(int tileNum) const
{
    Resource resource = tileList[tileNum].getResourceType();
    if (resource == Resource::PARK)
    {
        return "  ";
    }
    int resourceNum = tileList[tileNum].getResourceNum();
    return stdNum(resourceNum);
}

void Board::display() const
{
    int vertexNum = 0;
    int edgeNum = 0;
    int tileNum = 0;
    int tileNameNum = 0;
    int tileValueNum = 0;
    int geeseNum = 0;
    int geeseTile = geese.getTileNumber();

    // line 1
    cout << "                          " << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum) << endl;
    ++vertexNum;

    // line 2
    cout << "                            |         |" << endl;

    // line 3
    cout << "                           ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "   ";
    cout << stdNum(tileNum);
    ++tileNum;
    cout << "   ";
    cout << checkRoad(edgeNum) << endl;
    ++edgeNum;

    // line 4
    cout << "                            |";
    cout << checkResourceName(tileNameNum);
    ++tileNameNum;
    cout << "|" << endl;

    // line 5
    cout << "                ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "  ";
    cout << checkResourceNum(tileValueNum);
    ++tileValueNum;
    cout << "  ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum) << endl;
    ++vertexNum;

    // line 6
    cout << "                  |         |";
    if (geeseNum == geeseTile) 
    {
        cout << "  GEESE  "; 
    }
    else
    {
        cout << "         ";
    }
    ++geeseNum;
    cout << "|         |" << endl;

    // line 7
    cout << "                 ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "   ";
    cout << stdNum(tileNum);
    ++tileNum;
    cout << "   ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "        ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "   ";
    cout << stdNum(tileNum);
    ++tileNum;
    cout << "   ";
    cout << checkRoad(edgeNum) << endl;
    ++edgeNum;

    // line 8
    cout << "                  |";
    cout << checkResourceName(tileNameNum);
    ++tileNameNum;
    cout << "|         |";
    cout << checkResourceName(tileNameNum);
    ++tileNameNum;
    cout << "|" << endl;

    // repeat line 9-16, 17-24, 25-32
    for (int i = 0; i < 3; ++i)
    {

        // line 9
        cout << "      ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "--";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "--";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "  ";
        cout << checkResourceNum(tileValueNum);
        ++tileValueNum;
        cout << "  ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "--";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "--";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "  ";
        cout << checkResourceNum(tileValueNum);
        ++tileValueNum;
        cout << "  ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "--";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "--";
        cout << checkResidence(vertexNum) << endl;
        ++vertexNum;

        // line 10
        cout << "        |         |";
        if (geeseNum == geeseTile)
        {
            cout << "  GEESE  "; 
        }
        else
        {
            cout << "         ";
        }
        ++geeseNum;
        cout << "|         |";
        if (geeseNum == geeseTile)
        {
            cout << "  GEESE  "; 
        }
        else
        {
            cout << "         ";
        }
        ++geeseNum;
        cout << "|         |" << endl;

        // line 11
        cout << "       ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "   ";
        cout << stdNum(tileNum);
        ++tileNum;
        cout << "   ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "        ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "   ";
        cout << stdNum(tileNum);
        ++tileNum;
        cout << "   ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "        ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "   ";
        cout << stdNum(tileNum);
        ++tileNum;
        cout << "   ";
        cout << checkRoad(edgeNum) << endl;
        ++edgeNum;

        // line 12
        cout << "        |";
        cout << checkResourceName(tileNameNum);
        ++tileNameNum;
        cout << "|         |";
        cout << checkResourceName(tileNameNum);
        ++tileNameNum;
        cout << "|         |";
        cout << checkResourceName(tileNameNum);
        ++tileNameNum;
        cout << "|" << endl;

        // line 13
        cout << "      ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "  ";
        cout << checkResourceNum(tileValueNum);
        ++tileValueNum;
        cout << "  ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "--";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "--";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "  ";
        cout << checkResourceNum(tileValueNum);
        ++tileValueNum;
        cout << "  ";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "--";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "--";
        cout << checkResidence(vertexNum);
        ++vertexNum;
        cout << "  ";
        cout << checkResourceNum(tileValueNum);
        ++tileValueNum;
        cout << "  ";
        cout << checkResidence(vertexNum) << endl;
        ++vertexNum;

        // line 14
        cout << "        |";
        if (geeseNum == geeseTile)
        {
            cout << "  GEESE  "; 
        }
        else
        {
            cout << "         ";
        }
        ++geeseNum;
        cout << "|         |";
        if (geeseNum == geeseTile)
        {
            cout << "  GEESE  "; 
        }
        else
        {
            cout << "         ";
        }
        ++geeseNum;
        cout << "|         |";
        if (geeseNum == geeseTile)
        {
            cout << "  GEESE  "; 
        }
        else
        {
            cout << "         ";
        }
        ++geeseNum;
        cout << "|" << endl;

        // line 15
        cout << "       ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "        ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "   ";
        cout << stdNum(tileNum);
        ++tileNum;
        cout << "   ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "        ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "   ";
        cout << stdNum(tileNum);
        ++tileNum;
        cout << "   ";
        cout << checkRoad(edgeNum);
        ++edgeNum;
        cout << "        ";
        cout << checkRoad(edgeNum) << endl;
        ++edgeNum;

        // line 16
        cout << "        |         |";
        cout << checkResourceName(tileNameNum);
        ++tileNameNum;
        cout << "|         |";
        cout << checkResourceName(tileNameNum);
        ++tileNameNum;
        cout << "|         |" << endl;
    }

    // line 33
    cout << "      ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "  ";
    cout << checkResourceNum(tileValueNum);
    ++tileValueNum;
    cout << "  ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "  ";
    cout << checkResourceNum(tileValueNum);
    ++tileValueNum;
    cout << "  ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum) << endl;
    ++vertexNum;

    // line 34
    cout << "                  |";
    if (geeseNum == geeseTile)
    {
        cout << "  GEESE  "; 
    }
    else 
    {
        cout << "         ";
    }
    ++geeseNum;
    cout << "|         |";
    if (geeseNum == geeseTile)
    {
        cout << "  GEESE  "; 
    }
    else 
    {
        cout << "         ";
    }
    ++geeseNum;
    cout << "|" << endl;

    // line 35
    cout << "                 ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "        ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "   ";
    cout << stdNum(tileNum);
    ++tileNum;
    cout << "   ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "        ";
    cout << checkRoad(edgeNum) << endl;
    ++edgeNum;

    // line 36
    cout << "                  |         |";
    cout << checkResourceName(tileNameNum);
    ++tileNameNum;
    cout << "|         |" << endl;

    // line 37
    cout << "                ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "  ";
    cout << checkResourceNum(tileValueNum);
    ++tileValueNum;
    cout << "  ";
    cout << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum) << endl;
    ++vertexNum;

    // line 38
    cout << "                            |";
    if (geeseNum == geeseTile)
    {
        cout << "  GEESE  "; 
    }
    else 
    {
        cout << "         ";
    }
    ++geeseNum;
    cout << "|" << endl;

    // line 39
    cout << "                           ";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "        ";
    cout << checkRoad(edgeNum) << endl;
    ++edgeNum;

    // line 40
    cout << "                            |         |" << endl;

    // line 41
    cout << "                          " << checkResidence(vertexNum);
    ++vertexNum;
    cout << "--";
    cout << checkRoad(edgeNum);
    ++edgeNum;
    cout << "--";
    cout << checkResidence(vertexNum) << endl;
    ++vertexNum;
}

vector<Tile> Board::getTileList() const
{
    return this->tileList;
}

void Board::save(const string fileName)
{
    ofstream outfile;
    outfile.open(fileName, std::ios_base::app);

    for (size_t i = 0; i < tileList.size(); ++i)
    {
        Resource resource = tileList[i].getResourceType();
        if (resource == Resource::PARK)
        {
            if (i == tileList.size() - 1)
            {
                outfile << "5 7";
            }
            else
            {
                outfile << "5 7 ";
            }
            continue;
        }
        else if (resource == Resource::BRICK)
        {
            outfile << "0 ";
        }
        else if (resource == Resource::ENERGY)
        {
            outfile << "1 ";
        }
        else if (resource == Resource::GLASS)
        {
            outfile << "2 ";
        }
        else if (resource == Resource::HEAT)
        {
            outfile << "3 ";
        }
        else if (resource == Resource::WIFI)
        {
            outfile << "4 ";
        }
        if (i == tileList.size() - 1)
        {
            outfile << tileList[i].getResourceNum();
        }
        else
        {
            outfile << tileList[i].getResourceNum() << " ";
        }
    }
    outfile << '\n';
}

// Helper functions for load()

// observerListInit() initializes the observerList of each tile object
void Board::observerListInit()
{
    // first tile 0
    int vStart = 0;
    tileList[0].attach(&vertexList[vStart]);
    tileList[0].attach(&vertexList[vStart + 3]);
    tileList[0].attach(&vertexList[vStart + 3 + 5]);
    tileList[0].attach(&vertexList[vStart + 1]);
    tileList[0].attach(&vertexList[vStart + 1 + 3]);
    tileList[0].attach(&vertexList[vStart + 1 + 3 + 5]);

    // first group of tiles {3,4,5,8,9,10,13,14,15}
    vStart = 6;
    for (int i = 3; i <= 13; i += 5)
    {
        for (int j = 0; j < 3; j++)
        {
            tileList[i + j].attach(&vertexList[vStart]);
            tileList[i + j].attach(&vertexList[vStart + 1]);
            tileList[i + j].attach(&vertexList[vStart + 6]);
            tileList[i + j].attach(&vertexList[vStart + 6 + 1]);
            tileList[i + j].attach(&vertexList[vStart + 12]);
            tileList[i + j].attach(&vertexList[vStart + 12 + 1]);

            vStart += 2;
        }
        vStart += 6;
    }
    vStart = 2;
    // second group {1, 2}
    for (int i = 1; i <= 2; i++)
    {
        tileList[i].attach(&vertexList[vStart]);
        tileList[i].attach(&vertexList[vStart + 1]);
        tileList[i].attach(&vertexList[vStart + 5]);
        tileList[i].attach(&vertexList[vStart + 5 + 1]);
        tileList[i].attach(&vertexList[vStart + 11]);
        tileList[i].attach(&vertexList[vStart + 11 + 1]);
        vStart += 2;
    }
    // third group {6,7,11,12}
    vStart = 13;
    for (int i = 6; i <= 12; i += 5)
    {
        for (int j = 0; j < 2; j++)
        {
            tileList[i + j].attach(&vertexList[vStart]);
            tileList[i + j].attach(&vertexList[vStart + 1]);
            tileList[i + j].attach(&vertexList[vStart + 6]);
            tileList[i + j].attach(&vertexList[vStart + 6 + 1]);
            tileList[i + j].attach(&vertexList[vStart + 12]);
            tileList[i + j].attach(&vertexList[vStart + 12 + 1]);

            vStart += 2;
        }
        vStart += 8;
    }

    // fourth group {16, 17}
    vStart = 37;
    for (int i = 16; i <= 17; ++i)
    {
        tileList[i].attach(&vertexList[vStart]);
        tileList[i].attach(&vertexList[vStart + 6]);
        tileList[i].attach(&vertexList[vStart + 6 + 5]);
        tileList[i].attach(&vertexList[vStart + 1]);
        tileList[i].attach(&vertexList[vStart + 1 + 6]);
        tileList[i].attach(&vertexList[vStart + 1 + 6 + 5]);
    }

    // last vertex
    vStart = 44;
    tileList[18].attach(&vertexList[vStart]);
    tileList[18].attach(&vertexList[vStart + 5]);
    tileList[18].attach(&vertexList[vStart + 5 + 3]);
    tileList[18].attach(&vertexList[vStart + 1]);
    tileList[18].attach(&vertexList[vStart + 1 + 5]);
    tileList[18].attach(&vertexList[vStart + 1 + 5 + 3]);
}

// Helper Function
void Board::verEdgInitHelper(int startEdge, int startDiff, int rows)
{
    const int rowDiff = 5;
    for (int i = 0; i < rows; ++i)
    {
        int currEdge = startEdge + (i * 17);
        int leftVertex = currEdge - (rowDiff * i) - startDiff;
        edgeList[currEdge].setVertex(&vertexList[leftVertex]);
        edgeList[currEdge].setVertex(&vertexList[leftVertex + 1]);

        vertexList[leftVertex].setEdge(&edgeList[currEdge]);
        vertexList[leftVertex + 1].setEdge(&edgeList[currEdge]);
    }
}

void Board::verEdgInit()
{
    // 0
    verEdgInitHelper(0, 0);

    // {1,2}
    int vStart = 0;
    for (int i = 1; i <= 2; i++)
    {
        edgeList[i].setVertex(&vertexList[vStart]);
        edgeList[i].setVertex(&vertexList[vStart + 3]);
        vertexList[vStart].setEdge(&edgeList[i]);
        vertexList[vStart + 3].setEdge(&edgeList[i]);
        vStart++;
    }

    // {3,4}
    verEdgInitHelper(3, 1);
    verEdgInitHelper(4, 0);

    // {5,6,7,8}
    vStart = 2;
    for (int i = 5; i <= 8; i++)
    {
        edgeList[i].setVertex(&vertexList[vStart]);
        edgeList[i].setVertex(&vertexList[vStart + 5]);
        vertexList[vStart].setEdge(&edgeList[i]);
        vertexList[vStart + 5].setEdge(&edgeList[i]);
        vStart++;
    }

    // {9,26,43,60}
    verEdgInitHelper(9, 3, 4);

    // (11, 28, 45, 62)
    verEdgInitHelper(11, 1, 4);

    // (18, 35, 52)
    verEdgInitHelper(18, 5, 3);

    // (19, 36, 53)
    verEdgInitHelper(19, 4, 3);

    // (10, 27, 44, 61)
    verEdgInitHelper(10, 2, 4);

    // {12,...,17, 20,...,25,29,...,34,37,...42,46,...,51,54,...,59}
    vStart = 6;
    int count = 1;
    for (int i = 12; i <= 54; i += (8 + count))
    {
        for (int j = 0; j <= 5; j++)
        {
            edgeList[i + j].setVertex(&vertexList[vStart]);
            edgeList[i + j].setVertex(&vertexList[vStart + 6]);
            vertexList[vStart].setEdge(&edgeList[i + j]);
            vertexList[vStart + 6].setEdge(&edgeList[i + j]);
            vStart++;
        }
        count == 0 ? count++ : count--;
    }

    // {63,64,65,66}
    vStart = 43;
    for (int i = 63; i <= 66; i++)
    {
        edgeList[i].setVertex(&vertexList[vStart]);
        edgeList[i].setVertex(&vertexList[vStart + 5]);
        vertexList[vStart].setEdge(&edgeList[i]);
        vertexList[vStart + 5].setEdge(&edgeList[i]);
        vStart++;
    }

    // {69,70}
    vStart = 49;
    for (int i = 69; i <= 70; i++)
    {
        edgeList[i].setVertex(&vertexList[vStart]);
        edgeList[i].setVertex(&vertexList[vStart + 3]);
        vertexList[vStart].setEdge(&edgeList[i]);
        vertexList[vStart + 3].setEdge(&edgeList[i]);
        vStart++;
    }

    // (67, 68)
    verEdgInitHelper(67, 19);
    verEdgInitHelper(68, 18);

    // (71)
    verEdgInitHelper(71, 19);
}


string Board::loadFromFile(const string fileName) 
{
    const int BOARD_LAYOUT_LINE = 5;
    string layout;
    ifstream file{fileName};

    for (int i = 0; i <= BOARD_LAYOUT_LINE; ++i)
    {
        getline(file, layout);
    }
    load(layout);
    return layout;
}

void Board::load(string layoutStr)
{
    vector<pair<Resource, int>> layoutVector;
    istringstream ss{layoutStr};
    string resourceStr;
    string tileValStr;
    while (ss >> resourceStr && ss >> tileValStr)
    {
        pair<Resource, int> tilePair;

        if (resourceStr == "0")
        {
            tilePair.first = Resource::BRICK;
        }
        else if (resourceStr == "1")
        {
            tilePair.first = Resource::ENERGY;
        }
        else if (resourceStr == "2")
        {
            tilePair.first = Resource::GLASS;
        }
        else if (resourceStr == "3")
        {
            tilePair.first = Resource::HEAT;
        }
        else if (resourceStr == "4")
        {
            tilePair.first = Resource::WIFI;
        }
        else
        {
            tilePair.first = Resource::PARK;
        }

        istringstream ss2{tileValStr};
        int i;
        ss2 >> i;
        tilePair.second = i;

        layoutVector.emplace_back(tilePair);
    }
    loadWithLayout(layoutVector);
}

string Board::loadRandomBoard()
{
    vector<pair<Resource, int>> layoutVector;

    // add 7-PARK
    layoutVector.emplace_back(make_pair(Resource::PARK, 7));


    // 3 WIFI, 3 HEAT, 4 BRICK, 4 ENERGY, 4 GLASS, and 1 PARK
    // The values on the board will have the
    // distribution of one tile with value 2, one tile with the value 12, one tile with the value 7 (the Park), and two tiles each for the
    // values: 3, 4, 5, 6, 8, 9, 10, and 11.

    // 1 PARK NOT INCLUDED: 3 WIFI, 3 HEAT, 4 BRICK, 4 ENERGY, 4 GLASS
    vector<Resource> allTileTypes;

    for (int i = 0; i < 3; ++i) {
        allTileTypes.emplace_back(Resource::WIFI);
        allTileTypes.emplace_back(Resource::HEAT);
    }

    for (int i = 0; i < 4; ++i) {
        allTileTypes.emplace_back(Resource::BRICK);
        allTileTypes.emplace_back(Resource::ENERGY);
        allTileTypes.emplace_back(Resource::GLASS);
    }

    // one 7 NOT INCLUDED: one 2, one 12, two 3,4,5,6,8,9,10,11
    vector<int> allTileVals;

    allTileVals.emplace_back(2);
    allTileVals.emplace_back(12);
    for (int i = 0; i < 2; ++i) {
        allTileVals.emplace_back(3);
        allTileVals.emplace_back(4);
        allTileVals.emplace_back(5);
        allTileVals.emplace_back(6);
        allTileVals.emplace_back(8);
        allTileVals.emplace_back(9);
        allTileVals.emplace_back(10);
        allTileVals.emplace_back(11);
    }

    // shuffle both and make pairs

    shuffle(allTileTypes.begin(), allTileTypes.end(), default_random_engine(seed));
    shuffle(allTileVals.begin(), allTileVals.end(), default_random_engine(seed));

    for (size_t i = 0; i < allTileTypes.size(); ++i) {
        layoutVector.emplace_back(allTileTypes[i], allTileVals[i]);
    }


    // SHUFFLE LAYOUT PAIR VECTOR
    shuffle(layoutVector.begin(), layoutVector.end(), default_random_engine(seed));
    

    loadWithLayout(layoutVector);

    string layoutStr = "";
    for (auto pair : layoutVector) {
        layoutStr.append(" " + to_string(static_cast<int>(pair.first)));
        layoutStr.append(" " + to_string(pair.second));
    }
    return layoutStr;
}

void Board::loadWithLayout(vector<pair<Resource, int>> layoutVector)
{
    // construct all edges and vertices
    const int TOTAL_VERTICES = 53;
    const int TOTAL_EDGES = 71;

    for (int i = 0; i <= TOTAL_VERTICES; ++i) {
        Vertex v{i};
        vertexList.emplace_back(v);
    }

    for (int i = 0; i <= TOTAL_EDGES; ++i) {
        Edge e{i};
        edgeList.emplace_back(e);
    }

    // construct Tiles with values
    int count = 0;
    for (auto tilePair : layoutVector)
    {
        Tile t{count, tilePair.first, tilePair.second};
        tileList.emplace_back(t);
        ++count;
    }

    // attach observers
    observerListInit();

    // establish vector-edge relationship
    verEdgInit();
}

void Board::setGeese(Geese geese) {
    this->geese = geese;
}

vector<Vertex> & Board::getVertexList() {
    return vertexList;
}

Geese & Board::getGeese() {
    return geese;
}

vector<Edge> & Board::getEdgeList() {
    return edgeList;
}
