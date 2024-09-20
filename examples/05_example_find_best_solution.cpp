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
    b->addValue(19,4,1.);
    b->addValue(19,5,1.);
    b->addValue(19,6,1.);
    b->addValue(19,7,1.);
    b->addValue(19,8,1.);
    b->addValue(19,9,1.);

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
    VehicleState fState = {22, 6};
    

    // Compute path
    if(!astar.compute(iState, fState)){
        astar.completeCompute();
    }

    // Get result path
    Path path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    b->exportMapAndPath("output_image_05.ppm", path, 20); // TODO: This should be handled by astar class and not board. 

    return 0;
}
