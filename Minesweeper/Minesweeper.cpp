// Minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <print>
#include "game.h"
using namespace Minesweeper;

int main()
{
    Board board = Board(10, 10);
    board.printBoard();
}
