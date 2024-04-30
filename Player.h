//
//  Player.h
//  Project3
//
//  Created by Tommy Tan on 5/23/23.
//

#ifndef Player_h
#define Player_h

#include <iostream>
#include <string>
#include "Board.h"
#include "Side.h"


class Player
{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string m_name;
};

//Human Player
class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    ~HumanPlayer();
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    ~BadPlayer();
    virtual int chooseMove(const Board& b, Side s) const;

};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    ~SmartPlayer();
    virtual int chooseMove(const Board& b, Side s) const;
private:
    void chooseMove(const Board& b, int & bestHole, double & value, int depth, Side s, Timer& timer) const;
};








#endif /* Player_h */
