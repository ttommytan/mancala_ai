//
//  main.cpp
//  Project3
//
//  Created by Tommy Tan on 5/23/23.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "Board.h"
#include "Game.h"

using namespace std;

int main()
{
    Board test(3, 4);
    Player * a = new HumanPlayer("Tommy");
    Player * b = new SmartPlayer("Bot");
      test.setBeans(SOUTH, 1, 0);
      test.setBeans(SOUTH, 2, 1);
      test.setBeans(SOUTH, 3, 0);
      test.setBeans(NORTH, 1, 1);
      test.setBeans(NORTH, 2, 0);
      test.setBeans(NORTH, 3, 0);

    Game lame(test, a, b);

    lame.play();

    delete a;
    delete b;
}
