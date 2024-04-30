//
//  Player.cpp
//  Project3
//
//  Created by Tommy Tan on 5/23/23.
//

#include "Player.h"
#include <iostream>
#include <string>

using namespace std;


Player::Player(std::string name)
{
    m_name = name;
}

std::string Player::name() const
{
    return m_name;
}
bool Player::isInteractive() const
{
    return false;
}

Player::~Player()
{
    
}

//Human Player
HumanPlayer::HumanPlayer(string name): Player(name)
{
}
HumanPlayer::~HumanPlayer()
{
}

bool HumanPlayer::isInteractive() const
{
    return true;
}

//what to return when no moves can be made at all
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    if(b.beansInPlay(s) == 0)
    {
        return -1;
    }
    int holeNum;
    //cin value from human for the move
    cout << name() << ", please choose a hole." << endl;
    cin >> holeNum;
    
    //if the move is not valid then keep on prompting until a valid number is chosen
    while(holeNum <= 0 || holeNum > b.holes() || b.beans(s, holeNum) == 0)
    {
        cout << "Invalid hole. Please choose another hole." << endl;
        cin >> holeNum;
    }
    return holeNum;
        
}

//bad player

BadPlayer::BadPlayer(std::string name) : Player(name)
{
    
}
BadPlayer::~BadPlayer()
{
    
}
int BadPlayer::chooseMove(const Board& b, Side s) const
{
    int holeNum = -1;
    //choose a hole from 1, and gradually increasing until it finds a valid hole
    for(int i = 1; i <= b.holes(); i++)
    {
        if(b.beans(s, i) == 0)
        {
            
        }
        else
        {
            holeNum = i;
            break;
        }
        if(i == b.holes())
        {
            return -1;
        }
    }
    return holeNum;
}


SmartPlayer::SmartPlayer(std::string name) : Player(name)
{
    
}

SmartPlayer::~SmartPlayer()
{
    
}

void SmartPlayer::chooseMove(const Board& b, int & bestHole, double & value, int depth, Side s, Timer& timer) const
{
    //if timer elapsed is almost five sencond then just evaluate the current value of the board
    if(timer.elapsed() >= 4985)
    {
        bestHole = -1;
        value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        return;
    }
    if(s == NORTH)
    {
        //base cases
        //if no more beans in play then evaluate to see if north or south wins
        //values are calculated in south's term so if south wins then the value is set to be high
        if(b.beansInPlay(NORTH) == 0)
        {
            bestHole = -1;
            int totalSouthBeans = b.beansInPlay(SOUTH) + b.beans(SOUTH, 0);
            int northPot = b.beans(NORTH, 0);
            if(totalSouthBeans > northPot)
            {
                value = 100000;
                return;
            }
            else if(totalSouthBeans < northPot)
            {
                value = -100000;
                return;
            }
            else
            {
                value = 0;
                return;
            }
        }
        //second base case where if the depth excceeds 10
        if(depth > 10)
        {
            bestHole = -1;
            //evaluate by simply subtracting
            value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
            return;
        }
        //for loop to make every possible move
        for(int i = 1; i <= b.holes(); i++)
        {
            Side endSide;
            int endHole;
            //only make the move if the hole is playable
            if(b.beans(NORTH, i) > 0)
            {
                //create temporary board so that any changes do not affect the actual board
                Board temp(b);
                temp.sow(NORTH, i, endSide, endHole);
                //if capture we sow again and look for best val
                if(endSide == NORTH && endHole != 0 && temp.beans(NORTH, endHole) == 1 && temp.beans(SOUTH, endHole) != 0)
                {
                    temp.moveToPot(NORTH, endHole, NORTH);
                    temp.moveToPot(SOUTH, endHole, NORTH);
                }
                
                //end in own pot, an additional turn
                if(endHole == 0 && endSide == NORTH) //was if
                {
                    int tempBestHole;
                    double tempVal = -1.5;
                    chooseMove(temp, tempBestHole, tempVal, depth + 1, NORTH, timer);//hole = 5
                    if(tempBestHole == -1)
                    {
                        value = tempVal;
                        bestHole = i;
                    }
                    //if value has not been "initialized" value will take on the firs tempval no matter what it is
                    else if(value == -1.5)
                    {
                        value = tempVal;
                        bestHole = i;
                    }
                    else if(tempVal < value)
                    {
                        value = tempVal;
                        bestHole = i;
                    }
                    
                    
                    //make sure to sow with the newfound besthole
                    temp.sow(NORTH, tempBestHole, endSide, endHole);
                    
                    //make sure to capture if the new position requires that
                    if(endSide == NORTH && endHole != 0 && temp.beans(NORTH, endHole) == 1 && temp.beans(SOUTH, endHole) != 0)
                    {
                        temp.moveToPot(NORTH, endHole, NORTH);
                        temp.moveToPot(SOUTH, endHole, NORTH);
                    }
                }
                
               
                int h2;
                double v2 = -1.5;
                //now check the counter moves of the opponent
                chooseMove(temp, h2, v2, depth+1, SOUTH, timer);
                if(h2 == -1) //if it has reached the base case
                {
                    value = v2;
                    bestHole = i;
                }
                else if(value == -1.5)//if value has not been intialized
                {
                    value = v2;
                    bestHole = i;
                }
                else if(v2 < value)//if the found value is lower than current value then replace old value with new value because as North, we want what's worse for SOuth
                {
                    value = v2;
                    bestHole = i;
                }
            }
            
        }//for loop ends
    }
    else
    {
        if(b.beansInPlay(SOUTH) == 0)
        {
            bestHole = -1;
            int totalNorthBeans = b.beansInPlay(NORTH) + b.beans(NORTH, 0);
            int southPot = b.beans(SOUTH, 0);
            if(totalNorthBeans > southPot)
            {
                value = -100000;
                return;
            }
            else if(totalNorthBeans < southPot)
            {
                value = 100000;
                return;
            }
            else
            {
                value = 0;
                return;
            }
        }
        if(depth > 10)
        {
            bestHole = -1;
            value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
            return;
        }
        for(int i = 1; i <= b.holes(); i++)
        {
            Side endSide;
            int endHole;
            if(b.beans(SOUTH, i) > 0)
            {
                Board temp(b);
                temp.sow(SOUTH, i, endSide, endHole);
                //if capture we sow again and look for best val
                if(endSide == SOUTH && endHole != 0 && temp.beans(SOUTH, endHole) == 1 && temp.beans(NORTH, endHole) != 0)
                {
                    temp.moveToPot(NORTH, endHole, SOUTH);
                    temp.moveToPot(SOUTH, endHole, SOUTH);
                }


                if(endHole == 0 && endSide == SOUTH)
                {
  
                    int tempBestHole;
                    double tempVal = -1.5;
                    chooseMove(temp, tempBestHole, tempVal, depth + 1, SOUTH, timer);
                    if(tempBestHole == -1)
                    {
                        value = tempVal;
                        bestHole = i;
                    }
                    else if(value == -1.5)
                    {
                        value = tempVal;
                        bestHole = i;
                    }
                    else if(tempVal > value)
                    {
                        value = tempVal;
                        bestHole = i;

                    }
                    //return;
                    temp.sow(NORTH, tempBestHole, endSide, endHole);
                    
                    if(endSide == SOUTH && endHole != 0 && temp.beans(SOUTH, endHole) == 1 && temp.beans(NORTH, endHole) != 0)
                    {
                        temp.moveToPot(NORTH, endHole, SOUTH);
                        temp.moveToPot(SOUTH, endHole, SOUTH);
                    }
                }
                int h2;
                double v2 = -1.5;
                chooseMove(temp, h2, v2, depth+1, NORTH, timer);
                if(h2 == -1)
                {
                    value = v2;
                    bestHole = i;
                }
                else if(value == -1.5)
                {
                    value = v2;
                    bestHole = i;
                }
                else if(v2 > value)//if the found value is higher than current value then replace old value with new value because we want the maximum value
                {
                    value = v2;
                    bestHole = i;
                }
            }
            
        }
    }
}




int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    Timer timer;
    //make appropratie move depending if north or south
    if(s == NORTH)
    {
        int bestHole = -1;
        double value = -1.5;
        int depth = 0;
        //call helper function to find bestHole
        chooseMove(b, bestHole, value, depth, NORTH, timer);
        return bestHole;
    }
    else
    {
        int bestHole = -1;
        double value = -1.5;
        int depth = 1;
        chooseMove(b, bestHole, value, depth, SOUTH, timer);
        return bestHole;
    }
}



