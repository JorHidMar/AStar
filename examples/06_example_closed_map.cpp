#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    std::cout << std::fixed << std::setprecision(3);

    // Generate new map 10x10
    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);
    b->loadFromFile("../examples/example_map_closed");
    
    AStar astar(b);

    // Configure A* algorithm
    VehicleConstrains constrains;
    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeEuclideanDistance>(1.5);
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<MultiDirectionVehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    // Define initial and target position.
    VehicleState iState = {5,5};
    VehicleState fState = {20, 20};
    

    // Compute path
    if(!astar.compute(iState, fState)){
        astar.completeCompute();
    }

    // Get result path
    Path path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    b->exportMapAndPath("output_image_06.ppm", path, 20); // TODO: This should be handled by astar class and not board. 

    fState = {8,7};
    // Compute path
    if(!astar.compute(iState, fState)){
        astar.completeCompute();
    }

    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    b->exportMapAndPath("output_image_06_1.ppm", path, 20); // TODO: This should be handled by astar class and not board. 

    return 0;
}
