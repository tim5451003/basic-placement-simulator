#ifndef READER_H
#define READER_H

#include "../model/placement.h"
#include <string>

// Input reader for parsing placement data

class InputReader {
public:
    // Read placement data from file
    // Expected format:
    // Line 1: grid_width grid_height
    // Line 2: num_cells
    // Next num_cells lines: cell_id x y w h [fixed]
    // Line: num_nets
    // Next num_nets lines: net_id num_pins [cell_id offset_x offset_y]*
    static Placement readFromFile(const std::string& filename);
    
    // Read from simple text format
    static Placement readSimpleFormat(const std::string& filename);
    
private:
    static void parseGrid(Placement& pl, const std::string& line);
    static void parseCell(Placement& pl, const std::string& line);
    static void parseNet(Placement& pl, const std::string& line);
};

#endif // READER_H

