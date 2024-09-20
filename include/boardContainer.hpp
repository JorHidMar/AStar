#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
#include "extras.hpp"

struct Board
{
    std::unordered_map<std::string, float> board;
    std::pair<int, int> width = {0, 0};
    std::pair<int, int> height = {0, 0};
    std::map<int, int> xRange;
    std::map<int, int> yRange;  

    float unknown_cell = 0.5;

    void createEmptyBoard(int H, int W);

    void insertMatrix(std::vector<std::vector<float>> m);

    void insert(int x, int y, float value);

    void insert(const std::string &str, float value);

    void remove(int x, int y);

    void remove(const std::string &str);

    void operator=(Board &b);

    float operator[](const std::string &str);

    std::pair<int,int> getWidthRange();

    std::pair<int,int> getHeightRange();

    void clear();

    float get(int x, int y);

    float get(const std::string &str);

    bool find(int x, int y);

    bool find(const std::string &str);

    // TODO: Create empty
    bool empty();

    static std::pair<int, int> getPosition(const std::string &str);

};
