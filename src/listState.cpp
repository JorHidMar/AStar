#include "listState.hpp"

void ListState::insert(State &state){
    
    std::string key = state.p.convert2state();

    if(m.find(key) == m.end()){         // If position is not in the list, add insert it to m and m2. 
        m.insert({key, state});
        m2.insert(state);
    } else {
        if(m[key].f > state.f){         // If a state for a position exists, update the value if the f value is lower
            auto it_lower = m2.lower_bound(m[key]);
            for(auto it=it_lower; it->f <= it_lower->f; ++it){      // Update the m2 set
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

void ListState::pop(State &state){
    state = *m2.begin();
    m2.erase(m2.begin());

    m.erase(state.p.convert2state());
}

bool ListState::find(State &state){
    return m.find(state.p.convert2state()) != m.end();
}

State ListState::findGet(State &state){
    std::string key = state.p.convert2state();

    if(m.find(key) != m.end()){
        return m[key];
    }
    return {};
}

void ListState::clear(){
    m.clear();
    m2.clear();
}

bool ListState::empty(){
    return m2.empty();
}
