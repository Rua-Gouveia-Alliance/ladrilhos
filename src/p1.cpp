#include <vector>
#include <iostream>
#include <list>

using namespace std;

typedef struct {
    int x;
    int y;
    vector<int> corners;
} Board;

typedef struct Tile {
    int x;
    int y;
    int size;
    vector<struct Tile> conflicts;
} Tile;

bool overlap(Tile &t1, Tile &t2) {
    if (t1.x > t2.x + t2.size - 1 || t2.x > t1.x + t1.size - 1)
        return false;

    if (t1.y + t1.size - 1 < t2.y || t2.y + t2.size - 1 < t1.y)
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

void getPossibleTiles(vector<Tile> &tiles, int size, Board &board, vector<Tile> &conflicts) {
    vector<int> corners = board.corners;
    bool conflict;

    if (size > board.y || size > board.x || size <= 0)
        return;

    for (int y = 0; board.y - y >= size; y++)
        for (int x = 0; corners[y] - x >= size; x++) {
            Tile tile = {.x = x, .y = y, .size = size};
            conflict = false; 
            for (Tile t : conflicts) {
                if (t.x == tile.x && t.y == tile.y && t.size == tile.size) {
                    conflict = true;
                    break;
                }
            }
            if (!conflict) {
                tiles.push_back(tile);
            }
    }
}

void getBottomBoard(Board &result, Tile &tile, Board &board) {
    result.x = tile.x + tile.size;
    result.y = board.y - tile.y;

    if (result.x < 2 || result.y < 2)
        return;

    for (int i = 0; i <= result.y; i++)
        result.corners.push_back(i < tile.size ? tile.x : result.x);

    for (auto &t : tile.conflicts)
        t.y -= tile.y;
}

void getTopBoard(Board &result, Tile &tile, Board &board) {
    result.x = tile.x + tile.size;
    result.y = tile.y;

    if (result.x < 2 || result.y < 2)
        return;

    for (int i = 0; i <= result.y; i++)
        result.corners.push_back(
            board.corners[i] > result.x ? result.x : board.corners[i]);

}

void getSideBoard(Board &result, Tile &tile, Board &board) {
    int offset = tile.size + tile.x;
    result.x = board.x - offset;
    result.y = board.y;

    if (result.x < 2 || result.y < 2)
        return;

    int corner;
    for (int i = 0; i <= result.y; i++) {
        corner = board.corners[i] - offset;
        result.corners.push_back(corner < 0 ? 0 : corner);
    }

    for (auto &t : tile.conflicts)
        t.x -= offset;
}

bool notIncludedOtherBoards(Tile &tile1, Tile &tile2) {
    return (tile2.x < tile1.x + tile1.size && tile2.x + tile2.size > tile1.x + tile1.size) ||
        (tile2.y < tile1.y && tile2.y + tile2.size > tile1.y && tile2.x < tile1.x);
}

void removeDoubledCases(vector<Tile> &tiles) {
    int size = tiles.size();
    for (int i = 0; i < size; i++) {
        tiles[i].conflicts = vector<Tile>();
        for (int j = i+1; j < size; j++)
            if (!overlap(tiles[i], tiles[j]) && !notIncludedOtherBoards(tiles[i], tiles[j]) && !notIncludedOtherBoards(tiles[j], tiles[i])) {
                tiles[i].conflicts.push_back(tiles[j]);
            }
    }
}

unsigned long cappedSizeCombinations(int tile_size, Board &board, vector<Tile> &conflicts) {
    unsigned long result = 1;

    if (tile_size < 2 || board.x < 2 || board.y < 2)
        return result;

    vector<Tile> tiles;
    Board bottom_board;
    Board top_board;
    Board side_board;
    vector<Tile> cache;
    unsigned long num_cache;
    do {
        tiles = vector<Tile>();
        getPossibleTiles(tiles, tile_size, board, conflicts);
        removeDoubledCases(tiles);

        // normal tiles
        for (Tile &tile : tiles) {
            bottom_board = Board();
            top_board = Board();
            side_board = Board();

            cache = tile.conflicts;

            getBottomBoard(bottom_board, tile, board);
            num_cache = cappedSizeCombinations(tile_size, bottom_board, tile.conflicts);

            tile.conflicts = cache;
            getTopBoard(top_board, tile, board);
            num_cache *= cappedSizeCombinations(tile_size, top_board, tile.conflicts);

            tile.conflicts = cache;
            getSideBoard(side_board, tile, board);
            num_cache *= cappedSizeCombinations(tile_size, side_board, tile.conflicts);

            result += num_cache;
        }
    } while (--tile_size > 1);

    return result;
}

unsigned long getCombinations(Board &board) {
    int tile_size = getMaxTileSize(board);
    if (!tile_size)
        return 0;

    vector<Tile> conflicts;
    return cappedSizeCombinations(tile_size, board, conflicts);
}

void readInput(Board &board) {
    cin >> board.y >> board.x;

    int corner;
    for (int i = 0; i < board.y; i++) {
        cin >> corner;
        board.corners.push_back(corner);
    }
}

int main() {
    // Create board and read input
    Board board;
    readInput(board);

    cout << getCombinations(board) << endl;

    return 0;
}
