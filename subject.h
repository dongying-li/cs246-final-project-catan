#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "enum.h"

class Observer;

class Subject
{
    std::vector<Observer *> observers;

public:
    Subject() = default;
    void notifyObservers();
    void attach(Observer *ob);
    std::vector<Observer *> getObservers();
    virtual Resource getResourceType() const = 0;
    virtual ~Subject() = 0;
};

#endif
