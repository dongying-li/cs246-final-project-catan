#ifndef _BUILDER_H_
#define _BUILDER_H_
#include <map>
#include <vector>
#include <string>
#include "inventory.h"
#include "enum.h"
#include "dice.h"

class Vertex;
class Edge;
class Inventory;

class Builder
{
    Inventory inventory;
    int points;
    std::map<int, Residence> resList;
    std::vector<int> roadList;
    Colour colour;
    Dice *dice;

    std::string residenceToString(const Residence residence) const;

public:
    Builder(Colour colour);
    ~Builder();

    void updateRes(int i);
    void updateRoad(int i);
    void updateInventory(Resource, Residence);
    bool afterDistribute(); // print the update after the resources are distributed and return true 
                            //  if builder received any resources. Also write updates back to oldInv.
                            //  oldInv and curInv should be the same after the function call.
    
    Inventory &getInventory();
    int getPoint();
    Colour getColour();
    std::map<int, Residence> getResList();
    void setRoadList(std::vector<int> roadList);
    void setResList(std::map<int, Residence> resList);
    void updatePoint(int p);
    
    void swapDice(const DiceType type);
    int roll(); 

    void save(const std::string fileName);
    void printStatus() const;
    void printResList() const;
    
};

#endif
