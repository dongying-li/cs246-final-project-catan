#include <iostream>
#include <string>
#include <sstream>
#include "catan.h"
using namespace std;

int main(int argc, char *argv[])
{

    // cmd line interface
    bool givenBoard = false;

    Catan *catan = new Catan{};
    string boardLayout;

    for (int i = 1; i < argc; ++i)
    {
        string str = argv[i];
        if (str == "-seed")
        {
            if (i + 1 < argc)
            {
                string s = argv[i + 1];
                istringstream ss{s};
                ss >> seed;
            }
        }
        else if (str == "-load")
        {
            if (i + 1 < argc)
            {
                boardLayout = catan->loadGameState(argv[i + 1]);
                givenBoard = true;
            }
        }
        else if (str == "-board")
        {
            if (i + 1 < argc)
            {
                boardLayout = catan->loadBoardOnly(argv[i + 1]);
                givenBoard = true;
            }
        }
        else if (str == "-random-board" && !givenBoard)
        {
            givenBoard = false;
        }
    }

    if (!givenBoard)
    {
        boardLayout = catan->loadRandom();
    }

    // 4.1
    try {
        catan->initialSetup();

        while (true)
        {

            while (catan->checkWinner() == Colour::NoColour)
            {
                catan->playTurn();
            }

            // game ended
            cout << "Would you like to play again?" << endl
                << "> ";
            string i;
            while (cin >> i)
            {
                if (i == "yes")
                {
                    delete catan;
                    catan = new Catan{};
                    catan->loadBoardOnlyFromStr(boardLayout);
                    catan->initialSetup();
                    break;
                }
                else if (i == "no")
                {
                    delete catan;
                    return 0;
                }
            }
            if (cin.eof()) {
                throw true;
            }
        }
    } catch (bool) {
        cout << "User Entered EOF - Game over. Current game state is saved to backup.sv." << endl;
        catan->save("backup.sv");
    }
    
    delete catan;
    return 0;
}
