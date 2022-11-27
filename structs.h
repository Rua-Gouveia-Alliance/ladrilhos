#include <vector>

typedef struct {
    int x;
    int y;
    std::vector<int> corners;
} Board;

typedef struct {
    int x;
    int y;
    int size;
} Tile;
