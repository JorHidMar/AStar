#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"

int main(){

    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>();

    b->createEmptyBoard(10,10);
    b->addValue("1_2", 1.);
    b->addValue("2_4", 1.);

    std::vector<std::vector<float>> kernel = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };

    b->augmentBoard(2);
    b->expandBoard(kernel);

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

    auto path = astar.getBestPath();
    astar.printMapAndPath(path);

    return 0;
}
