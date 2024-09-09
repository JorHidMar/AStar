#include "board.hpp"

FixedBoard::FixedBoard(float wall_limit_, float unkown_cell_) : wall_limit(wall_limit_), unknown_cell(unknown_cell) {

}

FixedBoard::~FixedBoard(){

}

void FixedBoard::createEmptyBoard(int H, int W){
    board.clear();
    min_i = 0;
    min_j = 0;
    max_i = H;
    max_j = W;

    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            std::string key = std::to_string(i) + "_";
            key += std::to_string(j);   
            board.insert({key, 0.0});
        }
    }
}

void FixedBoard::loadBoard(std::vector<std::vector<float>> board_){
    board.clear();
    int i=0;
    for(auto row: board_){
        int j=0;
        for(auto col: row){
            if(col != -1) {
                std::string key = std::to_string(i) + "_";
                key += std::to_string(j);
                board.insert({key, col});
            }
        }
    }
}

void FixedBoard::loadBoard(std::vector<std::vector<float>> &board_){
    board.clear();
    // board.insert(board_.begin(), board_.end());
}

void FixedBoard::printBoard(){
    for(int i=min_i; i<max_i; i++){
        for(int j=min_j; j<max_j; j++){
            std::string st = std::to_string(i) + "_";
            st += std::to_string(j);

            if(board.find(st) != board.end()){
                std::cout << board[st] << "\t";
            } else {
                std::cout << "-\t";
            }
            
        }
        std::cout << std::endl;
    }
}

void FixedBoard::printBoardAndPath(std::vector<VehicleState> path){
    for(auto p : path){
        std::string st = std::to_string(p.x) + "_";
        st += std::to_string(p.y);
        board[st] = 2.;
    }

    for(int i=min_i; i<max_i; i++){
        for(int j=min_j; j<max_j; j++){
            std::string st = std::to_string(i) + "_";
            st += std::to_string(j);

            if(board.find(st) != board.end()){
                std::cout << board[st] << "\t";
            } else {
                std::cout << "-\t";
            }
            
        }
        std::cout << std::endl;
    }
}
    
bool FixedBoard::checkAvailable(VehicleState &a){

    std::string st = std::to_string(a.x) + "_";
    st += std::to_string(a.y);
    if(board.find(st) != board.end()){
        if(board[st] < wall_limit){
            return true;
        }
    }
    return false;
}

void FixedBoard::augmentBoard(uint factor){

    if(factor < 2){
        return;
    }

    std::unordered_map<std::string, float> board_copy;
    for(auto b: board){
        std::stringstream ss(b.first);
        int n[2];
        int k=0;
        std::string num;
        while(std::getline(ss, num, '_')){
            n[k++] = std::stoi(num);
        }

        for(int i=0; i<factor; i++){
            for(int j=0; j<factor; j++){
                std::string st = std::to_string(n[0]*factor+i) + "_";
                st += std::to_string(n[1]*factor+j);
                board_copy[st] = b.second;
            }
            
        }
        
    }

    board.clear();
    board = std::move(board_copy);

    // TODO: How is affected by negative numbers?
    min_i *= factor;
    min_j *= factor;

    max_i *= factor;
    max_j *= factor;
}

void FixedBoard::expandBoard(std::vector<std::vector<float>> kernel){
    std::unordered_map<std::string, float> board_copy;
    for(auto b: board){
        if(b.second >= wall_limit){
            board_copy[b.first] = 1.;
            continue;
        }
        std::stringstream ss(b.first);
        int n[2];
        int i=0;
        std::string num;
        while(std::getline(ss, num, '_')){
            n[i++] = std::stoi(num);
        }
        auto neigh = getNeighbours(n[0], n[1]);

        board_copy[b.first] = 
            kernel[0][0] * getValue(neigh[0]) + 
            kernel[0][1] * getValue(neigh[1]) +  
            kernel[0][2] * getValue(neigh[2]) + 
            kernel[1][0] * getValue(neigh[3]) +  
            kernel[1][1] * getValue(neigh[4]) +  
            kernel[1][2] * getValue(neigh[5]) + 
            kernel[2][0] * getValue(neigh[6]) +  
            kernel[2][1] * getValue(neigh[7]) +  
            kernel[2][2] * getValue(neigh[8]);
    }

    board.clear();
    board = std::move(board_copy);
}

float FixedBoard::getValue(std::string st){
    if(board.find(st) != board.end()){
        return board[st];
    }
    
    return unknown_cell;         // TODO: Should be variable, value for unknown cell
}

float FixedBoard::getValue(VehicleState &a){
    std::string st = std::to_string(a.x) + "_";
    st += std::to_string(a.y);
    return board[st];
}
    
std::vector<std::string> FixedBoard::getNeighbours(int i, int j){
    return {
        {std::to_string(i-1) + "_" + std::to_string(j-1)},
        {std::to_string(i-1) + "_" + std::to_string(j)},
        {std::to_string(i-1) + "_" + std::to_string(j+1)},
        {std::to_string(i) + "_" + std::to_string(j-1)},
        {std::to_string(i) + "_" + std::to_string(j)},
        {std::to_string(i) + "_" + std::to_string(j+1)},
        {std::to_string(i+1) + "_" + std::to_string(j-1)},
        {std::to_string(i+1) + "_" + std::to_string(j)},
        {std::to_string(i+1) + "_" + std::to_string(j+1)}
    };
}

void FixedBoard::addValue(std::string st, float v){
    board[st] = v;
}