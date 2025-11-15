#ifndef DETAIL_PLACE_H
#define DETAIL_PLACE_H

#include "../model/placement.h"

// Detailed placement: local refinement to further reduce wire length

class DetailedPlacer {
public:
    // Perform detailed placement refinement
    static void detailedPlace(Placement& pl, int window_size = 5, int max_iterations = 10);
    
    // Optimize within a local window
    static void optimizeWindow(Placement& pl, int center_x, int center_y, int window_size);
    
private:
    // Try small perturbations in a window
    static bool tryLocalMove(Placement& pl, Cell& cell, int window_size);
};

#endif // DETAIL_PLACE_H

