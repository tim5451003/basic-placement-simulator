#include "io/reader.h"
#include "opt/anneal.h"
#include "legal/legalize.h"
#include "detail/detail_place.h"
#include "viz/write_json.h"
#include "cost/cost.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string input_file = "input.txt";
    std::string output_file = "placement.json";
    
    // Parse command line arguments
    if (argc > 1) {
        input_file = argv[1];
    }
    if (argc > 2) {
        output_file = argv[2];
    }
    
    std::cout << "Basic Placement Simulator" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << std::endl;
    
    // Step 1: Read input
    std::cout << "Step 1: Reading input..." << std::endl;
    Placement pl = InputReader::readFromFile(input_file);
    
    if (pl.cells.empty()) {
        std::cerr << "Error: No cells loaded. Check input file format." << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << pl.cells.size() << " cells, " 
              << pl.nets.size() << " nets" << std::endl;
    std::cout << "Grid size: " << pl.grid.W << " x " << pl.grid.H << std::endl;
    std::cout << std::endl;
    
    // Step 2: Initial placement (random)
    std::cout << "Step 2: Initial placement..." << std::endl;
    double initial_cost = CostCalculator::calculateTotalCost(pl);
    std::cout << "Initial cost: " << initial_cost << std::endl;
    std::cout << "  HPWL: " << CostCalculator::calculateTotalHPWL(pl) << std::endl;
    std::cout << "  Overlap: " << CostCalculator::calculateOverlapPenalty(pl) << std::endl;
    std::cout << std::endl;
    
    // Step 3: Simulated annealing optimization
    std::cout << "Step 3: Simulated annealing optimization..." << std::endl;
    SimulatedAnnealing sa(1000.0, 0.90, 1.0, 0.1);  // T0=1000, alpha=0.90
    sa.optimize(pl, 100, 0);  // 100 epochs, auto moves_per_epoch
    std::cout << std::endl;
    
    // Step 4: Legalization
    std::cout << "Step 4: Legalization..." << std::endl;
    Legalizer::legalize(pl);
    double after_legal_cost = CostCalculator::calculateTotalCost(pl);
    std::cout << "Cost after legalization: " << after_legal_cost << std::endl;
    std::cout << std::endl;
    
    // Step 5: Detailed placement
    std::cout << "Step 5: Detailed placement..." << std::endl;
    DetailedPlacer::detailedPlace(pl, 5, 10);
    std::cout << std::endl;
    
    // Step 6: Final results
    std::cout << "Step 6: Final results..." << std::endl;
    double final_cost = CostCalculator::calculateTotalCost(pl);
    std::cout << "Final cost: " << final_cost << std::endl;
    std::cout << "  HPWL: " << CostCalculator::calculateTotalHPWL(pl) << std::endl;
    std::cout << "  Overlap: " << CostCalculator::calculateOverlapPenalty(pl) << std::endl;
    std::cout << "  Improvement: " << ((initial_cost - final_cost) / initial_cost * 100.0) 
              << "%" << std::endl;
    std::cout << std::endl;
    
    // Step 7: Write output
    std::cout << "Step 7: Writing output..." << std::endl;
    JsonWriter::writePlacement(pl, output_file);
    
    std::cout << std::endl;
    std::cout << "Placement complete!" << std::endl;
    std::cout << "To visualize, run: python viz/plot.py " << output_file << std::endl;
    
    return 0;
}

