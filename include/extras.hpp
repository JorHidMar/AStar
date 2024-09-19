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
     * @brief Convert the vehicleState to a string with the state.
     */
    std::string str() const { // TODO you should be able to change this function when defining class
        std::string key = std::to_string(x) + "_" + std::to_string(y); 
        return key;  
    }

    /**
     * @brief Convert the vehicleState to a string with the position for the map.
     */
    std::string convert2state(){
        std::string key = std::to_string(x) + "_" + std::to_string(y); 
        return key;  
    }

    /**
     * @brief Convert coordinates to string.
     */
    static std::string convert2pos(int i, int j){
        std::string key = std::to_string(i) + "_" + std::to_string(j); 
        return key;  
    }

    /**
     * @brief Convert string to coordinates.
     */
    static VehicleState rConvert2pos(const std::string &st){    // TODO: not necessary to return whole state
        VehicleState vs;
        std::stringstream ss(st);
        std::vector<int> n;
        int k=0;
        std::string num;
        while(std::getline(ss, num, '_')){
            n.push_back(std::stoi(num));
        }
        vs.x = n[0];
        vs.y = n[1];
        return vs;
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

/**
 * @brief Base class to compute distance. 
 */
class computeDistance {
public:
    computeDistance(float f_=1.5) : f(f_){}

    /**
     * @brief compute Manahattan distance between VehicleState a to b.
     */
    virtual float compute(VehicleState &a, VehicleState &b, float cost=0.){
        return (1 + 1.5*cost) * (std::abs(b.x-a.x) + std::abs(b.y-a.y));
    }
protected:
    float f;
};

/**
 * @brief Compute euclidean distance.
 */
class computeEuclideanDistance : public computeDistance {
public:

    computeEuclideanDistance(float f_=1.5) : computeDistance(f_){}

    /**
     * @brief compute euclidean distance between VehicleState a to b.
     */
    float compute(VehicleState &a, VehicleState &b, float cost=0.) override {
        return (1+this->f*cost) * std::sqrt(std::pow(b.x-a.x, 2) + std::pow(b.y-a.y, 2)); 
    }
};

/**
 * @brief Base class to define vehicle's movement. 
 */
class VehicleMovement {

    public:

    VehicleMovement(VehicleConstrains &constrains_) : constrains(constrains_){}

    /**
     * @brief Compute the movement, add vehicleState m to vs.
     * @return Resulting state.
     */
    virtual VehicleState move(VehicleState &vs, const VehicleState &m){

        VehicleState state;

        state.x = vs.x + m.x;
        state.y = vs.y + m.y;

        return state;
    }

    void print(){
        for(auto d: move_directions){
            std::cout << "Pos: " << d.x << " " << d.y << ", Speed: " << d.v << " " << d.w << ", Time: " << d.dt << std::endl;
        }
    }

    std::vector<VehicleState> move_directions = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    VehicleConstrains constrains;

protected:

    /**
     * @brief Check if value is within range.
     * @param a Value that we have.
     * @param m Maximum value we can have.
     * @return Corrected value.
     */
    float applyConst(float a, float m){
        return (std::abs(a) <= m) ? a : (a <= 0 ? -m : m);
    }

    /**
     * @brief Angle between [-pi, pi)
     */
    float normalizeAngle(float ang){ // 
        float a=ang;
        if(ang > M_PI ){
            a -= 2*M_PI;
        } else if(ang < -M_PI){
            a += 2*M_PI;
        }

        return a;
    }
};

/**
 * @brief Vehicle movement for 8 directions
 */
class MultiDirectionVehicleMovement : public VehicleMovement {
public:
    MultiDirectionVehicleMovement(VehicleConstrains &constrains_) : VehicleMovement(constrains_){
        compute_directions();
    }

    void compute_directions(){
        this->move_directions.clear();
        this->move_directions = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    }

};

typedef std::vector<VehicleState> Path;
