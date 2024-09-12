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

    for(int i=0; i<H; ++i){
        for(int j=0; j<W; ++j){
            board.insert({VehicleState::convert2pos(i,j), 0.0});
        }
    }
}

void FixedBoard::loadBoard(const std::vector<std::vector<float>> &board_){
    board.clear();
    int i=0;
    for(const auto &row: board_){
        int j=0;
        for(const auto &col: row){
            if(col >= 0) {
                board.insert({VehicleState::convert2pos(i,j), col});
            }
            ++j;
        }
        ++i;
    }
    min_i = 0;
    min_j = 0;
    max_i = board_.size();
    max_j = board_[0].size();
}

void FixedBoard::loadBoard(const Board &board_){
    board.clear();
    board.insert(board_.begin(), board_.end());
}

void FixedBoard::loadFromFile(const std::string &filename){
    std::ifstream file(filename);

    min_i = 0;
    min_j = 0;
    max_i = 0;
    max_j = 0;

    if(file.is_open()){
        std::string line;
        int i=0;
        while (std::getline(file, line)) {
            int j=0;
            std::vector<int> row;
            std::stringstream ss(line);
            std::string value;
            
            bool empty_line = true;
            while (std::getline(ss, value, ',')) {
                float val = std::stof(value);
                if(val >= 0){
                    board.insert({VehicleState::convert2pos(i,j), val});
                    max_j = std::max(j+1, max_j);
                    empty_line = false;
                }
                ++j;
            }
            if(!empty_line){
                max_i = std::max(i+1, max_i);
            }
            ++i;
        }
        file.close();
    } else {
        std::cout << "otherwise" << std::endl;
    }
}

void FixedBoard::printBoard(){
    for(int i=min_i; i<max_i; ++i){
        for(int j=min_j; j<max_j; ++j){
            std::string st = VehicleState::convert2pos(i,j);

            if(board.find(st) != board.end()){
                std::cout << board[st] << "\t";
            } else {
                std::cout << "-\t";
            }
            
        }
        std::cout << std::endl;
    }
}

void FixedBoard::printBoardAndPath(const Path &path){
    Board board_copy(board);

    for(const auto &p : path){
        std::string st = p.str();
        board_copy[st] = 2.;
    }

    for(int i=min_i; i<max_i; ++i){
        for(int j=min_j; j<max_j; ++j){
            std::string st = VehicleState::convert2pos(i,j);

            if(board_copy.find(st) != board_copy.end()){
                std::cout << board_copy[st] << "\t";
            } else {
                std::cout << "-\t";
            }
            
        }
        std::cout << std::endl;
    }
}

void FixedBoard::exportMap(const std::string &filename, uint factor){

    if(factor < 2){
        return;
    }

    Board board_copy;
    augmentBoard(factor, board_copy);

    std::ofstream imageFile(filename);

    imageFile << "P3\n" << factor * (max_j - min_j) << " " << factor * (max_i - min_i) << "\n255\n";

    for(int i=factor*min_i; i<factor*max_i; ++i){
        for(int j=factor*min_j; j<factor*max_j; ++j){
            std::string key = VehicleState::convert2pos(i,j);
            float value;
            if(board_copy.find(key) != board_copy.end()){
                value =  board_copy[key];
                auto v = value * 255.;
                if(value > 1.2){
                    imageFile << 0 << " " << 0 << " " << (int)v << " ";
                } else {
                    imageFile << (int)v << " " << (int)v << " " << (int)v << " ";
                }
            } else {
                auto v = unknown_cell * 255.;
                imageFile << (int)v << " " << 0 << " " << 0 << " ";
            }
        }
        imageFile << "\n";
    }
    imageFile.close();
}

void FixedBoard::exportMapAndPath(const std::string filename, const std::vector<VehicleState> &path, uint factor){

    if(factor < 2){
        return;
    }

    Board board_copy;

    for(auto p : path){
        std::string st = p.str();
        board_copy[st] = 2.;
    }

    augmentBoard(factor, board_copy);

    for(auto &p: path){
        auto n = VehicleState::rConvert2pos(p.str());

        for(int i=0; i<factor; ++i){
            for(int j=0; j<factor; ++j){
                std::string st = VehicleState::convert2pos(n.x*factor+i, n.y*factor+j);
                board_copy[st] = 2.;
            }
            
        }
        
    }

    std::ofstream imageFile(filename);

    imageFile << "P3\n" << factor * (max_j - min_j) << " " << factor * (max_i - min_i) << "\n255\n";

    for(int i=factor*min_i; i<factor*max_i; ++i){
        for(int j=factor*min_j; j<factor*max_j; ++j){
            std::string key = VehicleState::convert2pos(i,j);
            float value;
            if(board_copy.find(key) != board_copy.end()){
                value =  board_copy[key];
                auto v = value * 255.;
                if(value > 1.2){
                    imageFile << 0 << " " << 0 << " " << (int)v << " ";
                } else {
                    imageFile << (int)v << " " << (int)v << " " << (int)v << " ";
                }
            } else {
                auto v = unknown_cell * 255.;
                imageFile << (int)v << " " << 0 << " " << 0 << " ";
            }
        }
        imageFile << "\n";
    }
    imageFile.close();
}

void FixedBoard::exportMapCSV(const std::string filename){
    std::ofstream file(filename);

    if (file.is_open()) {
        for(int i=min_i; i<max_i; ++i){
            for(int j=min_j; j<max_j; ++j){
                std::string st = VehicleState::convert2pos(i,j);

                if(board.find(st) != board.end()){
                    // std::cout << board[st] << "\t";
                    file << board[st];
                } else {
                    file << "-1";
                }
                if(j != max_j-1){
                    file << ",";
                }
                
            }
            file << "\n";
        } 
        file.close();
    }
}
    
bool FixedBoard::checkAvailable(VehicleState &a){
    std::string st = a.str();
    if(board.find(st) != board.end()){
        if(board[st] < wall_limit){
            return true;
        }
    }
    return false;
}

void FixedBoard::augmentBoard(uint factor, Board &board_copy){

    for(const auto &b: board){
        auto n = VehicleState::rConvert2pos(b.first);

        for(int i=0; i<factor; ++i){
            for(int j=0; j<factor; ++j){
                std::string st = VehicleState::convert2pos(n.x*factor+i, n.y*factor+j);
                board_copy[st] = b.second;
            }
            
        }
        
    }
}

void FixedBoard::updateAugmentBoard(uint factor){

    if(factor < 2){
        return;
    }

    Board board_copy;
    augmentBoard(factor, board_copy);

    board.clear();
    board = std::move(board_copy);

    min_i *= factor;
    min_j *= factor;

    max_i *= factor;
    max_j *= factor;
}

void FixedBoard::expandBoard(const std::vector<std::vector<float>> &kernel){
    int kSize = kernel.size()/2;

    Board board_copy;
    for(const auto &b: board){
        if(b.second >= wall_limit){
            board_copy[b.first] = 1.;
            continue;
        }
        auto n = VehicleState::rConvert2pos(b.first);
        board_copy[b.first] = 0.;
        // TODO: Find better alternative that can better suit larger kernels. Although I would say that kernels of size 3x3 - 5x5 should be enough.  
        for(int i=-kSize; i<=kSize; ++i){
            for(int j=-kSize; j<=kSize; ++j){
                std::string s = VehicleState::convert2pos(n.x+i, n.y+j);
                board_copy[b.first] += kernel[i+kSize][j+kSize] * getValue(s);
            }
        }
        
    }

    board.clear();
    board = std::move(board_copy);
}

float FixedBoard::getValue(const std::string &st){
    if(board.find(st) != board.end()){
        return board[st];
    }
    return unknown_cell;
}

float FixedBoard::getValue(VehicleState &a){
    return board[a.str()];
}

void FixedBoard::addValue(const std::string &st, float v){
    auto n = VehicleState::rConvert2pos(st);
    min_i = std::min(n.x, min_i);
    min_j = std::min(n.y, min_j);
    max_i = std::max(n.x, max_i);
    max_j = std::max(n.y, max_j);
    board[st] = v;
}


void FixedBoard::updateUnknownCells(float unknown_cell_){
    unknown_cell = unknown_cell_;
}

void FixedBoard::updateWallValue(float wall_limit_){
    wall_limit = wall_limit_;
}

void FixedBoard::getBoard(Board &board_){
    board_.clear();
    board_.insert(board.begin(), board.end());
}
