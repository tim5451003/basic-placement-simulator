#ifndef LEGALIZE_H
#define LEGALIZE_H

#include "../model/placement.h"

// Legalization: remove overlaps by snapping cells to free grid positions

class Legalizer {
public:
    // Legalize placement by removing overlaps
    static void legalize(Placement& pl);
    
    // Find nearest free position for a cell
    static bool findFreePosition(const Placement& pl, const Cell& cell, int& new_x, int& new_y);
    
    // Check if a cell can be placed at a position without overlap
    static bool canPlace(const Placement& pl, const Cell& cell, int x, int y);
    
private:
    // Helper to clear cell from grid
    static void clearCellFromGrid(Placement& pl, int cell_id);
};

#endif // LEGALIZE_H

