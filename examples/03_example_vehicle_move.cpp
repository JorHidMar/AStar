#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    std::cout << std::fixed << std::setprecision(3);

    // Generate new map 10x10
    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);
    b->loadFromFile("../examples/example_map");

    // Augment the size of the map by a factor
    std::vector<std::vector<float>> kernel_5x5 = {
        {0.003663, 0.014652, 0.025641, 0.014652, 0.003663},
        {0.014652, 0.058608, 0.095238, 0.058608, 0.014652},
        {0.025641, 0.095238, 0.150183, 0.095238, 0.025641},
        {0.014652, 0.058608, 0.095238, 0.058608, 0.014652},
        {0.003663, 0.014652, 0.025641, 0.014652, 0.003663}
    };
    b->expandBoard(kernel_5x5);
    
    AStar astar(b);

    // Configure A* algorithm
    VehicleConstrains constrains;
    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeEuclideanDistance>(1.5);
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<MultiDirectionVehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    // Define initial and target position.
    VehicleState iState = {0,0};
    // VehicleState fState = {6,7};
    // VehicleState fState = {10,11};
    VehicleState fState = {12, 6};
    
    // Compute path
    astar.compute(iState, fState);

    // Get result path
    Path path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    b->exportMapAndPath("output_image_03.ppm", path, 20); // TODO: This should be handled by astar class and not board. 

    return 0;
}
