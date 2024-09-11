#pragma once

#include "extras.hpp"
#include <map>
#include <list>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>

/**
 * @brief Container to track the states
 */
struct ListState {
    std::unordered_map<std::string, State> m;       // Fast access to state by position
    std::multiset<State> m2;                        // Fast access to state with lowest f value

    // Insert value in list
    // If position is not in the list, add insert it to m and m2. 
    // 
    /**
     * @brief Insert new state in the list.
     * @param state: State to be inserted.
     * @note If position is not exist it insert it. Othewise it will compare the f value of the state, and update with the lower value.
     */
    void insert(State &state);

    /**
     * @brief Pop the value with lowest f value. It removes it from list.
     * @param state: Returned state.
     */
    void pop(State &state);

    /**
     * @brief Search if a state exists for a position.
     */
    bool find(State &state);

    /**
     * @brief Search if a state exists and get the value.
     */
    State findGet(State &state);

    /**
     * @brief Clean list.
     */
    void clear();

    /** 
     * @brief Return if the list is empty 
     */
    bool empty();   // TODO: Should it return both lists.

    void print(){
        for(auto aux: m2){
            aux.print();
        }
    }

};

