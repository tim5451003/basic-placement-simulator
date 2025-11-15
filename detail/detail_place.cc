#include "detail_place.h"
#include "../cost/cost.h"
#include <algorithm>
#include <random>
#include <iostream>

bool DetailedPlacer::tryLocalMove(Placement& pl, Cell& cell, int window_size) {
    if (cell.fixed) return false;
    
    int old_x = cell.x;
    int old_y = cell.y;
    
    double old_cost = CostCalculator::calculateTotalCost(pl);
    
    // Try small moves within window
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(-window_size, window_size);
    
    int new_x = old_x + dist(rng);
    int new_y = old_y + dist(rng);
    
    // Ensure bounds
    new_x = std::max(0, std::min(new_x, pl.grid.W - cell.w));
    new_y = std::max(0, std::min(new_y, pl.grid.H - cell.h));
    
    // Check if position is valid
    bool can_place = true;
    for (int dy = 0; dy < cell.h && can_place; ++dy) {
        for (int dx = 0; dx < cell.w && can_place; ++dx) {
            int gx = new_x + dx;
            int gy = new_y + dy;
            if (pl.grid.isOccupied(gx, gy) && pl.grid.occ[gy][gx] != cell.id) {
                can_place = false;
            }
        }
    }
    
    if (!can_place) return false;
    
    // Temporarily move cell
    cell.x = new_x;
    cell.y = new_y;
    pl.updateGrid();
    
    double new_cost = CostCalculator::calculateTotalCost(pl);
    
    // Accept if better
    if (new_cost < old_cost) {
        return true;
    } else {
        // Revert
        cell.x = old_x;
        cell.y = old_y;
        pl.updateGrid();
        return false;
    }
}

void DetailedPlacer::optimizeWindow(Placement& pl, int center_x, int center_y, int window_size) {
    // Find cells in window
    std::vector<Cell*> cells_in_window;
    
    for (auto& cell : pl.cells) {
        if (cell.fixed) continue;
        
        int cell_center_x = cell.x + cell.w / 2;
        int cell_center_y = cell.y + cell.h / 2;
        
        if (std::abs(cell_center_x - center_x) <= window_size &&
            std::abs(cell_center_y - center_y) <= window_size) {
            cells_in_window.push_back(&cell);
        }
    }
    
    // Try local moves for cells in window
    for (Cell* cell : cells_in_window) {
        tryLocalMove(pl, *cell, window_size / 2);
    }
}

void DetailedPlacer::detailedPlace(Placement& pl, int window_size, int max_iterations) {
    std::cout << "Performing detailed placement..." << std::endl;
    
    double initial_cost = CostCalculator::calculateTotalCost(pl);
    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Divide grid into windows and optimize each
        int num_windows_x = (pl.grid.W + window_size - 1) / window_size;
        int num_windows_y = (pl.grid.H + window_size - 1) / window_size;
        
        for (int wy = 0; wy < num_windows_y; ++wy) {
            for (int wx = 0; wx < num_windows_x; ++wx) {
                int center_x = (wx + 0.5) * window_size;
                int center_y = (wy + 0.5) * window_size;
                
                optimizeWindow(pl, center_x, center_y, window_size);
            }
        }
        
        double current_cost = CostCalculator::calculateTotalCost(pl);
        
        if (iter % 5 == 0 || iter == max_iterations - 1) {
            std::cout << "  Iteration " << iter << ": cost = " << current_cost << std::endl;
        }
        
        // Early termination if no improvement
        if (current_cost >= initial_cost * 0.999) {
            break;
        }
    }
    
    double final_cost = CostCalculator::calculateTotalCost(pl);
    std::cout << "Detailed placement: " << initial_cost << " -> " << final_cost << std::endl;
}

