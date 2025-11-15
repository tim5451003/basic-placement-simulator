#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <vector>
#include <string>

// Core data structures for the placement simulator

struct Cell {
    int id;
    int x, y;        // Current position
    int w, h;        // Width and height
    bool fixed;      // Whether cell is fixed (cannot be moved)
    
    Cell() : id(-1), x(0), y(0), w(1), h(1), fixed(false) {}
    Cell(int id, int x, int y, int w, int h, bool fixed = false)
        : id(id), x(x), y(y), w(w), h(h), fixed(fixed) {}
};

struct Pin {
    int cell_id;
    int offset_x, offset_y;  // Offset from cell's bottom-left corner
    
    Pin() : cell_id(-1), offset_x(0), offset_y(0) {}
    Pin(int cell_id, int offset_x, int offset_y)
        : cell_id(cell_id), offset_x(offset_x), offset_y(offset_y) {}
};

struct Net {
    int id;
    std::vector<Pin> pins;
    
    Net() : id(-1) {}
    Net(int id) : id(id) {}
};

struct Grid {
    int W, H;  // Width and height of the grid
    std::vector<std::vector<int>> occ;  // Occupation map (cell_id or -1 for empty)
    
    Grid() : W(0), H(0) {}
    Grid(int W, int H) : W(W), H(H) {
        occ.resize(H, std::vector<int>(W, -1));
    }
    
    bool isValid(int x, int y) const {
        return x >= 0 && x < W && y >= 0 && y < H;
    }
    
    bool isOccupied(int x, int y) const {
        if (!isValid(x, y)) return true;
        return occ[y][x] != -1;
    }
    
    void setOccupied(int x, int y, int cell_id) {
        if (isValid(x, y)) {
            occ[y][x] = cell_id;
        }
    }
    
    void clearOccupied(int x, int y) {
        if (isValid(x, y)) {
            occ[y][x] = -1;
        }
    }
};

struct Placement {
    std::vector<Cell> cells;
    std::vector<Net> nets;
    Grid grid;
    
    // Find cell by ID
    Cell* findCell(int id) {
        for (auto& cell : cells) {
            if (cell.id == id) return &cell;
        }
        return nullptr;
    }
    
    const Cell* findCell(int id) const {
        for (const auto& cell : cells) {
            if (cell.id == id) return &cell;
        }
        return nullptr;
    }
    
    // Update grid occupation based on current cell positions
    void updateGrid() {
        // Clear grid
        for (int y = 0; y < grid.H; ++y) {
            for (int x = 0; x < grid.W; ++x) {
                grid.occ[y][x] = -1;
            }
        }
        
        // Place cells
        for (const auto& cell : cells) {
            for (int dy = 0; dy < cell.h; ++dy) {
                for (int dx = 0; dx < cell.w; ++dx) {
                    int gx = cell.x + dx;
                    int gy = cell.y + dy;
                    if (grid.isValid(gx, gy)) {
                        grid.setOccupied(gx, gy, cell.id);
                    }
                }
            }
        }
    }
};

#endif // PLACEMENT_H

