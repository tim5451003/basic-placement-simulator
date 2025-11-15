#ifndef ANNEAL_H
#define ANNEAL_H

#include "../model/placement.h"
#include "../cost/cost.h"
#include <random>

// Simulated annealing optimizer for placement

struct Move {
    enum Type { SHIFT, SWAP };
    Type type;
    int cell_id1;
    int cell_id2;  // For swap moves
    int new_x, new_y;  // For shift moves
    
    Move() : type(SHIFT), cell_id1(-1), cell_id2(-1), new_x(0), new_y(0) {}
};

class SimulatedAnnealing {
public:
    SimulatedAnnealing(double T0 = 1000.0, double alpha = 0.90, 
                      double lambda_overlap = 1.0, double lambda_density = 0.1)
        : T0_(T0), alpha_(alpha), lambda_overlap_(lambda_overlap),
          lambda_density_(lambda_density), rng_(std::random_device{}()) {}
    
    // Perform initial random placement
    void randomInitialPlacement(Placement& pl);
    
    // Propose a random move
    Move proposeMove(const Placement& pl);
    
    // Check if a move is valid
    bool isValidMove(const Placement& pl, const Move& move);
    
    // Apply a move to placement
    void applyMove(Placement& pl, const Move& move);
    
    // Run simulated annealing optimization
    void optimize(Placement& pl, int max_epochs = 100, int moves_per_epoch = 0);
    
    // Get current temperature
    double getTemperature() const { return T_; }
    
private:
    double T0_;  // Initial temperature
    double alpha_;  // Cooling factor
    double lambda_overlap_;
    double lambda_density_;
    double T_;  // Current temperature
    std::mt19937 rng_;
    
    // Random number generators
    double rand01() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng_);
    }
    
    int randInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng_);
    }
    
    // Check if placement has stalled (cost not improving)
    bool hasStalled(const std::vector<double>& cost_history, size_t window = 10) {
        if (cost_history.size() < window) return false;
        double recent_avg = 0.0;
        double older_avg = 0.0;
        
        for (size_t i = 0; i < window; ++i) {
            recent_avg += cost_history[cost_history.size() - 1 - i];
            older_avg += cost_history[cost_history.size() - window - 1 - i];
        }
        recent_avg /= static_cast<double>(window);
        older_avg /= static_cast<double>(window);
        
        // Stalled if improvement is less than 1%
        return (older_avg - recent_avg) / older_avg < 0.01;
    }
};

#endif // ANNEAL_H

