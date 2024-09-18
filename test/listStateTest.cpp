#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include "listState.hpp"

class listStateTest : public ::testing::Test {
    protected:
    ListState listState;
    static std::vector<State> states;

    static void SetUpTestSuite(){
        
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib(0, 9);
        std::uniform_int_distribution<> distrib2(0, 20);

        
        int N = 10000;

        for(int i=0; i<N; ++i){
            states.push_back({{distrib(gen),distrib(gen)}, 0, 0,(float)distrib(gen)});
        }
    }

    static void TearDownTestSuite() {
    }

};

std::vector<State> listStateTest::states;

TEST_F(listStateTest, insertValue){

    for(auto &state : states){
        listState.insert(state);
    }


    float min_f = listState.m2.begin()->f;
    float best_min_f = std::min_element(states.begin(), states.end())->f;

    ASSERT_EQ(min_f, best_min_f);
    
}

TEST_F(listStateTest, popValue){
    for(auto &state : states){
        listState.insert(state);
    }

    bool success = true;
    float f_value = listState.m2.begin()->f;
    std::vector<State> popped_states;
    while(!listState.empty()){
        State st;
        listState.pop(st);

        // Check that values are given in the correct order
        if(f_value > st.f){     
            success = false;
            break;
        }
        
        // Check that the states are not repeated
        if(std::find(popped_states.begin(), popped_states.end(), st) != popped_states.end()){
            success = false;
            break;
        }

        // Check that the state is removed correctly
        if(listState.find(st)){
            success = false;
            break;
        }
        popped_states.push_back(st);
        f_value = st.f;
    }

    ASSERT_TRUE(success);
}

TEST_F(listStateTest, findValue){
    
    for(auto &state : states){
        listState.insert(state);
    }


    bool success = true;
    for(auto st : listState.m2){

        // Can find its value
        if(!listState.find(st)){
            success = false;
            break;
        }

        // State was created previously
        if(std::find(states.begin(), states.end(), st) == states.end()){
            success = false;
            break;
        }

        // The state that we get is correct
        auto new_state = listState.findGet(st);
        if(!(new_state == st) && new_state.f != st.f){
            success = false;
            break;
        }
    }

    ASSERT_TRUE(success);

}

TEST_F(listStateTest, overwriteState){
    std::vector<State> sts = {
        {{0,0}, 0, 0, 4},
        {{0,1}, 0, 0, 3},
        {{1,0}, 0, 0, 2},
        {{0,0}, 0, 0, 1}
        };

    for(auto st : sts){
        listState.insert(st);
    }
    
    State st; 

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[3].p && st.f == sts[3].f);

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[2].p && st.f == sts[2].f);

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[1].p && st.f == sts[1].f);
    ASSERT_TRUE(listState.empty());

}

TEST_F(listStateTest, notOverwriteState){
    std::vector<State> sts = {
        {{0,0}, 0, 0, 4},
        {{0,1}, 0, 0, 3},
        {{1,0}, 0, 0, 2},
        {{0,0}, 0, 0, 6}
        };

    for(auto st : sts){
        listState.insert(st);
    }
    
    State st; 

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[2].p && st.f == sts[2].f);

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[1].p && st.f == sts[1].f);

    listState.pop(st);
    ASSERT_TRUE(st.p == sts[0].p && st.f == sts[0].f);
    ASSERT_EQ(st.f, 4.);
    ASSERT_TRUE(listState.empty());

}
