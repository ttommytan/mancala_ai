

#ifndef Board_h
#define Board_h

#include <vector>
#include <iostream>
#include "Side.h"

class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
    
private:
    std::vector<int> north;
    std::vector<int> south;
    int m_initialBeans;
    int m_nholes;
    
};


#endif /* Header_h */
