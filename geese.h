#ifndef _GEESE_H
#define _GEESE_H
#include <string>

class Geese {
    int tileNumber;

    public:
        Geese();
        void moveGeese();
        void setTileNumber(const int tileNum);
        int getTileNumber() const;
        void save(const std::string fileName);
        ~Geese();
};

#endif
