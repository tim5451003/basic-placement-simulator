#include "write_json.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

std::string JsonWriter::placementToJson(const Placement& pl) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "{\n";
    oss << "  \"grid\": {\n";
    oss << "    \"width\": " << pl.grid.W << ",\n";
    oss << "    \"height\": " << pl.grid.H << "\n";
    oss << "  },\n";
    
    oss << "  \"cells\": [\n";
    for (size_t i = 0; i < pl.cells.size(); ++i) {
        const auto& cell = pl.cells[i];
        oss << "    {\n";
        oss << "      \"id\": " << cell.id << ",\n";
        oss << "      \"x\": " << cell.x << ",\n";
        oss << "      \"y\": " << cell.y << ",\n";
        oss << "      \"w\": " << cell.w << ",\n";
        oss << "      \"h\": " << cell.h << ",\n";
        oss << "      \"fixed\": " << (cell.fixed ? "true" : "false") << "\n";
        oss << "    }";
        if (i < pl.cells.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "  ],\n";
    
    oss << "  \"nets\": [\n";
    for (size_t i = 0; i < pl.nets.size(); ++i) {
        const auto& net = pl.nets[i];
        oss << "    {\n";
        oss << "      \"id\": " << net.id << ",\n";
        oss << "      \"pins\": [\n";
        for (size_t j = 0; j < net.pins.size(); ++j) {
            const auto& pin = net.pins[j];
            oss << "        {\n";
            oss << "          \"cell_id\": " << pin.cell_id << ",\n";
            oss << "          \"offset_x\": " << pin.offset_x << ",\n";
            oss << "          \"offset_y\": " << pin.offset_y << "\n";
            oss << "        }";
            if (j < net.pins.size() - 1) oss << ",";
            oss << "\n";
        }
        oss << "      ]\n";
        oss << "    }";
        if (i < pl.nets.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "  ]\n";
    oss << "}\n";
    
    return oss.str();
}

void JsonWriter::writePlacement(const Placement& pl, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return;
    }
    
    file << placementToJson(pl);
    file.close();
    
    std::cout << "Placement written to " << filename << std::endl;
}

