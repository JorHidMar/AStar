#pragma once

#include "extras.hpp"
#include <unordered_map>
#include <map>
#include <sstream>

class FixedBoard {
    public:
    FixedBoard();

    ~FixedBoard();

    void createEmptyBoard(int H, int W);

    void loadBoard(std::vector<std::vector<float>> board_);

    void loadBoard(std::vector<std::vector<float>> &board_);

    void printBoard();

    void printBoardAndPath(std::vector<VehicleState> path);
    
    bool checkAvailable(VehicleState &a);

    void augmentBoard(int factor=2);

    void expandBoard(std::vector<std::vector<float>> kernel);

    float getValue(std::string st);

    float getValue(VehicleState &a);
    
    std::vector<std::string> getNeighbours(int i, int j);

    void addValue(std::string st, float v);

    private:
    std::unordered_map<std::string, float> board;
    int min_i=0, max_i=0;
    int min_j=0, max_j=0;

    float wall_limit = 0.9;
};
