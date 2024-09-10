#include "board.hpp"

FixedBoard::FixedBoard(float wall_limit_, float unknown_cell_) : wall_limit(wall_limit_), unknown_cell(unknown_cell_) {

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

void FixedBoard::exportMap(const std::string filename, uint factor){

    // TODO: This should be divided into two, all this is recopied from before
    if(factor < 2){
        return;
    }

    std::unordered_map<std::string, float> board_copy;
    augmentBoard(factor, board_copy);

    std::ofstream imageFile(filename);

    imageFile << "P2\n" << factor * (max_j - min_j) << " " << factor * (max_i - min_i) << "\n255\n";

    for(int i=factor*min_i; i<factor*max_i; i++){
        for(int j=factor*min_j; j<factor*max_j; j++){
            std::string key = std::to_string(i) + "_";
            key += std::to_string(j);
            float value;
            if(board_copy.find(key) != board_copy.end()){
                value =  board_copy[key];
            } else {
                value = unknown_cell;
            }
            auto v = value * 255.;
            imageFile << (int)v << " ";
        }
        imageFile << "\n";
    }
    imageFile.close();
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

void FixedBoard::augmentBoard(uint factor, std::unordered_map<std::string, float> &board_copy){ // TODO: Move to private

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
}

void FixedBoard::updateAugmentBoard(uint factor){

    if(factor < 2){
        return;
    }

    std::unordered_map<std::string, float> board_copy;
    augmentBoard(factor, board_copy);

    board.clear();
    board = std::move(board_copy);

    min_i *= factor;
    min_j *= factor;

    max_i *= factor;
    max_j *= factor;
}

void FixedBoard::expandBoard(std::vector<std::vector<float>> &kernel){
    int kSize = kernel.size()/2;

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
        board_copy[b.first] = 0.;
        // TODO: Find better alternative that can better suit larger kernels. Although I would say that kernels of size 3x3 - 5x5 should be enough.  
        for(int i=-kSize; i<=kSize; i++){
            for(int j=-kSize; j<=kSize; j++){
                std::string s = std::to_string(n[0]+i) + "_" + std::to_string(n[1]+j);
                board_copy[b.first] += kernel[i+kSize][j+kSize] * getValue(s);
            }
        }
        
    }

    board.clear();
    board = std::move(board_copy);
}

float FixedBoard::getValue(std::string st){
    if(board.find(st) != board.end()){
        return board[st];
    }
    return unknown_cell;
}

float FixedBoard::getValue(VehicleState &a){
    std::string st = std::to_string(a.x) + "_";
    st += std::to_string(a.y);
    return board[st];
}

void FixedBoard::addValue(std::string st, float v){
    board[st] = v;
}