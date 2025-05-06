#include "geese.h"
#include <fstream>
#include <iostream>
using namespace std;

Geese::Geese(): tileNumber{0} {}

void Geese::moveGeese()
{
    cout << "Choose where to place the GEESE." << endl << "> ";
    int i;
    while (true)
    {
        cin >> i;
        if (!cin.fail())
        {
            if (tileNumber == i)
            {
                cout << "The geese may not be placed on the tile on which they were previously." << endl;
                cout << "Choose where to place the GEESE." << endl << "> ";
            }
            else
            {
                this->tileNumber = i;
                return;
            }
        }
        else
        {
            if (cin.eof()) {
                throw true;
            }
            cin.clear();
            cin.ignore();
            cout << "Please enter a valid integer" << endl;
            cout << "Choose where to place the GEESE." << endl << "> ";
        }
    }
}

void Geese::setTileNumber(const int tileNum) {
    tileNumber = tileNum;
}

int Geese::getTileNumber() const
{
    return tileNumber;
}

Geese::~Geese() {}

void Geese::save(const string fileName)
{
    ofstream outfile;
    outfile.open(fileName, std::ios_base::app);

    outfile << to_string(this->tileNumber) << '\n';
}
