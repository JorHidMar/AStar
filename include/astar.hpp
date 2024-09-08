#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include <cmath>
#include "stateDataType.hpp"
#include "board.hpp"

class AStar{

public:
    AStar(std::shared_ptr<FixedBoard> board_);

    ~AStar();

    std::vector<State> explore(State &a);

    bool compute();

    void compute(VehicleState &p_init, VehicleState &p_end);

    void setGoal(VehicleState& p);

    void getGoal(VehicleState &p);

    void setPosition(VehicleState& p);

    void getPosition(VehicleState &p);

    void printMap();

    std::vector<VehicleState> getBestPath();

    void printMapAndPath(std::vector<VehicleState> path);

    void setComputeCost(std::shared_ptr<computeDistance> cost_function);
    
    void setComputeHeuristic(std::shared_ptr<computeDistance> heuristic_function);

    void setVehicle(std::shared_ptr<VehicleMovement> veh);

private:
    VehicleState pos;
    VehicleState goal;

    ListState open_st;
    ListState closed_st;

    float threshold = 0.80;

    bool foundSolution = false;

private:
    std::shared_ptr<computeDistance> computeG;
    std::shared_ptr<computeDistance> computeH;
    std::shared_ptr<VehicleMovement> vehicle;
    std::shared_ptr<FixedBoard> board;

};
