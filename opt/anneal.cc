#include "anneal.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void SimulatedAnnealing::randomInitialPlacement(Placement& pl) {
    for (auto& cell : pl.cells) {
        if (cell.fixed) continue;
        
        // Random position within grid bounds
        int max_x = std::max(0, pl.grid.W - cell.w);
        int max_y = std::max(0, pl.grid.H - cell.h);
        
        if (max_x > 0) {
            cell.x = randInt(0, max_x);
        } else {
            cell.x = 0;
        }
        
        if (max_y > 0) {
            cell.y = randInt(0, max_y);
        } else {
            cell.y = 0;
        }
    }
    pl.updateGrid();
}

Move SimulatedAnnealing::proposeMove(const Placement& pl) {
    Move move;
    
    // Randomly choose between shift and swap
    if (rand01() < 0.7) {  // 70% shift, 30% swap
        // Shift move
        move.type = Move::SHIFT;
        
        // Find a movable cell
        std::vector<int> movable_cells;
        for (const auto& cell : pl.cells) {
            if (!cell.fixed) {
                movable_cells.push_back(cell.id);
            }
        }
        
        if (movable_cells.empty()) {
            move.cell_id1 = -1;
            return move;
        }
        
        move.cell_id1 = movable_cells[randInt(0, movable_cells.size() - 1)];
        const Cell* cell = pl.findCell(move.cell_id1);
        
        if (cell) {
            int max_x = std::max(0, pl.grid.W - cell->w);
            int max_y = std::max(0, pl.grid.H - cell->h);
            
            move.new_x = randInt(0, max_x);
            move.new_y = randInt(0, max_y);
        }
    } else {
        // Swap move
        move.type = Move::SWAP;
        
        std::vector<int> movable_cells;
        for (const auto& cell : pl.cells) {
            if (!cell.fixed) {
                movable_cells.push_back(cell.id);
            }
        }
        
        if (movable_cells.size() < 2) {
            move.cell_id1 = -1;
            return move;
        }
        
        int idx1 = randInt(0, movable_cells.size() - 1);
        int idx2 = randInt(0, movable_cells.size() - 1);
        while (idx2 == idx1 && movable_cells.size() > 1) {
            idx2 = randInt(0, movable_cells.size() - 1);
        }
        
        move.cell_id1 = movable_cells[idx1];
        move.cell_id2 = movable_cells[idx2];
    }
    
    return move;
}

bool SimulatedAnnealing::isValidMove(const Placement& pl, const Move& move) {
    if (move.cell_id1 == -1) return false;
    
    if (move.type == Move::SHIFT) {
        const Cell* cell = pl.findCell(move.cell_id1);
        if (!cell) return false;
        if (cell->fixed) return false;
        
        // Check bounds
        if (move.new_x < 0 || move.new_y < 0) return false;
        if (move.new_x + cell->w > pl.grid.W) return false;
        if (move.new_y + cell->h > pl.grid.H) return false;
        
        return true;
    } else {  // SWAP
        const Cell* cell1 = pl.findCell(move.cell_id1);
        const Cell* cell2 = pl.findCell(move.cell_id2);
        
        if (!cell1 || !cell2) return false;
        if (cell1->fixed || cell2->fixed) return false;
        
        return true;
    }
}

void SimulatedAnnealing::applyMove(Placement& pl, const Move& move) {
    if (move.type == Move::SHIFT) {
        Cell* cell = pl.findCell(move.cell_id1);
        if (cell) {
            cell->x = move.new_x;
            cell->y = move.new_y;
        }
    } else {  // SWAP
        Cell* cell1 = pl.findCell(move.cell_id1);
        Cell* cell2 = pl.findCell(move.cell_id2);
        
        if (cell1 && cell2) {
            std::swap(cell1->x, cell2->x);
            std::swap(cell1->y, cell2->y);
        }
    }
    pl.updateGrid();
}

void SimulatedAnnealing::optimize(Placement& pl, int max_epochs, int moves_per_epoch) {
    // Initialize random placement if needed
    randomInitialPlacement(pl);
    
    T_ = T0_;
    
    // Default moves per epoch: 10 × number of cells
    if (moves_per_epoch == 0) {
        moves_per_epoch = 10 * pl.cells.size();
    }
    
    std::vector<double> cost_history;
    double current_cost = CostCalculator::calculateTotalCost(pl, lambda_overlap_, lambda_density_);
    cost_history.push_back(current_cost);
    
    std::cout << "Initial cost: " << current_cost << std::endl;
    
    for (int epoch = 0; epoch < max_epochs; ++epoch) {
        int accepted_moves = 0;
        
        for (int it = 0; it < moves_per_epoch; ++it) {
            Move move = proposeMove(pl);
            
            if (!isValidMove(pl, move)) continue;
            
            // Create a copy to evaluate the move
            Placement test_pl = pl;
            applyMove(test_pl, move);
            
            double new_cost = CostCalculator::calculateTotalCost(test_pl, lambda_overlap_, lambda_density_);
            double delta_cost = new_cost - current_cost;
            
            // Accept or reject
            bool accept = false;
            if (delta_cost <= 0) {
                accept = true;
            } else {
                double prob = std::exp(-delta_cost / T_);
                if (rand01() < prob) {
                    accept = true;
                }
            }
            
            if (accept) {
                applyMove(pl, move);
                current_cost = new_cost;
                accepted_moves++;
            }
        }
        
        cost_history.push_back(current_cost);
        
        // Cool down
        T_ *= alpha_;
        
        // Print progress
        if (epoch % 10 == 0 || epoch == max_epochs - 1) {
            std::cout << "Epoch " << epoch << ": cost = " << current_cost 
                      << ", T = " << T_ << ", accepted = " << accepted_moves 
                      << "/" << moves_per_epoch << std::endl;
        }
        
        // Check for convergence
        if (hasStalled(cost_history)) {
            std::cout << "Converged at epoch " << epoch << std::endl;
            break;
        }
    }
    
    std::cout << "Final cost: " << current_cost << std::endl;
}

