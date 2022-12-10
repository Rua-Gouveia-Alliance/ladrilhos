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

void get_parents(std::vector<Board> &parents, Board &board) {
    if (board.corners[0] > 0) {
        std::vector<int> corners(board.corners);
        Board parent = {.x = board.x, .y = board.y, .corners = corners};
        parent.corners[0]--;
        parents.push_back(parent);
    }
    for (int i = 1; i < board.y; i++) {
        if (board.corners[i] > 0 && board.corners[i-1] < board.corners[i]) {
            std::vector<int> corners(board.corners);
            Board parent = {.x = board.x, .y = board.y, .corners = corners};
            parent.corners[i]--;
            parents.push_back(parent);
        }
    }
}

void get_common_board(Board &board, std::vector<Board> &boards) {
    int min, count = boards.size();
    board = {.x = board.x, .y = board.y};
    
    for (int i = 0; i < board.y; i++) {
        min = boards[0].corners[i];
        for (int j = 0; j < count; j++)
            if(boards[j].corners[i] < min)
                min = boards[j].corners[i];
        board.corners.push_back(min);
    }
}

unsigned long int get_combinations(Board &board) {
    int tile_size = get_max_tile_size(board);

    if (tile_size == 0)
        return 0;
    if (tile_size == 1)
        return 1;

    int answer = 0;
    std::vector<Board> parents;
    get_parents(parents, board);
    for (auto & p : parents)
        answer += get_combinations(p);
    
    if (parents.size() > 1) {
        Board common_board = Board();
        get_common_board(common_board, parents);
        answer -= get_combinations(common_board);
    }

    return answer;
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
    
    std::vector<Board> vector;
    get_parents(vector, board);
/*
    for (const auto & p : vector)
        print_board(p);
    */

    std::cout << get_combinations(board) << std::endl;

    return 0;
}