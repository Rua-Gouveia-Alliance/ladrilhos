#include <vector>
#include <unordered_map>
#include <iostream>

typedef struct {
    int y;
    int x;
    std::vector<int> corners;
} Board;

typedef struct {
    Board* board;
    unsigned long int combinations;
} Cell;

unsigned int hash_number(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

long int hash(Board* board) {
    long int hash = 0;
    for (const auto & i : board->corners)
        hash = (17 * hash + hash_number(i));
    return hash;
}

bool equal_boards(Board* board1, Board* board2) {

    if (board1->y != board2->y || board1->x != board2->x)
        return false;

    for (int i = 0; i < board1->y; i++)
        if(board1->corners[i] != board2->corners[i])
            return false;

    return true;
}

bool null_board(Board* board) {
    for (int i = 0; i < board->y; i++)
        if (board->corners[i] != 0)
            return false;
    return true;
}

bool finished_board(Board* board) {
    for (int i = 0; i < board->y - 1; i++)
        if (board->corners[i] > 1)
            return false;
    return true;
}

void place_rightmost_tile(std::vector<Board*>& result, Board* board) {
    int max = 0;
    for (int i = 0; i < board->y; i++) {
        if (board->corners[i] > board->corners[max])
            max = i;
        if (board->corners[max] == board->x)
            break;
    }
    
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

    while(tile_size > 0) {
        std::vector<int> corners(board->corners);
        Board* new_board = new Board();
        new_board->y = board->y;
        new_board->x = board->x;
        new_board->corners = corners;
        
        for (int i = 0; i < tile_size; i++)
           new_board->corners[max+i] -= tile_size;
        
        result.push_back(new_board);
        tile_size--;
    }
}

void insert_board(long int board_id, unsigned long int answer, Board* board, std::unordered_map<long int, Cell>& table) {
    Cell cell = {.board = board, .combinations = answer};
    while (table.find(board_id) != table.end()) 
        board_id = hash_number(board_id);
    table[board_id] = cell;
}

long int lookup_board(long int board_id, Board* board, std::unordered_map<long int, Cell>& table) {
    long int answer = -1;
    while (table.find(board_id) != table.end()) { 
        if (equal_boards(table[board_id].board, board)) {
            answer = board_id;
            break;
        }
        board_id = hash_number(board_id);
    }
    return answer;
}

unsigned long int get_combinations(Board* board, std::unordered_map<long int, Cell>& table) {

    if (finished_board(board)) {
        delete board;
        return 1;
    }
    
    long int board_id = hash(board);
    long int lookup = lookup_board(board_id, board, table);
    if(lookup != -1) {
        delete board;
        return table[lookup].combinations;
    }
    
    unsigned long int answer = 0;
    std::vector<Board*> children = std::vector<Board*>();
    place_rightmost_tile(children, board);
    
    for (auto &b : children)
        answer += get_combinations(b, table);

    insert_board(board_id, answer, board, table);

    return answer;
}

void spin_board(Board* board) {
    int j;
    std::vector<int> corners;

    for (j = 0; j < board->corners[0]; j++) 
        corners.push_back(board->y);
    
    for (int i = 1; i < board->y; i++) {
        if (board->corners[i] == board->x) {
            for (; j < board->x; j++) 
                corners.push_back(board->y - i);
            break;
        } else {
            while (i < board->y && board->corners[i] == board->corners[i-1])
                i++;
            for (; j < board->corners[i]; j++)
                corners.push_back(board->y-i);
        }
    }
    
    for (;j < board->x; j++)
        corners.push_back(0);


    int temp = board->x;
    board->x = board->y;
    board->y = temp;    
    board->corners = corners;
}

unsigned long int compute_board(Board* board, std::unordered_map<long int, Cell>& table) {

    if (null_board(board)) {
        delete board;
        return 0;
    }
    
    if (board->x < 0.75 * board->y)
        spin_board(board);

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
    
    std::unordered_map<long int, Cell> map;

    std::cout << compute_board(board, map) << std::endl;
    
    for (auto &e : map)
        delete e.second.board;

    return 0;
}
