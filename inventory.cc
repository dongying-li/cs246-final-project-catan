#include "inventory.h"
#include <string>
using namespace std;

void Inventory::reconcile()
{
    oldInv = curInv;
}

int Inventory::totalInvCount() 
{
    int sum = 0;
    for (auto resourcePair : curInv) 
    {
        sum += resourcePair.second;
    }
    return sum;
}

vector<Resource> Inventory::vectorizeInv() 
{
    vector<Resource> v;
    for (auto resourcePair : curInv)
    {
        for (int i = 0; i < resourcePair.second; ++i)
        {
            v.emplace_back(resourcePair.first);
        }
    }
    return v;
}
