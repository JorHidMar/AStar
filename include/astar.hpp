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

    /**
     * @brief Compute A* algorithm
     * @return Found a solution
     * 
     * It uses starting and ending postion set by setPosition and setGoal respectively. If these are not defined, it will use default. 
     */
    bool compute();

    /**
     * @brief Compute A* algorithm
     * @param p_init:  Initial position
     * @param p_end:  Final position
     * @return Found a solution
     */
    bool compute(VehicleState &p_init, VehicleState &p_end);

    /**
     * @brief Set goal for target position
     * @param p: Goal position 
     */
    void setGoal(VehicleState &p);

    /**
     * @brief Get goal position
     * @param p: Target position
     */
    void getGoal(VehicleState &p);

    /**
     * @brief Set current position
     * @param p: Current position 
     */
    void setPosition(VehicleState &p);

    /**
     * @brief Get goal for target position
     * @param p: Current position
     */
    void getPosition(VehicleState &p);

    /**
     * @brief Print internal map
     */
    void printMap();

    /**
     * @brief Computes best path found. If there is no solution, it will return an empty list
     */
    std::vector<VehicleState> getBestPath(); // TODO: Should return by reference, return bool if path was found

    /**
     * @brief Prints map and 
     * @note Not suitable for big maps (will not print them properly)
     */
    void printMapAndPath(std::vector<VehicleState> &path);  // TODO: Create class and export to image

    /**
     * @brief Set compute cost
     * @param cost_function: implementation to compute cost function (g)
     * @note You can implement your own implementation of this distance.
     */
    void setComputeCost(std::shared_ptr<computeDistance> cost_function);
    
    /**
     * @brief Set compute heuristic
     * @param cost_function: implementation to compute cost function (h)
     * @note You can implement your own implementation of this distance.
     */
    void setComputeHeuristic(std::shared_ptr<computeDistance> heuristic_function);

    /**
     * @brief Set vehicle behaviour
     * @param veh: implementation of vehicles movement
     * @note You can implement your own movement.
     */
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
