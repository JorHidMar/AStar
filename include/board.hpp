#pragma once

#include "boardContainer.hpp"
#include <unordered_map>
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
    void loadBoard(const std::vector<std::vector<float>> &board_);

    /**
     * @brief Load a board based on an existing map.
     * @param board_: Board to copy.
     */
    void loadBoard(Board &board_);

    /** 
     * @brief Load map from file.
     * @param filename
     */
    void loadFromFile(const std::string &filename); // TODO: Should carry extra info

    void operator=(FixedBoard &fb){
        board = fb.board;
    }

    /**
     * @brief Print the board.
     */
    void printBoard();

    /**
     * @brief Print the board and a path on top of it.
     * @param path: Path to be printed.
     */
    void printBoardAndPath(const Path &path);
    
    /**
     * @brief Save map in an image.
     * @param filename: Name of the file to export the map.
     */
    void exportMap(const std::string &filename, uint factor=5);

    /**
     * @brief Save map in an image.
     * @param filename: Name of the file to export the map.
     */
    void exportMapAndPath(const std::string filename, const Path &path, uint factor=5);

    /**
     * @brief Save map in csv file.
     * @param filename: Name of the file to export the map.
     */
    void exportMapCSV(const std::string filename);

    /**
     * @brief Check if the position for the vehicleState exists and it is not blocked.
     */
    bool checkAvailable(VehicleState &a);

    /**
     * @brief Multiply the map for a kernel to expand obstacles or uncertainties.
     * @param kernel: A 3x3 matrix to apply on the map.
     */
    void expandBoard(const std::vector<std::vector<float>> &kernel);

    /**
     * @brief Get value of the map for a position.
     * @param st: String that we want to get the value "{x}_{y}".
     * @return Value if it exists otherwise it returns unknown_cell.
     */
    float getValue(const std::string &st);

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
    void addValue(const std::string &st, float v);

    /**
     * @brief Modify a cell in the map.
     * @param x: x coordinate
     * @param y: y coordinate
     * @param v: New value for the position.  
     */
    void addValue(int x, int y, float v);

    /**
     * @brief Remove a cell in the map.
     * @param x: x coordinate
     * @param y: y coordinate
     */
    void removeValue(int x, int y);

    /**
     * @brief Remove a cell in the map.
     * @param st: Position that you want to change.
     */
    void removeValue(const std::string &st);

    /**
     * @brief Update value for cells outside of the map
     */
    void updateUnknownCells(float unknown_cell_);

    /**
     * @brief Update when a cell is considered a wall.
     */
    void updateWallValue(float wall_limit_);

    /**
     * @brief Copy map to self.
     */
    void getBoard(Board &board_);

    // TODO: board should be a type itself with the minimum and maximum
    Board board;   // TODO: It should be private

private:

    float wall_limit = 0.9;
};
