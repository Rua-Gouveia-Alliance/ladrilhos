#include "structs.h"
#include <iostream>
#include <list>

#define LOG(x) std::cout << x << std::endl

using namespace std;

void printTile(Tile &t) {
    cout << "X: " << t.x << ", Y: " << t.y << ", Size: " << t.size << endl;
}

void printBoard(Board &b) {
    cout << "X: " << b.x << ", Y: " << b.y << endl << "Corners:" << endl;
    vector<int> corners = b.corners;
    int size = corners.size();
    for (int i = 0; i < size; i++)
        cout << "X: " << corners[i] << ", Y:" << i << endl;
}

bool overlap(Tile &t1, Tile &t2) {
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

int getMaxTileSize(Board &board) {
    vector<int> corners = board.corners;

    int tile_size = 0;
    for (int i = 0; i < board.y; i++)
        if (board.y - i >= corners[i] && corners[i] > tile_size)
            tile_size = corners[i];
        else if (corners[i] >= board.y - i && board.y - i > tile_size)
            tile_size = board.y - i;

    return tile_size;
}

void getPossibleTiles(vector<Tile> &tiles, int size, Board &board) {
    vector<int> corners = board.corners;

    if (size > board.y || size > board.x || size == 0)
        return;

    for (int y = 0; board.y - y >= size; y++)
        for (int x = 0; corners[y] - x >= size; x++) {
            Tile square = {.x = x, .y = y, .size = size};
            tiles.push_back(square);
        }
}

void getBottomBoard(Board &result, Tile &tile, Board &board) {
    result.x = tile.x + tile.size;
    result.y = board.y - tile.y;
    int corner_x = tile.x;

    for (int i = 0; i <= result.y; i++)
        result.corners.push_back(i < tile.size ? corner_x : result.x);
}

void getTopBoard(Board &result, Tile &tile, Board &board) {
    result.x = tile.x + tile.size;
    result.y = tile.y;

    for (int i = 0; i <= result.y; i++)
        result.corners.push_back(
            board.corners[i] > result.x ? result.x : board.corners[i]);
}

void getSideBoard(Board &result, Tile &tile, Board &board) {
    int offset = tile.size + tile.x;
    result.x = board.x - offset;
    result.y = board.y;

    int corner;
    for (int i = 0; i <= result.y; i++) {
        corner = board.corners[i] - offset;
        result.corners.push_back(corner < 0 ? 0 : corner);
    }
}

void removeDoubledCases(vector<Tile> &removed_tiles, vector<Tile> &tiles) {
    int size = tiles.size();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (!overlap(tiles[i], tiles[j])) {
                removed_tiles.push_back(tiles[j]);
                tiles.erase(tiles.begin() + j);
                size--;
            }
}

int cappedSizeCombinations(int tile_size, Board &board) {
    int result = 1;

    if (tile_size == 1 || tile_size == 0)
        return result;

    vector<Tile> tiles;
    vector<Tile> removed_tiles;
    Board bottom_board;
    Board top_board;
    Board side_board;
    do {
        getPossibleTiles(tiles, tile_size, board);
        removeDoubledCases(removed_tiles, tiles);

        // for the not removed tiles
        for (Tile &tile : tiles) {
            bottom_board = Board();
            top_board = Board();
            side_board = Board();
            getBottomBoard(bottom_board, tile, board);
            getTopBoard(top_board, tile, board);
            getSideBoard(side_board, tile, board);
            result += (cappedSizeCombinations(tile_size, bottom_board) *
                       cappedSizeCombinations(tile_size, top_board) *
                       cappedSizeCombinations(tile_size, side_board));
        }

        // for the removed tiles
        for (Tile &tile : removed_tiles) {
            bottom_board = Board();
            top_board = Board();
            side_board = Board();
            getBottomBoard(bottom_board, tile, board);
            getTopBoard(top_board, tile, board);
            getSideBoard(side_board, tile, board);
            result += (cappedSizeCombinations(tile_size - 1, bottom_board) *
                       cappedSizeCombinations(tile_size - 1, top_board) *
                       cappedSizeCombinations(tile_size - 1, side_board));
        }

        tiles = vector<Tile>();
        removed_tiles = vector<Tile>();
        bottom_board = Board();
        top_board = Board();
        side_board = Board();
    } while (--tile_size > 1);

    return result;
}

int getCombinations(Board &board) {
    int tile_size = getMaxTileSize(board);

    if (tile_size == 1 || tile_size == 0)
        return tile_size;
    return cappedSizeCombinations(tile_size, board);
}

int main() {
    Board board = {.x = 4, .y = 3, .corners = {4, 4, 4}};

    LOG(getCombinations(board));

    board = {.x = 5, .y = 4, .corners = {0, 2, 3, 5, 5}};
    LOG(getCombinations(board));

    return 0;
}
