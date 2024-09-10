#pragma once

#include "extras.hpp"
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>

class FixedBoard {
    public:
    FixedBoard(float wall_limit_=0.9, float unknown_cell_=0.5);

    ~FixedBoard();

    /**
     * @brief Create a rectangular map.
     * @param H: Height
     * @param W: Width
     */
    void createEmptyBoard(int H, int W);

    /**
     * @brief Load a board based on a matrix.
     * @param board_: Board to copy.
     */
    void loadBoard(std::vector<std::vector<float>> board_);

    /**
     * @brief Load a board based on an existing map.
     * @param board_: Board to copy.
     */
    void loadBoard(std::vector<std::vector<float>> &board_);

    /**
     * @brief Print the board.
     */
    void printBoard();

    /**
     * @brief Print the board and a path on top of it.
     * @param path: Path to be printed.
     */
    void printBoardAndPath(std::vector<VehicleState> path);
    
    /**
     * @brief Save map in an image.
     * @param filename: Name of the file to export the map.
     */
    void exportMap(const std::string filename, uint factor=5);

    /**
     * @brief Check if the position for the vehicleState exists and it is not blocked.
     */
    bool checkAvailable(VehicleState &a);

    /**
     * @brief Expand the map.
     * @param factor: Factor to expand the map. It must be >1.
     */
    void updateAugmentBoard(uint factor=2); // TODO: Change name

    /**
     * @brief Multiply the map for a kernel to expand obstacles or uncertainties.
     * @param kernel: A 3x3 matrix to apply on the map.
     */
    void expandBoard(std::vector<std::vector<float>> &kernel);   // TODO: Support other type of kernels.

    /**
     * @brief Get value of the map for a position.
     * @param st: String that we want to get the value "{x}_{y}".
     * @return Value if it exists otherwise it returns unknown_cell.
     */
    float getValue(std::string st);

    /**
     * @brief Get value of the map for a position.
     * @param a: vehicleState that we want to get the value "{x}_{y}".
     * @return Value if it exists otherwise it returns unknown_cell.
     */
    float getValue(VehicleState &a);
    
    /**
     * @brief Modify a cell in the map.
     * @param st: Position that you want to change.
     * @param v: New value for the position.  
     */
    void addValue(std::string st, float v);

protected:

    /**
     * @brief Expand the map.
     * @param factor: Factor to expand the map.
     * @param board_copy: Copy of the map that you want to augment.
     */
    void augmentBoard(uint factor, std::unordered_map<std::string, float> &board_copy);

private:

    std::unordered_map<std::string, float> board;
    int min_i=0, max_i=0;
    int min_j=0, max_j=0;

    float wall_limit = 0.9;         // TODO: Modify the paramenter
    float unknown_cell = 0.5;       // TODO: Modify the paramenter
};
