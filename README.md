# Basic Placement Simulator

A simplified placement simulator for EDA (Electronic Design Automation) that places standard cells on a 2D chip grid while optimizing for minimal total wire length (HPWL) and minimal overlap.

## Overview

This project implements a basic placement stage similar to what's found in physical design software like Cadence Innovus. It uses simulated annealing to optimize cell placement, followed by legalization and detailed placement refinement.

## Features

- **Cell Placement**: Place cells on a 2D grid with width and height constraints
- **Wire Length Optimization**: Minimize Half-Perimeter Wire Length (HPWL)
- **Overlap Removal**: Legalization to remove cell overlaps
- **Simulated Annealing**: Optimization algorithm for placement
- **Visualization**: Python script to visualize placement results

## Project Structure

```
.
├── main.cpp              # Main entry point
├── model/                # Data structures
│   ├── placement.h
│   └── placement.cc
├── io/                   # Input/output
│   ├── reader.h
│   └── reader.cc
├── cost/                 # Cost functions
│   ├── cost.h
│   └── cost.cc
├── opt/                  # Optimization
│   ├── anneal.h
│   └── anneal.cc
├── legal/                # Legalization
│   ├── legalize.h
│   └── legalize.cc
├── detail/               # Detailed placement
│   ├── detail_place.h
│   └── detail_place.cc
├── viz/                  # Visualization
│   ├── write_json.h
│   ├── write_json.cc
│   └── plot.py
├── CMakeLists.txt        # CMake build file
├── Makefile              # Make build file
└── test_input.txt        # Sample input file
```

## Prerequisites

- **C++17 Compiler**: g++ (MinGW-w64), MSVC (Visual Studio), or Clang
- **Python 3** (optional, for visualization)
- **matplotlib** (optional): `pip install matplotlib`

## Building

### Option 1: VS Code (Easiest)

1. **Build:** Press `Ctrl+Shift+B` (or Terminal → Run Build Task)
2. **Run:** Press `F5` (or Run → Start Debugging)

### Option 2: Using Make

```bash
make
```

### Option 3: Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Option 4: Using Build Script (Windows)

```powershell
.\build.bat
```

### Option 5: Manual Compilation

```powershell
g++ -std=c++17 -Wall -Wextra -O2 -I. main.cpp model\placement.cc io\reader.cc cost\cost.cc opt\anneal.cc legal\legalize.cc detail\detail_place.cc viz\write_json.cc -o placement_simulator.exe
```

## Usage

### Basic Usage

```bash
./placement_simulator input.txt output.json
```

### Input Format

The input file should follow this format:

```
grid_width grid_height
num_cells
cell_id x y w h [fixed]
...
num_nets
net_id num_pins cell_id offset_x offset_y ...
...
```

Example:
```
100 100
3
0 10 10 5 5
1 20 20 4 4
2 30 30 6 6
2
0 2 0 2 2 1 1 1
1 3 0 0 0 1 0 0 2 0 0
```

### Visualization

After running the simulator, visualize the results:

**Using the visualization script:**
```powershell
.\visualize.bat
```

**Or directly with Python:**
```powershell
python viz\plot.py output.json -o placement.png
```

**Interactive view (opens in window):**
```powershell
python viz\plot.py output.json
```

The visualization shows:
- **Cells**: Colored rectangles with cell IDs
- **Fixed cells**: Shown in red (if any)
- **Nets**: Blue dashed rectangles showing HPWL bounding boxes
- **Grid**: Background showing the chip layout area

**Note:** Requires Python 3 with matplotlib installed. Install with:
```powershell
python -m pip install matplotlib
```

## Algorithm

1. **Initial Placement**: Randomly place cells on the grid
2. **Simulated Annealing**: 
   - Propose moves (shift or swap)
   - Accept moves based on cost improvement or probability
   - Gradually cool down temperature
3. **Legalization**: Remove overlaps by snapping cells to free positions
4. **Detailed Placement**: Local refinement to further reduce wire length

## Cost Function

The total cost is computed as:

```
Cost = HPWL + λ_overlap × OverlapPenalty + λ_density × DensityPenalty
```

Where:
- **HPWL** (Half-Perimeter Wire Length): Sum of bounding box perimeters for all nets
- **OverlapPenalty**: Sum of overlapping cell areas
- **DensityPenalty**: Variance in cell density across grid bins

## Parameters

Default parameters:
- Grid size: 100×100 (configurable via input)
- Initial temperature (T0): 1000.0
- Cooling factor (α): 0.90
- Max epochs: 100
- Moves per epoch: 10 × number_of_cells
- Cost weights: λ_overlap = 1.0, λ_density = 0.1

## Testing

Start with small test cases (5-10 cells, few nets) and gradually scale up. The included `test_input.txt` provides a sample test case.

## Troubleshooting

### "g++ is not recognized"
- Install MinGW-w64 and add to PATH
- Restart VS Code/terminal after installation
- Verify with: `g++ --version`

### Build task fails in VS Code
- Ensure g++ is in your PATH
- Try building manually in terminal
- Check that all source files exist

### "Execution policy error" (PowerShell)
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Compilation errors
- Ensure C++17 compatible compiler
- Check include paths are correct
- Verify all source files are present

## Requirements

- C++17 compiler (g++, clang++, or MSVC)
- Python 3 with matplotlib (for visualization)
- CMake 3.10+ (optional, for CMake build)

## License

This is an educational project for learning EDA placement algorithms.

