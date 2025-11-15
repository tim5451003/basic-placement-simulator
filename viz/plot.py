#!/usr/bin/env python3
"""
Visualization script for placement results.
Reads JSON output and displays the placement using matplotlib.
"""

import json
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import sys
import argparse

def load_placement(filename):
    """Load placement data from JSON file."""
    with open(filename, 'r') as f:
        return json.load(f)

def plot_placement(data, output_file=None, show_nets=True):
    """Plot the placement visualization."""
    fig, ax = plt.subplots(1, 1, figsize=(12, 10))
    
    grid_w = data['grid']['width']
    grid_h = data['grid']['height']
    
    # Set axis limits
    ax.set_xlim(0, grid_w)
    ax.set_ylim(0, grid_h)
    ax.set_aspect('equal')
    ax.grid(True, alpha=0.3)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_title('Placement Visualization')
    
    # Color map for cells
    try:
        # Newer matplotlib API (3.5+)
        colors = plt.get_cmap('tab20')
    except AttributeError:
        # Older matplotlib API
        import matplotlib.cm as cm
        colors = cm.get_cmap('tab20')
    
    # Draw cells
    cell_rects = {}
    for i, cell in enumerate(data['cells']):
        x = cell['x']
        y = cell['y']
        w = cell['w']
        h = cell['h']
        cell_id = cell['id']
        fixed = cell.get('fixed', False)
        
        color = colors(i % 20)
        if fixed:
            # Fixed cells in red
            color = 'red'
            alpha = 0.8
        else:
            alpha = 0.5
        
        rect = patches.Rectangle((x, y), w, h, 
                                linewidth=1.5, edgecolor='black',
                                facecolor=color, alpha=alpha)
        ax.add_patch(rect)
        cell_rects[cell_id] = rect
        
        # Label cell
        ax.text(x + w/2, y + h/2, str(cell_id), 
               ha='center', va='center', fontsize=8, fontweight='bold')
    
    # Draw nets
    if show_nets:
        for net in data['nets']:
            if len(net['pins']) < 2:
                continue
            
            # Get pin positions
            pin_positions = []
            for pin in net['pins']:
                cell_id = pin['cell_id']
                cell = next((c for c in data['cells'] if c['id'] == cell_id), None)
                if cell:
                    pin_x = cell['x'] + pin['offset_x']
                    pin_y = cell['y'] + pin['offset_y']
                    pin_positions.append((pin_x, pin_y))
            
            # Draw bounding box (HPWL visualization)
            if len(pin_positions) >= 2:
                xs = [p[0] for p in pin_positions]
                ys = [p[1] for p in pin_positions]
                min_x, max_x = min(xs), max(xs)
                min_y, max_y = min(ys), max(ys)
                
                # Draw rectangle
                width = max_x - min_x
                height = max_y - min_y
                if width > 0 and height > 0:
                    rect = patches.Rectangle((min_x, min_y), width, height,
                                           linewidth=1, edgecolor='blue',
                                           facecolor='none', linestyle='--', alpha=0.3)
                    ax.add_patch(rect)
                
                # Draw connections (simplified - just show pins)
                for px, py in pin_positions:
                    circle = patches.Circle((px, py), 0.5, color='blue', alpha=0.6)
                    ax.add_patch(circle)
    
    plt.tight_layout()
    
    if output_file:
        plt.savefig(output_file, dpi=150, bbox_inches='tight')
        print(f"Plot saved to {output_file}")
    else:
        plt.show()

def main():
    parser = argparse.ArgumentParser(description='Visualize placement results')
    parser.add_argument('input_file', help='Input JSON file')
    parser.add_argument('-o', '--output', help='Output image file (optional)')
    parser.add_argument('--no-nets', action='store_true', help='Hide net visualization')
    
    args = parser.parse_args()
    
    try:
        data = load_placement(args.input_file)
        plot_placement(data, args.output, show_nets=not args.no_nets)
    except FileNotFoundError:
        print(f"Error: File {args.input_file} not found", file=sys.stderr)
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: Invalid JSON file: {e}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()

