#include "reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Placement InputReader::readFromFile(const std::string& filename) {
    Placement pl;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return pl;
    }
    
    std::string line;
    
    // Read grid dimensions
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        int W, H;
        if (iss >> W >> H) {
            pl.grid = Grid(W, H);
        }
    }
    
    // Read number of cells
    int num_cells = 0;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> num_cells;
    }
    
    // Read cells
    for (int i = 0; i < num_cells; ++i) {
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            int id, x, y, w, h;
            bool fixed = false;
            
            if (iss >> id >> x >> y >> w >> h) {
                std::string fixed_str;
                if (iss >> fixed_str) {
                    fixed = (fixed_str == "fixed" || fixed_str == "1" || fixed_str == "true");
                }
                pl.cells.emplace_back(id, x, y, w, h, fixed);
            }
        }
    }
    
    // Read number of nets
    int num_nets = 0;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> num_nets;
    }
    
    // Read nets
    for (int i = 0; i < num_nets; ++i) {
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            int net_id, num_pins;
            
            if (iss >> net_id >> num_pins) {
                Net net(net_id);
                
                for (int j = 0; j < num_pins; ++j) {
                    int cell_id, offset_x, offset_y;
                    if (iss >> cell_id >> offset_x >> offset_y) {
                        net.pins.emplace_back(cell_id, offset_x, offset_y);
                    }
                }
                
                pl.nets.push_back(net);
            }
        }
    }
    
    file.close();
    pl.updateGrid();
    return pl;
}

Placement InputReader::readSimpleFormat(const std::string& filename) {
    return readFromFile(filename);
}

