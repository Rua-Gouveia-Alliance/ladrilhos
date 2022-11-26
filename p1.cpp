#include <iostream>
#include "structs.h"

#define LOG(x) std::cout << x << std::endl

using namespace std;

int getBiggestSquare(Board board)
{
    int size = board.corners.size();
    int squareSize = 0;

    for (int i = 0; i < size; i++)
        if (board.n-i >= board.corners[i])
            if (board.corners[i] > squareSize)
                squareSize = board.corners[i];

    return squareSize;
}

int main()
{
    Board board;
    board.n = 4;
    board.m = 5;
    vector<int> corners = {0, 2, 3, 5};
    board.corners = corners;

    LOG(getBiggestSquare(board));

    return 0;
}
