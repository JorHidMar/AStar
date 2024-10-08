#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    // Generate new map 10x10
    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);
    b->createEmptyBoard(10,10);

    // Add obstacles
    b->addValue("1_2", 1.);
    b->addValue("2_4", 1.);

    b->printBoard();

    AStar astar(b);

    // Configure A* algorithm
    VehicleConstrains constrains;
    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeDistance>();
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<VehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    // Define initial and target position.
    VehicleState iState = {0,0};
    VehicleState fState = {9,9};
    astar.setPosition(iState);
    astar.setGoal(fState);
    
    // Compute path
    astar.compute();

    // Get result path
    std::vector<VehicleState> path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    // Export image
    b->exportMap("example_01.ppm", 20);
    b->exportMapAndPath("example_01_1.ppm", path, 20);

    return 0;
}
