#include "stateDataType.hpp"

// Insert value in list
// If position is not in the list, add insert it to m and m2. 
// 
void ListState::insert(State &state){
    // m2.insert(state);
    
    std::string key = std::to_string(state.p.x) + "_";
    key += std::to_string(state.p.y);

    if(m.find(key) == m.end()){         // If position is not in the list, add insert it to m and m2. 
        m.insert({key, state});
        m2.insert(state);
    } else {
        if(m[key].f > state.f){         // If a state for a position exists, update the value if the f value is lower
            auto it_lower = m2.lower_bound(m[key]);
            for(auto it=it_lower; it->f <= it_lower->f; it++){      // Update the m2 set
                if(m[key] == *it){
                    m2.erase(it);
                    break;
                }
            }
            m2.insert(state);
            m[key] = state;
        }
    }
}

// Pop lowest value from m list
void ListState::pop(State &state){
    state = *m2.begin();
    m2.erase(m2.begin());

    std::string key = std::to_string(state.p.x) + "_";
    key += std::to_string(state.p.y);

    m.erase(key);
}

// Return if a states exists for a positon
bool ListState::find(State &state){
    std::string key = std::to_string(state.p.x) + "_";
    key += std::to_string(state.p.y);

    return m.find(key) != m.end();
}

// Find if a state exists for a position and returns if it exists.
State ListState::findGet(State &state){
    std::string key = std::to_string(state.p.x) + "_";
    key += std::to_string(state.p.y);

    if(m.find(key) != m.end()){
        return m[key];
    }
    return {};
}

// Clear m and m2
void ListState::clear(){
    m.clear();
    m2.clear();
}

// Return if the list is empty (TODO: Should it return both?)
bool ListState::empty(){
    return m2.empty();
}
