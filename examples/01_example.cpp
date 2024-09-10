#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);

    b->createEmptyBoard(10,10);
    b->addValue("1_2", 1.);
    b->addValue("2_4", 1.);

    std::vector<std::vector<float>> kernel_3x3 = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };

    std::vector<std::vector<float>> kernel_5x5 = {
        {0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625},
        {0.015625,   0.0625,   0.09375,   0.0625,   0.015625},
        {0.0234375,  0.09375,  0.140625,  0.09375,  0.0234375},
        {0.015625,   0.0625,   0.09375,   0.0625,   0.015625},
        {0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625}
    };
    std::cout << std::fixed << std::setprecision(4);
    b->updateAugmentBoard(2);
    b->expandBoard(kernel_5x5);

    AStar astar(b);

    VehicleConstrains constrains = {0,0,5,2,0,0,5,2};

    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeEuclideanDistance>();
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<VehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    VehicleState iState = {0,0};
    VehicleState fState = {10,10};
    astar.setPosition(iState);
    astar.setGoal(fState);
    
    astar.compute();

    std::vector<VehicleState> path;
    astar.getBestPath(path);
    astar.printMapAndPath(path);

    std::string gray = "output_grayscale.ppm";
    b->exportMap(gray.c_str(), 20);

    return 0;
}
