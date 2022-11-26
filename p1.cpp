#include <iostream>
#include "structs.h"

#define LOG(x) std::cout << x << std::endl

using namespace std;

int getBiggestSquare(Board board)
{
    int size = board.corners.size();
    int squareSize = 0;

    for (int i = 0; i < size; i++)
        if (board.y - i >= board.corners[i])
            if (board.corners[i] > squareSize)
                squareSize = board.corners[i];

    return squareSize;
}

int getPossibleSquarePositions(int side, Board board)
{
    if (side > board.y || side > board.x)
        return 0;

    int possiblePositions = 0;
    for (int y = 0; board.y - y >= side; y++)
        for (int x = 0; board.corners[y] - x >= side; x++)
            possiblePositions++;

    return possiblePositions;
}

int main()
{
    Board board;
    board.y = 4;
    board.x = 5;
    vector<int> corners = {0, 2, 3, 5};
    board.corners = corners;

    LOG(getPossibleSquarePositions(2, board));

    return 0;
}
