#include "boardContainer.hpp"

// Should not be here
void Board::createEmptyBoard(int H, int W){
    board.clear();
    width.first = 0;
    width.second = W-1;
    height.first = 0;
    height.second = H-1;

    for(int i=0; i<H; ++i){
        for(int j=0; j<W; ++j){                
            std::string key = std::to_string(j) + "_" + std::to_string(i); 
            board.insert({key, 0.0});
        }
    }

    for(int i=0; i<H; i++){
        yRange[i] = W;
    }
    for(int i=0; i<W; i++){
        xRange[i] = H;
    }
}

void Board::insertMatrix(std::vector<std::vector<float>> m){
    int H = m.size();
    int W = m[0].size();

    for(int i=0; i<H; i++){
        yRange[i] = W;
    }
    for(int i=0; i<W; i++){
        xRange[i] = H;
    }

    width = {0, W-1};
    height = {0, H-1};

    int i=0;
    for(const auto &row: m){
        int j=0;
        for(const auto &col: row){
            if(col >= 0) {
                std::string key = std::to_string(j) + "_" + std::to_string(i); 
                board.insert({key, col});
            }
            ++j;
        }
        ++i;
    }
}

void Board::insert(int x, int y, float value){
    std::string key = std::to_string(x) + "_" + std::to_string(y);
    if(board.find(key) == board.end()){

        if(board.empty()){
            width = {x,x};
            height = {y,y};
        }

        board.insert({key, value});

        if(width.first > x){
            width.first = x;
        } else if(width.second < x){
            width.second = x;
        }

        if(height.first > y){
            height.first = y;
        } else if(height.second < y){
            height.second = y;
        }
        
        if(xRange.find(x) == xRange.end()){
            xRange[x] = 1;
        } else {
            xRange[x] += 1;
        }

        if(yRange.find(y) == yRange.end()){
            yRange[y] = 1;
        } else {
            yRange[y] += 1;
        }
    } else {
        board[key] = value;
    }

}

void Board::insert(const std::string &str, float value){
        
    std::stringstream ss(str);
    std::vector<int> n;
    std::string num;
    while(std::getline(ss, num, '_')){
        n.push_back(std::stoi(num));
    }
    int x = n[0];
    int y = n[1];
            
    if(board.find(str) == board.end()){

        if(board.empty()){
            width = {x,x};
            height = {y,y};
        }

        board.insert({str, value});

        if(width.first > x){
            width.first = x;
        } else if(width.second < x){
            width.second = x;
        }

        if(height.first > y){
            height.first = y;
        } else if(height.second < y){
            height.second = y;
        }

        if(xRange.find(x) == xRange.end()){
            xRange[x] = 1;
        } else {
            xRange[x] += 1;
        }

        if(yRange.find(y) == yRange.end()){
            yRange[y] = 1;
        } else {
            yRange[y] += 1;
        }
        
    } else {
        board[str] = value;
    }

}

void Board::remove(int x, int y){
    std::string key = std::to_string(x) + "_" + std::to_string(y);
    if(board.find(key) == board.end()){
        return;
    }
    board.erase(key);

    if(board.empty()){
        xRange.clear();
        yRange.clear();
        width = {0,0};
        height = {0,0};
        return;
    }

    xRange[x] -= 1;
    yRange[y] -= 1;

    if(xRange[x] <= 0){
        xRange.erase(x);
        if(x == width.first){
            int new_x = xRange.begin()->first;
            width.first = new_x;
        } else if(x == width.second) {
            int new_x = std::prev(xRange.end())->first;
            width.second = new_x;
        }
    }

    if(yRange[y] <= 0){
        yRange.erase(y);
        if(y == height.first){
            int new_y = yRange.begin()->first;
            height.first = new_y;
        } else if(y == height.second) {
            int new_y = std::prev(yRange.end())->first;
            height.second = new_y;
        }
    }
}

void Board::remove(const std::string &str){
    if(board.find(str) == board.end()){
        return;
    }
    board.erase(str);

    if(board.empty()){
        xRange.clear();
        yRange.clear();
        width = {0,0};
        height = {0,0};
        return;
    }

    std::stringstream ss(str);
    std::vector<int> n;
    std::string num;
    while(std::getline(ss, num, '_')){
        n.push_back(std::stoi(num));
    }
    int x = n[0];
    int y = n[1];

    xRange[x] -= 1;
    yRange[y] -= 1;

    if(xRange[x] <= 0){
        xRange.erase(x);
        if(x == width.first){
            int new_x = xRange.begin()->first;
            width.first = new_x;
        } else if(x == width.second) {
            int new_x = std::prev(xRange.end())->first;
            width.second = new_x;
        }
    }

    if(yRange[y] <= 0){
        yRange.erase(y);
        if(y == height.first){
            int new_y = yRange.begin()->first;
            height.first = new_y;
        } else if(y == height.second) {
            int new_y = std::prev(yRange.end())->first;
            height.second = new_y;
        }
    }
}

void Board::operator=(Board &b){
    board.clear();
    board.insert(b.board.begin(), b.board.end());
    width = b.width;
    height = b.height;
    xRange.clear();
    yRange.clear();
    xRange.insert(b.xRange.begin(), b.xRange.end());
    yRange.insert(b.yRange.begin(), b.yRange.end());
}

float Board::operator[](const std::string &str){
    if(board.find(str) != board.end()){
        return board[str];
    }
    return unknown_cell;
}

std::pair<int,int> Board::getWidthRange(){
    return width;
}

std::pair<int,int> Board::getHeightRange(){
    return height;
}

void Board::clear(){
    board.clear();
    width = {0,0};
    height = {0,0};
    xRange.clear();
    yRange.clear();
}

float Board::get(int x, int y){
    std::string key = std::to_string(x) + "_" + std::to_string(y); 
    if(board.find(key) != board.end()){
        return board[key];
    }
    return unknown_cell;
}

float Board::get(const std::string &str){
    if(board.find(str) != board.end()){
        return board[str];
    }
    return unknown_cell;
}

bool Board::find(int x, int y){
    std::string key = std::to_string(x) + "_" + std::to_string(y); 
    if(board.find(key) != board.end()){
        return true;
    }
    return false;
}

bool Board::find(const std::string &str){
    if(board.find(str) != board.end()){
        return true;
    }
    return false;
}

bool Board::empty(){
    return board.empty();
}
