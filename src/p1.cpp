#include <vector>
#include <iostream>
#include <list>

typedef struct {
    int x;
    int y;
    std::vector<int> corners;
} Board;

int get_max_tile_size(Board &board) {
    std::vector<int> corners = board.corners;
    int tile_size = 0;

    for (int i = 0; i < board.y; i++) {
        if (board.y - i >= corners[i] && corners[i] > tile_size) {
            tile_size = corners[i];
            if (tile_size > 1)
                return tile_size;
        }
        else if (corners[i] >= board.y - i && board.y - i > tile_size) {
            tile_size = board.y - i;
            if (tile_size > 1)
                return tile_size;
        }
    }

    return tile_size;
}

void place_rightmost_tile(std::vector<Board>& result, Board& board) {
    int max = 0;
    result.clear();
    for (int i = 0; i < board.y; i++)
        if (board.corners[i] > board.corners[max])
            max = i;
    
    int tile_size = 0;
    if (board.y - max >= board.corners[max] && board.corners[max] > tile_size)
        tile_size = board.corners[max];
    if (board.corners[max] >= board.y - max && board.y - max > tile_size)
        tile_size = board.y - max;

    for (int i = 0; i < tile_size; i++) {
        if (board.corners[max + i] < board.corners[max]) {
            tile_size = i;
            break;
        }
    }

    if (tile_size == 0) return;

    do {
        std::vector<int> corners(board.corners);
        Board new_board = {.x = board.x, .y = board.y, .corners = corners};
        
        for (int i = 0; i < tile_size; i++)
           new_board.corners[max+i] -= tile_size;
        
        result.push_back(new_board);

    } while (--tile_size > 0);
    
    
}

unsigned long int get_combinations(Board &board) {
    int size = get_max_tile_size(board);

    if (size < 2)
        return 1;
    
    unsigned long int answer = 0;
    std::vector<Board> children = std::vector<Board>();
    place_rightmost_tile(children, board);
    
    for (auto &b : children)
        answer += get_combinations(b);

    return answer;
}

unsigned long int compute_board(Board &board) {
    int size = get_max_tile_size(board);

    if (size < 2)
        return size;
    
    return get_combinations(board);
}

void print_board(Board board) {
    std::cout << "X:" << board.x << " Y: " << board.y << std::endl;
    for (const auto & c : board.corners)
        std::cout << c << std::endl;
}

void read_input(Board &board) {
    std::cin >> board.y >> board.x;

    int corner;
    for (int i = 0; i < board.y; i++) {
        std::cin >> corner;
        board.corners.push_back(corner);
    }
}

int main() {
    // Create board and read input
    Board board;
    read_input(board);
    
    std::cout << compute_board(board) << std::endl;

    return 0;
}