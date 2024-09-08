#pragma once

#include "extras.hpp"
#include <map>
#include <list>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>

struct ListState {
    std::unordered_map<std::string, State> m;       // Fast access to state by position
    std::multiset<State> m2;                        // Fast access to state with lowest f value

    // Insert value in list
    // If position is not in the list, add insert it to m and m2. 
    // 
    void insert(State &state);

    // Pop lowest value from m list
    void pop(State &state);

    // Return if a states exists for a positon
    bool find(State &state);

    // Find if a state exists for a position and returns if it exists.
    State findGet(State &state);

    // Clear m and m2
    void clear();

    // Return if the list is empty (TODO: Should it return both?)
    bool empty();

};

