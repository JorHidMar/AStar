#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include "listState.hpp"
#include "board.hpp"
#include "vehicleMovement.hpp"
#include "distanceCompute.hpp"

class AStar{

public:
    AStar(std::shared_ptr<FixedBoard> board_);

    ~AStar();

    /**
     * @brief Compute A* algorithm.
     * @return Found a solution.
     * 
     * It uses starting and ending postion set by setPosition and setGoal respectively. If these are not defined, it will use default. 
     */
    bool compute();

    /**
     * @brief Compute A* algorithm.
     * @param p_init:  Initial position.
     * @param p_end:  Final position.
     * @return Found a solution.
     */
    bool compute(const VehicleState &p_init, const VehicleState &p_end);

    bool completeCompute();

    /**
     * @brief Set goal for target position.
     * @param p: Goal position.
     */
    void setGoal(const VehicleState &p);

    /**
     * @brief Get goal position.
     * @param p: Target position.
     */
    void getGoal(VehicleState &p);

    /**
     * @brief Set current position.
     * @param p: Current position.
     */
    void setPosition(const VehicleState &p);

    /**
     * @brief Get goal for target position.
     * @param p: Current position.
     */
    void getPosition(VehicleState &p);

    /**
     * @brief Print internal map.
     */
    void printMap();

    /**
     * @brief Computes best path found. If there is no solution, it will return an empty list.
     * @param path: Output path to target.
     * @return Could find a path.
     */
    bool getBestPath(Path &path);

    /**
     * @brief Prints map and path
     * @param path: Path to be printed on top of the map
     * @note Not suitable for big maps (will not print them properly).
     */
    void printMapAndPath(const Path &path);

    /**
     * @brief Export map to ppm file
     * @param filename: Name of file 
     * @note Only supports .ppm format.
     */
    void exportImage(const std::string &filename, uint factor);
    // TODO: useless, it shuold print the path too.

    /**
     * @brief Set compute cost.
     * @param cost_function: implementation to compute cost function (g).
     * @note You can implement your own implementation of this distance.
     */
    void setComputeCost(std::shared_ptr<computeDistance> cost_function);
    
    /**
     * @brief Set compute heuristic.
     * @param cost_function: implementation to compute cost function (h).
     * @note You can implement your own implementation of this distance.
     */
    void setComputeHeuristic(std::shared_ptr<computeDistance> heuristic_function);

    /**
     * @brief Set vehicle behaviour.
     * @param veh: implementation of vehicles movement.
     * @note You can implement your own movement.
     */
    void setVehicle(std::shared_ptr<VehicleMovement> veh);

protected:

    /**
     * @brief Get future states based on present ones.
     */
    std::vector<State> explore(State &a);

    /**
     * @brief Get future states based on present ones.
     */
    std::vector<State> completeExplore(State &a);

private:
    VehicleState pos;
    VehicleState goal;
    VehicleState alternative_target_state;

    ListState open_st;
    ListState closed_st;

    bool foundSolution = false;
    bool foundAlternativeSolution = false;

    std::shared_ptr<computeDistance> computeG;
    std::shared_ptr<computeDistance> computeH;
    std::shared_ptr<VehicleMovement> vehicle;
    std::shared_ptr<FixedBoard> board;

};
