#include "board.hpp"

FixedBoard::FixedBoard(float wall_limit_, float unknown_cell_) : wall_limit(wall_limit_) {
    board.unknown_cell = unknown_cell_;
}

FixedBoard::~FixedBoard(){

}

void FixedBoard::createEmptyBoard(int H, int W){
    board.createEmptyBoard(H, W);
}

void FixedBoard::loadBoard(const std::vector<std::vector<float>> &board_){
    board.clear();
    board.insertMatrix(board_);

}

void FixedBoard::loadBoard(Board &board_){
    board=board_;
}

void FixedBoard::loadFromFile(const std::string &filename){
    std::ifstream file(filename);

    if(file.is_open()){
        board.clear();
        std::string line;
        int i=0;
        while (std::getline(file, line)) {
            int j=0;
            std::stringstream ss(line);
            std::string value;
            
            bool empty_line = true;
            while (std::getline(ss, value, ',')) {
                float val = std::stof(value);
                if(val >= 0){
                    board.insert(j, i, val);
                }
                ++j;
            }
            ++i;
        }
        file.close();
    }
}

void FixedBoard::printBoard(){

    auto xrange = board.getWidthRange();
    auto yrange = board.getHeightRange();

    for(int i=yrange.first; i<=yrange.second; ++i){
        for(int j=xrange.first; j<=xrange.second; ++j){

            if(board.find(j, i)){
                std::cout << board.get(j, i) << "\t";
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
        board_copy.insert(st, 2.);
    }

    auto xrange = board.getWidthRange();
    auto yrange = board.getHeightRange();

    for(int i=yrange.first; i<=yrange.second; ++i){
        for(int j=xrange.first; j<=xrange.second; ++j){

            if(board_copy.find(j, i)){
                std::cout << board_copy.get(j,i) << "\t";
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

    std::ofstream imageFile(filename);

    if(!imageFile.is_open()){
        return;
    }


    auto xrange = board.getWidthRange();
    auto yrange = board.getHeightRange();

    imageFile << "P3\n" << factor * (xrange.second - xrange.first + 1) << " " << factor * (yrange.second - yrange.first + 1) << "\n255\n";

    // TODO: Unknown could be separate value
    for(int i=yrange.first; i<=yrange.second; ++i){
        for(int k=0; k<factor; ++k){
            for(int j=xrange.first; j<=xrange.second; ++j){
                float value = board.get(j, i);
                int v = value * 255.;
                bool empty_value = !board.find(j, i);
                for(int l=0; l<factor; ++l){
                    if(empty_value){
                        imageFile << 255 << " " << 0 << " " << 0 << " ";
                    } else {
                        if(value > 1.2){
                            imageFile << 0 << " " << 0 << " " << (int)v << " ";
                        } else {
                            imageFile << (int)v << " " << (int)v << " " << (int)v << " ";
                        }
                    }
                }
            }
            imageFile << "\n";
        }
    }

    imageFile.close();
}

void FixedBoard::exportMapAndPath(const std::string filename, const std::vector<VehicleState> &path, uint factor){

    if(factor < 2){
        return;
    }

    std::ofstream imageFile(filename);

    if(!imageFile.is_open()){
        return;
    }

    Board board_copy = board;

    for(auto p : path){
        board_copy.insert(p.str(), 2.);
    }


    auto xrange = board.getWidthRange();
    auto yrange = board.getHeightRange();

    imageFile << "P3\n" << factor * (xrange.second - xrange.first + 1) << " " << factor * (yrange.second - yrange.first + 1) << "\n255\n";

    for(int i=yrange.first; i<=yrange.second; ++i){
        for(int k=0; k<factor; ++k){
            for(int j=xrange.first; j<=xrange.second; ++j){
                float value = board_copy.get(j, i);
                int v = value * 255.;
                bool empty_value = !board_copy.find(j, i);
                for(int l=0; l<factor; ++l){
                    if(empty_value){
                        imageFile << 255 << " " << 0 << " " << 0 << " ";
                    } else {
                        if(value > 1.2){
                            imageFile << 0 << " " << 0 << " " << (int)v << " ";
                        } else {
                            imageFile << (int)v << " " << (int)v << " " << (int)v << " ";
                        }
                    }
                }
            }
            imageFile << "\n";
        }
    }

    imageFile.close();

}

void FixedBoard::exportMapCSV(const std::string filename){
    std::ofstream file(filename);

    auto xrange = board.getWidthRange();
    auto yrange = board.getHeightRange();

    if (file.is_open()) {
        for(int i=yrange.first; i<=yrange.second; ++i){
            for(int j=xrange.first; j<=xrange.second; ++j){

                if(board.find(j,i)){
                    file << board.get(j,i);
                } else {
                    file << "-1";
                }
                if(j != xrange.second){
                    file << ",";
                }
                
            }
            file << "\n";
        } 
        file.close();
    }
}
    
bool FixedBoard::checkAvailable(VehicleState &a){
    if(board.find(a.str())){
        if(board.get(a.str()) < wall_limit){
            return true;
        }
    }
    return false;
}

void FixedBoard::expandBoard(const std::vector<std::vector<float>> &kernel){
    int kSize = kernel.size()/2;

    Board board_copy = board;
    for(const auto &b: board.board){
        if(b.second >= wall_limit){
            board_copy.insert(b.first, 1.);
            continue;
        }

        auto n = Board::getPosition(b.first);

        float new_value = 0.;
        // TODO: Find better alternative that can better suit larger kernels. Although I would say that kernels of size 3x3 - 5x5 should be enough.  
        for(int i=-kSize; i<=kSize; ++i){
            for(int j=-kSize; j<=kSize; ++j){
                new_value += kernel[i+kSize][j+kSize] * board.get(n.first+i, n.second+j);
            }
        }

        board_copy.insert(b.first, new_value);
        
    }

    board = board_copy;
}

float FixedBoard::getValue(const std::string &st){
    return board.get(st);
    
}

float FixedBoard::getValue(VehicleState &a){
    return board.get(a.str());
}

void FixedBoard::addValue(const std::string &st, float v){
    board.insert(st, v);
}

void FixedBoard::addValue(int x, int y, float v){
    board.insert(x, y, v);
}

void FixedBoard::removeValue(int x, int y){
    board.remove(x, y);
}

void FixedBoard::removeValue(const std::string &st){
    board.remove(st);
}

void FixedBoard::updateUnknownCells(float unknown_cell_){
    board.unknown_cell = unknown_cell_;
}

void FixedBoard::updateWallValue(float wall_limit_){
    wall_limit = wall_limit_;
}

void FixedBoard::getBoard(Board &board_){
    board_.clear();
    board_ = board;
}
