#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include "boardContainer.hpp"

class boardContainerTest : public ::testing::Test {
    protected:
    Board board;

    static void SetUpTestSuite(){
        
    }

    static void TearDownTestSuite() {
    }

};


TEST_F(boardContainerTest, insertLimitHandler){

    board.insert(1, 2, 0.);

    // Change the values
    ASSERT_TRUE(board.width.first == 1 && board.width.second == 1 &&
        board.height.first == 2 && board.height.second == 2);
    ASSERT_TRUE(board.xRange.find(1) != board.xRange.end());
    ASSERT_EQ(board.xRange[1], 1);
    ASSERT_TRUE(board.yRange.find(2) != board.yRange.end());
    ASSERT_EQ(board.yRange[2], 1);

    board.insert(3,4,0.);

    ASSERT_TRUE(board.width.first == 1 && board.width.second == 3 &&
        board.height.first == 2 && board.height.second == 4);
    ASSERT_TRUE(board.xRange.find(3) != board.xRange.end());
    ASSERT_EQ(board.xRange[3], 1);
    ASSERT_TRUE(board.yRange.find(4) != board.yRange.end());
    ASSERT_EQ(board.yRange[4], 1);

    board.insert(-2,-1,0.);
    ASSERT_TRUE(board.width.first == -2 && board.width.second == 3 &&
        board.height.first == -1 && board.height.second == 4);
    ASSERT_TRUE(board.xRange.find(-2) != board.xRange.end());
    ASSERT_EQ(board.xRange[-2], 1);
    ASSERT_TRUE(board.yRange.find(-1) != board.yRange.end());
    ASSERT_EQ(board.yRange[-1], 1);

    ASSERT_EQ(board.xRange.begin()->first, -2);
    ASSERT_EQ(std::prev(board.xRange.end())->first, 3);
    ASSERT_EQ(board.yRange.begin()->first, -1);
    ASSERT_EQ(std::prev(board.yRange.end())->first, 4);
    
}

TEST_F(boardContainerTest, removeLimitHandler){

    board.insert(1, 2, 0.);
    ASSERT_TRUE(board.width.first == 1 && board.width.second == 1 &&
        board.height.first == 2 && board.height.second == 2);
    board.remove(1, 2);

    // Change the values
    ASSERT_TRUE(board.empty());
    ASSERT_TRUE(board.xRange.empty());
    ASSERT_TRUE(board.yRange.empty());
    ASSERT_TRUE(board.width.first == 0 && board.width.second == 0 &&
        board.height.first == 0 && board.height.second == 0);

    board.insert(3,4,0.);
    ASSERT_TRUE(board.width.first == 3 && board.width.second == 3 &&
        board.height.first == 4 && board.height.second == 4);

    board.insert(0,0,0.);
    board.insert(1,1,0.);
    board.insert(2,1,0.);
    board.insert(3,1,0.);
    ASSERT_TRUE(board.width.first == 0 && board.width.second == 3 &&
        board.height.first == 0 && board.height.second == 4);
    ASSERT_EQ(board.xRange[1], 1);
    ASSERT_EQ(board.yRange[1], 3);

    board.remove(0,0);
    ASSERT_TRUE(board.width.first == 1 && board.width.second == 3 &&
        board.height.first == 1 && board.height.second == 4);
    ASSERT_TRUE(board.xRange.find(0) == board.xRange.end());
    ASSERT_TRUE(board.xRange.begin()->first == 1 &&
                board.xRange.begin()->second == 1);
    ASSERT_TRUE(board.yRange.find(0) == board.yRange.end());
    ASSERT_TRUE(board.yRange.begin()->first == 1 &&
                board.yRange.begin()->second == 3);

    board.remove(1,1);
    ASSERT_TRUE(board.width.first == 2 && board.width.second == 3 &&
        board.height.first == 1 && board.height.second == 4);
    ASSERT_TRUE(board.xRange.find(1) == board.xRange.end());
    ASSERT_TRUE(board.xRange.begin()->first == 2 &&
                board.xRange.begin()->second == 1);
    ASSERT_TRUE(board.yRange.find(1) != board.yRange.end());
    ASSERT_TRUE(board.yRange.begin()->first == 1 &&
                board.yRange.begin()->second == 2);

    board.remove(1,1); 
    ASSERT_TRUE(board.width.first == 2 && board.width.second == 3 &&
        board.height.first == 1 && board.height.second == 4);
    ASSERT_TRUE(board.xRange.find(1) == board.xRange.end());
    ASSERT_TRUE(board.xRange.begin()->first == 2 &&
                board.xRange.begin()->second == 1);
    ASSERT_TRUE(board.yRange.find(1) != board.yRange.end());
    ASSERT_TRUE(board.yRange.begin()->first == 1 &&
                board.yRange.begin()->second == 2);
}

TEST_F(boardContainerTest, randomAddAndRemove){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0, 99);

    std::vector<int> n_xValues(100, 0);
    std::vector<int> n_yValues(100, 0);
    
    int N_add = 50;
    int N_remove = 2000;

    std::vector<std::pair<int,int>> valuesAdd;
    std::vector<std::pair<int,int>> valuesRemove;

    for(int i=0; i<N_add; ++i){
        valuesAdd.push_back({distrib(gen),distrib(gen)});
    }
    for(int i=0; i<N_remove; ++i){
        valuesRemove.push_back({distrib(gen),distrib(gen)});
    }

    int min_i=valuesAdd[0].first, max_i=valuesAdd[0].first;
    int min_j=valuesAdd[0].second, max_j=valuesAdd[0].second;

    for(auto v : valuesAdd){
        int x = v.first;
        int y = v.second;


        if(min_i > x){
            min_i = x;
        } else if(max_i < x){
            max_i = x;
        }

        if(min_j > y){
            min_j = y;
        } else if(max_j < y){
            max_j = y;
        }

        if(!board.find(x, y)){
            n_xValues[x] += 1;
            n_yValues[y] += 1;
        }

        board.insert(x, y, 0.);
    }

    ASSERT_TRUE(min_i == board.width.first && max_i == board.width.second &&
        min_j == board.height.first && max_j == board.height.second);

    for(auto v : valuesRemove){
        int x = v.first;
        int y = v.second;

        if(board.find(x, y)){
            n_xValues[x] -= 1;
            n_yValues[y] -= 1;
        }

        board.remove(x, y);
    }

    bool found_min_x = false;
    for(int i=0; i<n_xValues.size(); ++i){
        if(n_xValues[i] > 0 && !found_min_x){
            min_i = i;
            max_i = i;
            found_min_x = true;
        } else if(n_xValues[i]>0){
            max_i = i;
        }
    }
    bool found_min_y = false;
    for(int i=0; i<n_yValues.size(); ++i){
        if(n_yValues[i] > 0 && !found_min_y){
            min_j = i;
            max_j = i;
            found_min_y = true;
        } else if(n_yValues[i] > 0){
            max_j = i;
        }
    }

    if(!(found_min_x && found_min_y)){
        ASSERT_TRUE(board.empty());
        ASSERT_TRUE(board.xRange.empty());
        ASSERT_TRUE(board.yRange.empty());
        ASSERT_TRUE(board.width.first == 0 && board.width.second == 0 &&
            board.height.first == 0 && board.height.second == 0);
    } else {
        ASSERT_TRUE(min_i == board.width.first && max_i == board.width.second &&
            min_j == board.height.first && max_j == board.height.second);
    }
}