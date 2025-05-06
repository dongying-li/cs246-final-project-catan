#ifndef _CATAN_H
#define _CATAN_H
#include "board.h"
#include "dice.h"
#include "enum.h"
#include <string>
#include <vector>

class Builder;

class Catan
{
    Board *board;
    std::vector<Builder *> builderList;
    size_t activeBuilderIndex;

    void distribute(const int tile);
    void deduct(const int deductFactor, const int bound);

    void nextPlayer();
    void swapDice(const DiceType type);
    void nextTurn();

    void status() const;
    void trade(std::string colour, std::string give, std::string take);
    void steal();

    void buildRes(int vertexNum, Builder *builder);
    void improveRes(int vertexNum, Builder *builder);
    void buildRoad(int edgeNum, Builder *builder);

public:
    Catan();
    ~Catan();

    std::string loadGameState(const std::string fileName); // Board is set here
    std::string loadBoardOnly(const std::string fileName);
    void loadBoardOnlyFromStr(const std::string layoutStr);
    std::string loadRandom();

    void save(const std::string fileName = "backup.sv");
    void initialSetup();
    
    Colour checkWinner() const;
    void playTurn();
};

#endif
