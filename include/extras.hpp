#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <unordered_map>

/**
 * @brief Structure to define the pose and twist of a vehicle.
 */
struct VehicleState
{
    // Pose
    int x=0;
    int y=0;
    int angle=0;

    // Twist
    float v=0;
    float w=0;

    float dt=0;

    void operator=(const VehicleState &vs){
        x = vs.x;
        y = vs.y;
        angle = vs.angle;
        v = vs.v;
        w = vs.w;
    }

    // TODO: You might not care about the angle for final position
    bool operator==(VehicleState &p){
        return x == p.x && y == p.y && angle == p.angle;
    }

    VehicleState operator+(const VehicleState &vs){
        VehicleState r;
        r.x = x + vs.x;
        r.y = y + vs.y;
        r.angle = vs.angle;
        r.v = vs.v;
        r.w = vs.w;
        return r;
    }

    void print(){
        std::cout << "Position: " << x << ", " << y << ", " << angle << std::endl;
        std::cout << "Speed: " << v << ", " << w << std::endl;
    }

    /**
     * @brief Convert the vehicleState to a string with the position in the map.
     */
    std::string str() const { // TODO you should be able to change this function when defining class
        std::string key = std::to_string(x) + "_" + std::to_string(y); 
        return key;  
    }

    /**
     * @brief Convert the vehicleState to a string with the state.
     */
    std::string convert2state(){
        std::string key = std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(angle); 
        return key;  
    }
    
};

/**
 * @brief Structure to define constrains of the vehicle.
 * @note To be integrated, used to customize algorithm to vehicle constrains
 */
struct VehicleConstrains
{
    // Size
    int width;
    int lenght;

    // Dynamic constraints
    float max_v;
    float max_w;

    // Robot's center
    float x;
    float y;

    float fv; // Factor to step the speed into smaller ones
    float fw; // Factor to step the angular spped into smaller ones

    void operator=(const VehicleConstrains &vc){
        width = vc.width;
        lenght = vc.lenght;
        max_v = vc.max_v;
        max_w = vc.max_w;
        x = vc.x;
        y = vc.y;
    }
    
};

/**
 * @brief Structure to define the state of the vehicle
 */
struct State
{
    VehicleState p;

    float g=0;              // Cost
    float h=0;              // Heuristic
    float f=0;

    VehicleState parent;
    bool start = false;

    void operator=(State s){
        p = s.p;
        g = s.g;
        h = s.h;
        f = s.f;
        parent = s.parent;
        start = s.start;
    }
    bool operator< (const State &a) const {
        return a.f > f;
    }

    bool operator== (const State &a) const {
        return p.x == a.p.x && p.y == a.p.y && p.angle == a.p.angle;
    }

    void print(){
        std::cout << g << ", " << h << ", " << f << ", Pos: ";
        p.print();
    }
};

typedef std::vector<VehicleState> Path;
