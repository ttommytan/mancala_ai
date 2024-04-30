//
//  Game.cpp
//  Project3
//
//  Created by Tommy Tan on 5/24/23.
//

#include "Game.h"
#include <iostream>
#include <chrono>
using namespace std;
Game::Game(const Board& b, Player* south, Player* north): m_board(b)
{
    m_north = north;
    m_south = south;
}

int Game::beans(Side s, int hole) const
{
    return(m_board.beans(s, hole));
}

//fucntion to easily print out spaces
void Game::printSpaces(int n) const
{
    for(int i = 0; i < n; i ++)
    {
        cout << " ";
    }
}
void Game::display() const
{
    int pad = static_cast<int>(m_north->name().size()) + 10;
    printSpaces(pad);
    printSpaces(m_board.holes()-1);
    cout << m_north->name() << endl;
    printSpaces(pad);
    //space out the holes
    for(int i = 1; i <= m_board.holes(); i++)
    {
        cout << i;
        printSpaces(2);
    }
    cout << endl;
    printSpaces(pad);
    //printing out the seperator
    for(int i = 1; i <= 3*m_board.holes() - 1; i++)
    {
        cout << "-";
    }
    cout << endl;
    
    printSpaces(pad);
    for(int i = 1; i <= m_board.holes(); i++)
    {
        cout << m_board.beans(NORTH, i);
        printSpaces(2);
    }
    cout << endl;
    printSpaces(0);
    //spacing out the names
    cout << m_north->name() << "'s Pot " << m_board.beans(NORTH, 0); //printing out the beans in the pot// should it print out the names
    
    printSpaces(3*m_board.holes() + 3);
    cout << m_south->name() << "'s Pot " << m_board.beans(SOUTH, 0) << endl;
    printSpaces(pad);
    for(int i = 1; i <= m_board.holes(); i++)
    {
        cout << m_board.beans(SOUTH, i);
        printSpaces(2);
    }
    cout << endl;
    printSpaces(pad);
    for(int i = 1; i <= 3*m_board.holes() - 1; i++)
    {
        cout << "-";
    }
    cout << endl;
    printSpaces(pad);
    for(int i = 1; i <= m_board.holes(); i++)
    {
        cout << i;
        printSpaces(2);
    }
    cout << endl;
    printSpaces(pad);
    printSpaces(m_board.holes()-1);
    cout <<  m_south->name() << endl << endl;

}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    over = true;
    //if both sides still have beans in the holes the game is not over
    if(m_board.beansInPlay(NORTH) != 0 && m_board.beansInPlay(SOUTH) != 0)
    {
        over = false;
        return;
    }
    // the game is considered over at this point
    //if total beans in play and in pot in north is greater than south then north won
    if(m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) > m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH))
    {
        over = true;
        hasWinner = true;
        winner = NORTH;
    }
    //if total beans in play and in pot in north is less than south then south won
    else if(m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) < m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH))
    {
        over = true;
        hasWinner = true;
        winner = SOUTH;
    }
    //tie
    else if(m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH) == m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH))
    {
        over = true;
        hasWinner = false;
    }
    
}
//If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.

bool Game::move(Side s)
{
    bool over = false;
    bool hasWinner = false;
    Side winner;
//    status(over, hasWinner, winner);
//    if(over)
//    {
//        //sweeping everything into the pots
//        for(int i = 1; i <= m_board.holes(); i++)
//        {
//            m_board.moveToPot(NORTH, i, NORTH);
//            m_board.moveToPot(SOUTH, i, SOUTH);
//        }
//        return false;
//    }
    if(s == SOUTH)
    {
       // auto startTime = std::chrono::steady_clock::now();

        //get the hole number that the player chooses. will vary depending on the type of player
        int holeNum = m_south->chooseMove(m_board, SOUTH);

//        auto endTime = std::chrono::steady_clock::now();
//        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//        cout << m_south->name() << " took " << elapsedTime/1000 << " seconds" << endl;
        Side endSide;
        int endHole;
        
        //if player is a bot then prompt enter to see the output
        if(!m_south->isInteractive())
        {
            cout << m_south->name() << " chose hole number " << holeNum << endl;
//            cout << "Press enter to continue" << endl;
//            cin.clear();
//            cin.ignore(10000, '\n');
//            //if the other player is human then the above cin.ignore "consumes" the enter so another cin.ignore is required to achieve the promp effect
//            if(m_north->isInteractive())
//            {
//                cin.ignore(10000, '\n');
//            }

        }
        //now sow with the given hole num
        m_board.sow(SOUTH, holeNum, endSide, endHole);
        
        //if it fits the capture scenario, capture
        if(endSide == SOUTH && endHole != 0 && m_board.beans(SOUTH, endHole) == 1 && m_board.beans(NORTH, endHole) != 0)
        {
            m_board.moveToPot(NORTH, endHole, SOUTH);
            m_board.moveToPot(SOUTH, endHole, SOUTH);
            display();
            return true;
        }
        display();
        
        //while the last bean was placed in the player's pot, move again
        while(endHole == 0 && endSide == SOUTH)
        {
          //   startTime = std::chrono::steady_clock::now();

            if(m_board.beansInPlay(SOUTH) == 0)
            {
                //sweep
                for(int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(NORTH, i, NORTH);
                }
                break;
            }
            holeNum = m_south->chooseMove(m_board, SOUTH);
            
//             endTime = std::chrono::steady_clock::now();
           
//             elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//            cout << m_south->name() << " took " << elapsedTime/1000 << " seconds" << endl;
            
            
            if(!m_south->isInteractive())
            {
                cout << m_south->name() << " chose hole number " << holeNum << endl;
//                cout << "Press enter to continue" << endl;
//                cin.clear();
//                cin.ignore(10000, '\n');
                //because this is only prompted if a bot moves then we know for sure the revious move wasn't a human's
            }
//            
            //after finding the move sow, the hole
            m_board.sow(SOUTH, holeNum, endSide, endHole);
            //end game if beans in play is 0
            if(m_board.beansInPlay(SOUTH) == 0)
            {
                status(over, hasWinner, winner);
                if(over)//dont rlly need
                {
                    //sweeping everything into the pots
                    for(int i = 1; i <= m_board.holes(); i++)
                    {
                        m_board.moveToPot(NORTH, i, NORTH);
                    }
                    return false;
                }
            }
            display();
        }
        //capture scenario
        if(endSide == SOUTH && endHole != 0 && m_board.beans(SOUTH, endHole) == 1 && m_board.beans(NORTH, endHole) != 0)
        {
            m_board.moveToPot(NORTH, endHole, SOUTH);
            m_board.moveToPot(SOUTH, endHole, SOUTH);
            display();
            return true;
        }
    }
    else
    {
       // auto startTime = std::chrono::steady_clock::now();

        int holeNum = m_north->chooseMove(m_board, NORTH);

        // Stop the timer
//        auto endTime = std::chrono::steady_clock::now();
//
//        // Calculate the elapsed time in milliseconds
//        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//        cout << m_north->name() << " took " << elapsedTime/1000 << " seconds" << endl;
        Side endSide;
        int endHole;
        if(!m_north->isInteractive())
        {
            cout << m_north->name() << " chose hole number " << holeNum << endl;
//            cout << "Press enter to continue" << endl;

//            cin.ignore(10000, '\n');
//            if(m_south->isInteractive())
//            {
//                cin.ignore(10000, '\n');
//            }

        }
        m_board.sow(NORTH, holeNum, endSide, endHole); //sow should never have a bad value for the holenum cuz chooseMove only allows good moves so do i need to make an if statement for this?
        //if the last bean was placed in the player's pot, move again
        display();
        while(endHole == 0 && endSide == NORTH)
        {
            
             //startTime = std::chrono::steady_clock::now();
            
            if(m_board.beansInPlay(NORTH) == 0)
            {
                //sweep
                for(int i = 1; i <= m_board.holes(); i++)
                {
                    m_board.moveToPot(SOUTH, i, SOUTH);
                }
                break;
            }

            holeNum = m_north->chooseMove(m_board, NORTH); //do i need to make this say smthing else
            

//             endTime = std::chrono::steady_clock::now();
//             elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
            
            
           // cout << m_south->name() << " took " << elapsedTime/1000.0 << " seconds" << endl;
            // Check if the elapsed time exceeds five seconds
            if(!m_north->isInteractive())
            {
                cout << m_north->name() << " chose hole number " << holeNum << endl;
//                cout << "Press enter to continue" << endl;
//               // cin.clear();
//                cin.ignore(10000, '\n');
            }
            m_board.sow(NORTH, holeNum, endSide, endHole);
            //end game
            if(m_board.beansInPlay(NORTH) == 0)
            {
                status(over, hasWinner, winner);
                if(over)
                {
                    //sweeping everything into the pots
                    for(int i = 1; i <= m_board.holes(); i++)
                    {
                        m_board.moveToPot(SOUTH, i, SOUTH);
                    }
                    return false;
                }
            }
            display();
        }
        //capture scenario
        if(endSide == NORTH && endHole != 0 && m_board.beans(NORTH, endHole) == 1 && m_board.beans(SOUTH, endHole) != 0)
        {
            m_board.moveToPot(SOUTH, endHole, NORTH);
            m_board.moveToPot(NORTH, endHole, NORTH);
            display();
            return true;
        }
        

    }
    return true;
}


//

void Game::play()
{
    display();
    bool over = false;
    bool hasWinner = false;
    Side winner;
    
    while(!over)
    {
        if(m_board.beansInPlay(SOUTH) == 0)
        {
            status(over, hasWinner, winner);
            break;
        }
        move(SOUTH);
        
        if(!m_south->isInteractive())
        {
            cout << "Press enter to continue" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            //if the other player is human then the above cin.ignore "consumes" the enter so another cin.ignore is required to achieve the promp effect
            if(m_north->isInteractive())
            {
                cin.ignore(10000, '\n');
            }

        }
        
//        status(over, hasWinner, winner);
//        if(over)
//        {
//            break;
//        }
        if(m_board.beansInPlay(NORTH) == 0)
        {
            status(over, hasWinner, winner);
            break;
        }
        move(NORTH);
        if(!m_north->isInteractive())
        {
            cout << "Press enter to continue" << endl;

            cin.ignore(10000, '\n');
            if(m_south->isInteractive())
            {
                cin.ignore(10000, '\n');
            }

        }

//        status(over, hasWinner, winner);
//        if(over)
//        {
//            break;
//        }

        
    }
    
    if(over)
    {
        //sweeping everything into the pots again for good measure
        for(int i = 1; i <= m_board.holes(); i++)
        {
            m_board.moveToPot(NORTH, i, NORTH);
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        //return false;
    }
    if(hasWinner)
    {
        display();
        cout << "Game Over! Game won by ";
        if(winner == NORTH)
        {
            cout << m_north->name() << "!" << endl;
        }
        else
        {
            cout << m_south->name() << "!" << endl;
        }
    }
    else
    {
        display();
        cout << "There has been a tie!" << endl;
    }
    
}
//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
//int beans(Side s, int hole) const;
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
