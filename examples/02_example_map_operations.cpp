#include <iostream>
#include <memory>
#include "astar.hpp"
#include "board.hpp"
#include <iomanip>

int main(){

    // Generate new map 10x10
    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);
    b->createEmptyBoard(10,10);

    // Add obstacles and remove tiles
    b->addValue("1_2", 1.);
    b->addValue(2, 4, 1.);
    b->removeValue(5,9);
    b->removeValue("5_8");

    b->printBoard();

    // Blurred obstacles using kernel
    std::vector<std::vector<float>> kernel_3x3 = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };

    b->expandBoard(kernel_3x3);
    b->printBoard();

    b->exportMapCSV("example_02");
    b->loadFromFile("example_02");

    // Add value outside of map
    b->addValue(10,10,0.);

    b->printBoard();

    b->exportMap("example_image_02.ppm", 20);

    ////////////////////////////////////////
    std::vector<std::vector<float>> lBoard = {
        {0, 0, 0, 0, 0},
        {0, 0, -1, -1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };

    std::shared_ptr<FixedBoard> b1 = std::make_shared<FixedBoard>();
    b1->loadBoard(lBoard);

    b1->printBoard();
    b1->exportMapCSV("example_2_1");
    b->loadFromFile("example_2_1");
    b->printBoard();

    b->exportMap("example_image_02_1.ppm", 20);

    return 0;
}
