#ifndef COST_H
#define COST_H

#include "../model/placement.h"

// Cost function computation for placement optimization

class CostCalculator {
public:
    // Calculate Half-Perimeter Wire Length for a net
    static double calculateHPWL(const Placement& pl, const Net& net);
    
    // Calculate total HPWL for all nets
    static double calculateTotalHPWL(const Placement& pl);
    
    // Calculate overlap penalty (sum of overlapping cell areas)
    static double calculateOverlapPenalty(const Placement& pl);
    
    // Calculate density penalty (imbalance in cell distribution)
    static double calculateDensityPenalty(const Placement& pl);
    
    // Calculate total cost with weights
    static double calculateTotalCost(const Placement& pl, 
                                     double lambda_overlap = 1.0,
                                     double lambda_density = 0.1);
    
    // Calculate cost delta for a proposed move
    // This is more efficient than recalculating total cost
    static double calculateCostDelta(const Placement& old_pl,
                                    const Placement& new_pl,
                                    double lambda_overlap = 1.0,
                                    double lambda_density = 0.1);
    
private:
    // Helper to get pin position
    static void getPinPosition(const Placement& pl, const Pin& pin, int& x, int& y);
};

#endif // COST_H

