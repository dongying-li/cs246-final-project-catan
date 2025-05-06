#include <vector>
#include "subject.h"
#include "observer.h"
#include <iostream>
using namespace std;

Subject::~Subject() {}

void Subject::notifyObservers()
{
    for (auto ob : observers)
    {
        ob->notify(*this);
    }
}

void Subject::attach(Observer *ob)
{
    observers.emplace_back(ob);
}

std::vector<Observer *> Subject::getObservers()
{
    return observers;
}
