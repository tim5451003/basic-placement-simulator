#ifndef WRITE_JSON_H
#define WRITE_JSON_H

#include "../model/placement.h"
#include <string>

// Output placement results to JSON format for visualization

class JsonWriter {
public:
    // Write placement to JSON file
    static void writePlacement(const Placement& pl, const std::string& filename);
    
    // Write placement to JSON string
    static std::string placementToJson(const Placement& pl);
};

#endif // WRITE_JSON_H

