#ifndef _FAIRDICE_H
#define _FAIRDICE_H
#include "dice.h"

class FairDice : public Dice {
    public:
        FairDice();
        int getNumber() const override;
};

#endif
