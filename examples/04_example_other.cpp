#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    std::cout << std::fixed << std::setprecision(3);

    // Generate new map 10x10
    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.0);
    b->loadFromFile("../examples/example_map_small");

    // Augment the size of the map by a factor

    // Blurred obstacles using kernel
    std::vector<std::vector<float>> kernel_3x3 = {
        {0.0833, 0.1667, 0.0833},
        {0.1667, 0.0000, 0.1667},
        {0.0833, 0.1667, 0.0833}
    };
    // b->updateAugmentBoard(2);
    // std::vector<std::vector<float>> kernel_5x5 = {
    //     {0.003663, 0.014652, 0.025641, 0.014652, 0.003663},
    //     {0.014652, 0.058608, 0.095238, 0.058608, 0.014652},
    //     {0.025641, 0.095238, 0.150183, 0.095238, 0.025641},
    //     {0.014652, 0.058608, 0.095238, 0.058608, 0.014652},
    //     {0.003663, 0.014652, 0.025641, 0.014652, 0.003663}
    // };
    b->expandBoard(kernel_3x3);
    
    AStar astar(b);

    // Configure A* algorithm
    VehicleConstrains constrains;
    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeEuclideanDistance>();
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<MultiDirectionVehicleMovement>(constrains);
    // std::shared_ptr<VehicleMovement> vMove = std::make_shared<VehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    // Define initial and target position.
    VehicleState iState = {0,0};
    // VehicleState fState = {12,9};
    VehicleState fState = {4,4};
    astar.setPosition(iState);
    astar.setGoal(fState);
    
    // Compute path
    astar.compute();

    // Get result path
    std::vector<VehicleState> path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    std::string gray = "output_grayscale.ppm";
    b->exportMap(gray.c_str(), 20);

    return 0;
}
