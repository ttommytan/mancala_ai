//
//  Game.h
//  Project3
//
//  Created by Tommy Tan on 5/24/23.
//

#ifndef Game_h
#define Game_h

#include <iostream>
#include "Board.h"
#include "Player.h"

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;

private:
    void printSpaces(int n = 10) const;
    Board m_board;
    Player * m_north;
    Player * m_south;
};

#endif /* Game_h */
