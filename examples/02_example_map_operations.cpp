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

    // Augment the size of the map by a factor
    b->updateAugmentBoard(2);

    // Blurred obstacles using kernel
    std::vector<std::vector<float>> kernel_3x3 = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };

    b->expandBoard(kernel_3x3);
    b->exportMap("output.ppm", 20);

    //////////////////////////////////////////
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

    //////////////////////////////////////////
    std::shared_ptr<FixedBoard> b2 = std::make_shared<FixedBoard>();
    b2->loadFromFile("example_2_1");
    b2->printBoard();
    b2->exportMapCSV("example_2_2");

    return 0;
}
