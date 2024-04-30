//
//  Board.cpp
//  Project3
//
//  Created by Tommy Tan on 5/23/23.
//

#include "Board.h"
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    //If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
    if(nHoles <= 0)
    {
        nHoles = 1;
    }
    if(nInitialBeansPerHole < 0)
    {
        nInitialBeansPerHole = 0;
    }
    m_nholes = nHoles;
    //setting the pots for south and north to zero
    north.push_back(0);
    south.push_back(0);
    
    //adding correct initial beans to hole
    for(int i = 1; i <= nHoles; i++)
    {
        north.push_back(nInitialBeansPerHole);
        south.push_back(nInitialBeansPerHole);
    }
}

int Board::holes() const
{
    return m_nholes;
}

int Board::beans(Side s, int hole) const
{
    if(hole > m_nholes || hole < 0)
    {
        return -1;
    }
    if(s == NORTH)
    {
        return(north[hole]);
    }
    else
    {
        return(south[hole]);
    }
}
//sum all beans in vectors north and south
int Board::beansInPlay(Side s) const
{
    if(s == NORTH)
    {
        int sum = 0;
        for(int i = 1; i <= m_nholes; i++)
        {
            sum += north[i];
        }
        return sum;
    }
    else
    {
        int sum = 0;
        for(int i = 1; i <= m_nholes; i++)
        {
            sum += south[i];
        }
        return sum;
    }
}

//adding all the beans in both the pot and the board, aka summing all values in the two vectors
int Board::totalBeans() const
{
    int sum = 0;
    for(int i = 0; i <= m_nholes; i++)
    {
        sum += north[i];
        sum += south[i];
    }
    return sum;
}



bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    //if try to sow a negative hole or try to sow the pot or if hole is greatern than existing holes, return false
    if(hole <= 0 || hole > m_nholes)
    {
        return false;
    }
    if(s == NORTH)
    {
        //can't sow a hole wihh no beans
        if(north[hole] == 0)
        {
            return false;
        }
        int beans = north[hole];
        north[hole] = 0;
        
        //following the counterclockwise of the board decrement each position and plus one bean to each hole while subtracting a bean from the total amount of beans from the orignial hole
        for(int i = hole-1; i >= 0 && beans > 0; i--)
        {
            north[i]++;
            beans--;
            if(beans == 0)
            {
                endSide = NORTH;
                endHole = i;
            }
        }
        //if the sowing continues into the south side the vector needs to increase instead of decreasing
        for(int i = 1; i <= m_nholes && beans > 0; i++)
        {
            south[i]++;
            beans--;
            if(beans == 0)
            {
                endSide = SOUTH;
                endHole = i;
            }
        }
        //while there are still more beans the game will keep on looping around dropping a bean per hole
        //same code as above
        while(beans > 0)
        {
            for(int i = m_nholes; i >= 0 && beans > 0; i--)
            {
                north[i]++;
                beans--;
                if(beans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                }
            }
            for(int i = 1; i <= m_nholes && beans > 0; i++)
            {
                south[i]++;
                beans--;
                if(beans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                }
            }
        }
    }
    else
    {
        if(south[hole] == 0)
        {
            return false;
        }
        int beans = south[hole];
        south[hole] = 0;
        for(int i = hole+1; i <= m_nholes && beans > 0; i++)
        {
            south[i]++;
            beans--;
            if(beans == 0)
            {
                endSide = SOUTH;
                endHole = i;
            }
        }
        //placing one bean into south's pot. have to do it specially cuz south's pot is at spot zero
        if(beans > 0)
        {
            south[0]++;
            beans--;
            if(beans == 0)
            {
                endSide = SOUTH;
                endHole = 0;
            }
        }
        for(int i = m_nholes; i > 0 && beans > 0; i--)
        {
            north[i]++;
            beans--;
            if(beans == 0)
            {
                endSide = NORTH;
                endHole = i;
            }
        }
        //while there are still more beans the game will keep on looping around dropping a bean per hole
        while(beans > 0)
        {
            for(int i = 1; i <= m_nholes && beans > 0; i++)
            {
                south[i]++;
                beans--;
                if(beans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                }
            }
            if(beans > 0)
            {
                south[0]++;
                beans--;
                if(beans == 0)
                {
                    endSide = SOUTH;
                    endHole = 0;
                }
            }
            for(int i = m_nholes; i > 0 && beans > 0; i--)
            {
                north[i]++;
                beans--;
                if(beans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                }
            }

        }

    }
    return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole <= 0 || hole > m_nholes)
    {
        return false;
    }
    if(potOwner == SOUTH)
    {
        if(s == NORTH)
        {
            //add everything in the north hole into the south pot then set the hole to 0
            south[0] += north[hole];
            north[hole] = 0;
        }
        else if(s == SOUTH)
        {

            //add everything in the south hole into the south pot
            south[0] += south[hole];
            south[hole] = 0;
        }
        return true;
    }
    else
    {
        if(s == NORTH)
        {
            if(north[hole] == 0)
            {
                return false;
            }
            //add everything in the north hole into the north pot and set the hole to 0
            north[0] += north[hole];
            north[hole] = 0;
        }
        else if(s == SOUTH)
        {
            if(south[hole] == 0)
            {
                return false;
            }
            //add everything in the south hole into north pot
            north[0] += south[hole];
            south[hole] = 0;
        }
        return true;
            
    }
    
}

bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole < 0 || hole > m_nholes)
    {
        return false;
    }
    if(s == NORTH)
    {
        north[hole] = beans;
        return true;
    }
    else
    {
        south[hole] = beans;
        return true;
    }
}
