#include "fairdice.h"
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;

FairDice::FairDice() {};

int FairDice::getNumber() const {
    int result = 0;

    vector<int> rolls = {1, 2, 3, 4, 5, 6};
        
    // roll 1
    shuffle(rolls.begin(), rolls.end(), std::default_random_engine(seed));
    result += rolls.front();
    // roll 2
    shuffle(rolls.begin(), rolls.end(), std::default_random_engine(seed));
    result += rolls.front();
    
    return result;
}
