#ifndef _INVENTORY_H_
#define _INVENTROY_H_
#include <map>
#include <vector>
#include "enum.h"

struct Inventory
{
    std::map<Resource, int> curInv;
    std::map<Resource, int> oldInv;
    void reconcile();
    int totalInvCount();
    std::vector<Resource> vectorizeInv();
};

#endif
