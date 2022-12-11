#include <vector>
#include <unordered_map>
#include <iostream>

typedef struct {
    int x;
    int y;
    std::vector<int> corners;
} Board;

typedef struct {
    Board* board;
    unsigned long int combinations;
} Cell;

unsigned int hash_corner(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

unsigned long int hash(Board* board) {
    int hash = 0;
    for (const auto & i : board->corners)
        hash = 31 * hash + hash_corner(i);
    return hash;
}

bool equal_boards(Board* board1, Board* board2) {

    if (board1->x != board2->x || board1->y != board2->y)
        return false;

    for (int i = 0; i < board1->y; i++)
        if(board1->corners[i] != board2->corners[i])
            return false;

    return true;
}

int get_max_tile_size(Board* board) {
    std::vector<int> corners = board->corners;
    int tile_size = 0;

    for (int i = 0; i < board->y; i++) {
        if (board->y - i >= corners[i] && corners[i] > tile_size) {
            tile_size = corners[i];
            if (tile_size > 1)
                return tile_size;
        }
        else if (corners[i] >= board->y - i && board->y - i > tile_size) {
            tile_size = board->y - i;
            if (tile_size > 1)
                return tile_size;
        }
    }

    return tile_size;
}

void place_rightmost_tile(std::vector<Board*>& result, Board* board) {
    int max = 0;
    for (int i = 0; i < board->y; i++)
        if (board->corners[i] > board->corners[max])
            max = i;
    
    int tile_size = 0;
    if (board->y - max >= board->corners[max] && board->corners[max] > tile_size)
        tile_size = board->corners[max];
    if (board->corners[max] >= board->y - max && board->y - max > tile_size)
        tile_size = board->y - max;

    for (int i = 0; i < tile_size; i++) {
        if (board->corners[max + i] < board->corners[max]) {
            tile_size = i;
            break;
        }
    }

    if (tile_size == 0) return;

    do {
        std::vector<int> corners(board->corners);
        Board* new_board = new Board();
        new_board->x = board->x;
        new_board->y = board->y;
        new_board->corners = corners;
        
        for (int i = 0; i < tile_size; i++)
           new_board->corners[max+i] -= tile_size;
        
        result.push_back(new_board);

    } while (--tile_size > 0);
    
    
}

unsigned long int get_combinations(Board* board, std::unordered_map<unsigned long int, Cell>& table) {
    int size = get_max_tile_size(board);

    if (size < 2)
        return 1;
    
    unsigned long int board_id = hash(board);
    if(table.find(board_id) != table.end())
        if (equal_boards(board, table[board_id].board))
            return table[board_id].combinations;
    
    unsigned long int answer = 0;
    std::vector<Board*> children = std::vector<Board*>();
    place_rightmost_tile(children, board);
    
    for (auto &b : children)
        answer += get_combinations(b, table);

    Cell cell = {.board = board, .combinations = answer};
    table[board_id] = cell;

    return answer;
}

unsigned long int compute_board(Board* board, std::unordered_map<unsigned long int, Cell>& table) {
    int size = get_max_tile_size(board);

    if (size < 2)
        return size;
    
    return get_combinations(board, table);
}

Board* read_input() {
    Board* board = new Board();
    std::cin >> board->y >> board->x;

    int corner;
    for (int i = 0; i < board->y; i++) {
        std::cin >> corner;
        board->corners.push_back(corner);
    }
    return board;
}

int main() {
    // Create board and read input
    Board* board = read_input();
    
    std::unordered_map<unsigned long int, Cell> map;
    
    std::cout << compute_board(board, map) << std::endl;
    
    for (auto &e : map)
        delete e.second.board;

    return 0;
}