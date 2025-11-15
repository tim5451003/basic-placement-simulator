#include "cost.h"
#include <algorithm>
#include <cmath>
#include <climits>

void CostCalculator::getPinPosition(const Placement& pl, const Pin& pin, int& x, int& y) {
    const Cell* cell = pl.findCell(pin.cell_id);
    if (cell) {
        x = cell->x + pin.offset_x;
        y = cell->y + pin.offset_y;
    } else {
        x = 0;
        y = 0;
    }
}

double CostCalculator::calculateHPWL(const Placement& pl, const Net& net) {
    if (net.pins.empty()) return 0.0;
    
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    
    for (const auto& pin : net.pins) {
        int x, y;
        getPinPosition(pl, pin, x, y);
        
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }
    
    return (max_x - min_x) + (max_y - min_y);
}

double CostCalculator::calculateTotalHPWL(const Placement& pl) {
    double total = 0.0;
    for (const auto& net : pl.nets) {
        total += calculateHPWL(pl, net);
    }
    return total;
}

double CostCalculator::calculateOverlapPenalty(const Placement& pl) {
    double penalty = 0.0;
    
    // Check all pairs of cells for overlap
    for (size_t i = 0; i < pl.cells.size(); ++i) {
        for (size_t j = i + 1; j < pl.cells.size(); ++j) {
            const Cell& c1 = pl.cells[i];
            const Cell& c2 = pl.cells[j];
            
            // Check if cells overlap
            bool overlap_x = !(c1.x + c1.w <= c2.x || c2.x + c2.w <= c1.x);
            bool overlap_y = !(c1.y + c1.h <= c2.y || c2.y + c2.h <= c1.y);
            
            if (overlap_x && overlap_y) {
                // Calculate overlapping area
                int overlap_w = std::min(c1.x + c1.w, c2.x + c2.w) - std::max(c1.x, c2.x);
                int overlap_h = std::min(c1.y + c1.h, c2.y + c2.h) - std::max(c1.y, c2.y);
                penalty += overlap_w * overlap_h;
            }
        }
    }
    
    return penalty;
}

double CostCalculator::calculateDensityPenalty(const Placement& pl) {
    if (pl.grid.W == 0 || pl.grid.H == 0) return 0.0;
    
    // Divide grid into bins and check density distribution
    const int num_bins = 10;
    int bin_w = pl.grid.W / num_bins;
    int bin_h = pl.grid.H / num_bins;
    
    std::vector<std::vector<int>> bin_density(num_bins, std::vector<int>(num_bins, 0));
    
    // Count cells in each bin
    for (const auto& cell : pl.cells) {
        int bin_x = std::min(cell.x / bin_w, num_bins - 1);
        int bin_y = std::min(cell.y / bin_h, num_bins - 1);
        bin_density[bin_y][bin_x] += cell.w * cell.h;
    }
    
    // Calculate variance in density
    double mean = 0.0;
    int total_area = 0;
    for (int y = 0; y < num_bins; ++y) {
        for (int x = 0; x < num_bins; ++x) {
            total_area += bin_density[y][x];
        }
    }
    mean = total_area / (double)(num_bins * num_bins);
    
    double variance = 0.0;
    for (int y = 0; y < num_bins; ++y) {
        for (int x = 0; x < num_bins; ++x) {
            double diff = bin_density[y][x] - mean;
            variance += diff * diff;
        }
    }
    variance /= (num_bins * num_bins);
    
    return variance;
}

double CostCalculator::calculateTotalCost(const Placement& pl,
                                          double lambda_overlap,
                                          double lambda_density) {
    double hpwl = calculateTotalHPWL(pl);
    double overlap = calculateOverlapPenalty(pl);
    double density = calculateDensityPenalty(pl);
    
    return hpwl + lambda_overlap * overlap + lambda_density * density;
}

double CostCalculator::calculateCostDelta(const Placement& old_pl,
                                          const Placement& new_pl,
                                          double lambda_overlap,
                                          double lambda_density) {
    // For efficiency, we could compute only the delta, but for simplicity
    // we compute full costs. In a production system, incremental updates
    // would be more efficient.
    double old_cost = calculateTotalCost(old_pl, lambda_overlap, lambda_density);
    double new_cost = calculateTotalCost(new_pl, lambda_overlap, lambda_density);
    return new_cost - old_cost;
}

