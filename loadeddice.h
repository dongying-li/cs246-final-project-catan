#ifndef LOADEDDICE_H
#define LOADEDDICE_H
#include "dice.h"

class LoadedDice : public Dice {
    public:
        LoadedDice();
        int getNumber() const override;
};

#endif
