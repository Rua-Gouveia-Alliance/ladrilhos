#include "structs.h"
#include <iostream>
#include <list>

#define LOG(x) std::cout << x << std::endl

using namespace std;

void printTile(Tile t) {
    cout << "X: " << t.x << ", Y: " << t.y << ", Size: " << t.size << endl;
}

void printBoard(Board b) {
    cout << "X: " << b.x << ", Y: " << b.y << endl << "Corners:" << endl;
    vector<int> corners = *(b.corners);
    int size = corners.size();
    for (int i = 0; i < size; i++)
        cout << "X: " << corners[i] << ", Y:" << i << endl;
}

bool overlap(Tile t1, Tile t2) {
    if (t1.x >= t2.x + t2.size)
        return false;
    if (t1.y >= t2.y + t2.size)
        return false;
    if (t1.x + t1.size <= t2.x)
        return false;
    if (t1.y + t1.size <= t2.y)
        return false;
    return true;
}

int getMaxTileSize(Board board) {
    vector<int> corners = *(board.corners);

    int tile_size = 0;
    for (int i = 0; i < board.y; i++)
        if (board.y - i >= corners[i] && corners[i] > tile_size)
            tile_size = corners[i];
        else if (corners[i] >= board.y - i && board.y - i > tile_size)
            tile_size = board.y - i;

    return tile_size;
}

vector<Tile> *getPossibleTiles(int size, Board board) {
    vector<Tile> *possible_squares = new vector<Tile>;
    vector<int> corners = *(board.corners);

    if (size > board.y || size > board.x || size == 0)
        return possible_squares;

    for (int y = 0; board.y - y >= size; y++)
        for (int x = 0; corners[y] - x >= size; x++) {
            Tile square = {.x = x, .y = y, .size = size};
            possible_squares->push_back(square);
        }

    return possible_squares;
}

Board getBottomBoard(Tile tile, Board board) {
    int x = tile.x + tile.size;
    int y = board.y - tile.y;
    int corner_x = tile.x;
    vector<int> *corners = new vector<int>;

    for (int i = 0; i <= y; i++)
        corners->push_back(i < tile.size ? corner_x : x);

    Board bottom_board = {.x = x, .y = y, .corners = corners};
    return bottom_board;
}

Board getTopBoard(Tile tile, Board board) {
    int x = tile.x + tile.size;
    int y = tile.y;
    vector<int> corners = *(board.corners);
    vector<int> *top_corners = new vector<int>;

    for (int i = 0; i <= y; i++)
        top_corners->push_back(corners[i] > x ? x : corners[i]);

    Board top_board = {.x = x, .y = y, .corners = top_corners};
    return top_board;
}

Board getSideBoard(Tile tile, Board board) {
    int offset = tile.size + tile.x;
    int x = board.x - offset;
    int y = board.y;
    vector<int> corners = *(board.corners);
    vector<int> *side_corners = new vector<int>;
    int corner;

    for (int i = 0; i <= y; i++) {
        corner = corners[i] - offset;
        side_corners->push_back(corner < 0 ? 0 : corner);
    }

    Board side_board = {.x = x, .y = y, .corners = side_corners};
    return side_board;
}

vector<Tile> *removeDoubledCases(vector<Tile> &tiles) {
    vector<Tile> *removed_tiles = new vector<Tile>;
    int size = tiles.size();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (!overlap(tiles[i], tiles[j])) {
                removed_tiles->push_back(tiles[j]);
                tiles.erase(tiles.begin() + j);
                size--;
            }
    return removed_tiles;
}

int cappedSizeCombinations(int tile_size, Board board) {
    int result = 1;

    if (tile_size == 1 || tile_size == 0)
        return result;

    do {
        vector<Tile> tiles = *getPossibleTiles(tile_size, board);
        vector<Tile> removed_tiles = *removeDoubledCases(tiles);

        // for the not removed tiles
        for (Tile &tile : tiles)
            result +=
                (cappedSizeCombinations(tile_size,
                                        getBottomBoard(tile, board)) *
                 cappedSizeCombinations(tile_size, getTopBoard(tile, board)) *
                 cappedSizeCombinations(tile_size, getSideBoard(tile, board)));

        // for the removed tiles
        for (Tile &tile : removed_tiles)
            result += (cappedSizeCombinations(tile_size - 1,
                                              getBottomBoard(tile, board)) *
                       cappedSizeCombinations(tile_size - 1,
                                              getTopBoard(tile, board)) *
                       cappedSizeCombinations(tile_size - 1,
                                              getSideBoard(tile, board)));
    } while (--tile_size > 1);

    return result;
}

int getCombinations(Board board) {
    int tile_size = getMaxTileSize(board);

    if (tile_size == 1 || tile_size == 0)
        return tile_size;
    return cappedSizeCombinations(tile_size, board);
}

int main() {
    vector<int> corners = {4, 4, 4};
    Board board = {.x = 4, .y = 3, .corners = &corners};

    LOG(getCombinations(board));

    corners = {0, 2, 3, 5, 5};
    board = {.x = 5, .y = 4, .corners = &corners};
    LOG(getCombinations(board));

    return 0;
}
