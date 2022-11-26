#include <iostream>
#include "structs.h"

#define LOG(x) std::cout << x << std::endl

using namespace std;

int getBiggestSquareSize(Board board)
{
    int size = board.corners.size();
    int squareSize = 0;

    for (int i = 0; i < size; i++)
        if (board.y - i >= board.corners[i])
            if (board.corners[i] > squareSize)
                squareSize = board.corners[i];

    return squareSize;
}

vector<Square> *getPossibleSquares(int size, Board board)
{
    vector<Square> *possibleSquares = new vector<Square>;
    if (size > board.y || size > board.x)
        return possibleSquares;

    for (int y = 0; board.y - y >= size; y++)
    {
        for (int x = 0; board.corners[y] - x >= size; x++)
        {
            Square square = {.x = x, .y = y, .size = size};
            (*possibleSquares).push_back(square);
        }
    }

    return possibleSquares;
}

Board getBottomBoard(Square square, Board board)
{
    Board newboard;
    return board;
}

/*vector<Board> splitBoard(int square, Board board)
{
}*/

int getPossibleConfigurations(Board board)
{
    int biggestSquare = getBiggestSquareSize(board);
    if (biggestSquare == 1)
        return 1;
    if (biggestSquare == 2)
        return 1 + (*getPossibleSquares(biggestSquare, board)).size();
    return 0;
}

void printSquare(Square s) {
    cout << "X: " << s.x << ", " << "Y: " << s.y << ", " << "Size: " << s.size << endl;
}

int main()
{
    Board board;
    board.y = 4;
    board.x = 5;
    vector<int> corners = {0, 2, 3, 5};
    board.corners = corners;

    vector<Square>* vector = getPossibleSquares(2, board);
    for (const auto& s : (*vector))
        printSquare(s);
    

    return 0;
}
