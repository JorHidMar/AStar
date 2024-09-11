#include "astar.hpp"

AStar::AStar(std::shared_ptr<FixedBoard> board_) : board(board_){

}

AStar::~AStar(){

}    

std::vector<State> AStar::explore(State &a){
    std::vector<State> next_states;
    for(auto dp : vehicle->move_directions){
        VehicleState next = vehicle->move(a.p, dp);
        
        if(!board->checkAvailable(next)){
            // Check if out of boundaries and if there is an obstacle
            continue;
        }

        float new_g = a.g + computeG->compute(a.p, next, 1.+board->getValue(next));
        float new_h = computeH->compute(next, goal);

        State new_state = {next, new_g, new_h, new_g + new_h, a.p, false};

        next_states.push_back(new_state);
    }

    return next_states;
}

bool AStar::compute(){
    foundSolution = false;

    open_st.clear();
    closed_st.clear();

    float init_h = computeH->compute(pos, goal);
    State st = {pos, 0, init_h, init_h, pos, true};
    open_st.insert(st);
    

    int s = 0;
    while(!open_st.empty()){            // If empty, there is no solution
        State current_state;
        open_st.pop(current_state);
        closed_st.insert(current_state);   

        if(current_state.p == goal){
            foundSolution = true;
            std::cout << "Found solution" << std::endl;
            break;
        }

        auto next_steps = explore(current_state);

        for(auto ns : next_steps){
            bool found = false;

            if(closed_st.find(ns)){     // Check if the node was already visited
                continue;
            }

            open_st.insert(ns);         // Insert node and update if it is repeated with lower evaluation cost (f)
        }
    }   
    return foundSolution;
}

bool AStar::compute(VehicleState &p_init, VehicleState &p_end){
    setPosition(p_init);
    setGoal(p_end);
    return compute();
}

void AStar::setGoal(VehicleState &p){
    goal = p;
}

void AStar::getGoal(VehicleState &p){
    p = goal;
}

void AStar::setPosition(VehicleState &p){
    pos = p;
}

void AStar::getPosition(VehicleState &p){
    p = pos;
}

void AStar::printMap(){
    board->printBoard();
}

bool AStar::getBestPath(std::vector<VehicleState> &path){
    if(!foundSolution){
        std::cout << "Could not find solution" << std::endl;
        return false;
    }

    State goal_state = {goal};
    auto current_pos = closed_st.findGet(goal_state);
    
    path.push_back(current_pos.p);
    while(!current_pos.start){
        path.push_back(current_pos.parent);
        State current_state = {current_pos.parent};
        current_pos = closed_st.findGet(current_state);
    }

    return true;
}

void AStar::printMapAndPath(std::vector<VehicleState> &path){
    if(path.empty()){
        std::cout << "Path is empty" << std::endl;
        return;
    }
    board->printBoardAndPath(path);
}

void AStar::exportImage(std::string &filename, uint factor){
    board->exportMap(filename, factor);
}

void AStar::setComputeCost(std::shared_ptr<computeDistance> cost_function){
    computeG = cost_function;
}

void AStar::setComputeHeuristic(std::shared_ptr<computeDistance> heuristic_function){
    computeH = heuristic_function;
}

void AStar::setVehicle(std::shared_ptr<VehicleMovement> veh){
    vehicle = veh;
}
