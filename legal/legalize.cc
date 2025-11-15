#include "legalize.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void Legalizer::clearCellFromGrid(Placement& pl, int cell_id) {
    for (int y = 0; y < pl.grid.H; ++y) {
        for (int x = 0; x < pl.grid.W; ++x) {
            if (pl.grid.occ[y][x] == cell_id) {
                pl.grid.occ[y][x] = -1;
            }
        }
    }
}

bool Legalizer::canPlace(const Placement& pl, const Cell& cell, int x, int y) {
    // Check bounds
    if (x < 0 || y < 0 || x + cell.w > pl.grid.W || y + cell.h > pl.grid.H) {
        return false;
    }
    
    // Check for overlaps (excluding the cell itself)
    for (int dy = 0; dy < cell.h; ++dy) {
        for (int dx = 0; dx < cell.w; ++dx) {
            int gx = x + dx;
            int gy = y + dy;
            
            if (pl.grid.isOccupied(gx, gy)) {
                int occupied_id = pl.grid.occ[gy][gx];
                if (occupied_id != cell.id && occupied_id != -1) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool Legalizer::findFreePosition(const Placement& pl, const Cell& cell, int& new_x, int& new_y) {
    // Try current position first
    if (canPlace(pl, cell, cell.x, cell.y)) {
        new_x = cell.x;
        new_y = cell.y;
        return true;
    }
    
    // Search in expanding spiral from current position
    int max_radius = std::max(pl.grid.W, pl.grid.H);
    
    for (int radius = 1; radius < max_radius; ++radius) {
        // Try positions in a square around current position
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                // Only check positions on the perimeter of the square
                if (abs(dx) != radius && abs(dy) != radius) continue;
                
                int x = cell.x + dx;
                int y = cell.y + dy;
                
                if (canPlace(pl, cell, x, y)) {
                    new_x = x;
                    new_y = y;
                    return true;
                }
            }
        }
    }
    
    return false;
}

void Legalizer::legalize(Placement& pl) {
    std::cout << "Legalizing placement..." << std::endl;
    
    // Sort cells by area (larger cells first) for better legalization
    std::vector<Cell*> cell_ptrs;
    for (auto& cell : pl.cells) {
        if (!cell.fixed) {
            cell_ptrs.push_back(&cell);
        }
    }
    
    std::sort(cell_ptrs.begin(), cell_ptrs.end(),
              [](const Cell* a, const Cell* b) {
                  return (a->w * a->h) > (b->w * b->h);
              });
    
    // Clear grid
    pl.updateGrid();
    
    // Legalize each cell
    int legalized = 0;
    for (Cell* cell : cell_ptrs) {
        // Temporarily remove cell from grid
        clearCellFromGrid(pl, cell->id);
        
        int new_x, new_y;
        if (findFreePosition(pl, *cell, new_x, new_y)) {
            cell->x = new_x;
            cell->y = new_y;
            legalized++;
        } else {
            std::cerr << "Warning: Could not legalize cell " << cell->id << std::endl;
        }
        
        // Update grid with new position
        for (int dy = 0; dy < cell->h; ++dy) {
            for (int dx = 0; dx < cell->w; ++dx) {
                int gx = cell->x + dx;
                int gy = cell->y + dy;
                if (pl.grid.isValid(gx, gy)) {
                    pl.grid.setOccupied(gx, gy, cell->id);
                }
            }
        }
    }
    
    std::cout << "Legalized " << legalized << " cells" << std::endl;
}

