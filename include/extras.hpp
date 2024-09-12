#pragma once

// #include "astar.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

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

    std::string str(){ // TODO you should be able to change this function when defining class
        std::string key = std::to_string(x) + "_" + std::to_string(y); 
        return key;  
    }

    std::string convert2state(){
        std::string key = std::to_string(x) + "_" + std::to_string(y); 
        return key;  
    }

    // TODO: better to use a vector or something else? But what if im not interested in others?
    static std::string convert2pos(int i, int j){
        std::string key = std::to_string(i) + "_" + std::to_string(j); 
        return key;  
    }

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
 * @brief Derived class to compute distance. 
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
 * @brief Derived class to compute distance. 
 */
class computeTimeDistance : public computeDistance {
public:
    computeTimeDistance(float f_=1.5) : computeDistance(f_){}

    /**
     * @brief compute time difference between VehicleState a to b.
     */
    float compute(VehicleState &a, VehicleState &b, float cost=0.) override {
        return (b.dt - a.dt) * (1 + this->f * cost);    // TODO: dt does not seem to be the best variable name
    }
};

/**
 * @brief Base class to define vehicle's movement. 
 */
class VehicleMovement { //TODO: Review names for derived classes and the description

    public:

    VehicleMovement(VehicleConstrains &constrains_) : constrains(constrains_){}

    /**
     * @brief Compute the movement, add vehicleState m to vs.
     * @return Resulting state.
     */
    virtual VehicleState move(VehicleState &vs, VehicleState &m){

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
 * @brief Derived class to define vehicle's movement for 8 directions
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

/**
 * @brief Derived class to define vehicle's movement, single action
 */
class SingleActionVehicleMovement : public VehicleMovement{
public:
    SingleActionVehicleMovement(VehicleConstrains constrains_) : VehicleMovement(constrains_){
        compute_directions();
    }

    void compute_directions() {
        this->move_directions.clear();
        this->move_directions = {{1,0},{-1,0},{0,0,45},{0,0,-45}}; // Easier to keep track with degrees. // TODO: Better to keep internal calculation in degrees or move to radians?
    }

    VehicleState move(VehicleState &vs, VehicleState &m) override { // TODO: Should it be static? Probably not..
        // TODO: This assumes that movements are the ones given in move_directions
        VehicleState state = vs;
        state.angle += m.angle;

        if(state.angle <= -180){
            state.angle += 360;
        } else if(state.angle > 180){
            state.angle -= 360;
        }

        if(m.angle == 0) {
            return state;
        }

        switch (state.angle){              // TODO: Unnecessary when m.angle != 0
            case -135:
                state.x -= m.x;
                state.y += m.x;       // Board should be inverted at some point to avoid this
                break;
            case -90:
                state.y += m.x;
                break;
            case -45:
                state.x += m.x;
                state.y += m.x;
                break;
            case 0:
                state.x += m.x;
                break;
            case 45:
                state.x += m.x;
                state.y -= m.x;
                break;
            case 90:
                state.y -= m.x;
                break;
            case 135:
                state.x -= m.x;
                state.y -= m.x;
                break;
            case 180:
                state.x -= m.x;
                break;
            default:
                break;
        }

        return state;
    }
};

/**
 * @brief Derived class to define vehicle's movement for differential robot.
 * @note To be integrated. 
 */
class DifferentialRobotMovement : public VehicleMovement {
    public:

    DifferentialRobotMovement(VehicleConstrains &constrains_, float dt) : VehicleMovement(constrains_) {
        compute_directions();
    }

    /**
     * @brief Update move_directions to get the movement direction map.
     */
    void compute_directions() {
        this->move_directions.clear();
        float dt = 0.1;
        int fv = this->constrains.fv;
        int fw = this->constrains.fw;
        for(int i=0; i<=fv; i++){
            float v = (float)i * this->constrains.max_v / (float)fv;
            for(int j=-fw; j<=fw; j++){
                float w = (float)j * this->constrains.max_v / (float)fw;
                this->move_directions.push_back({0,0,0,v,w,dt});
            }
        }
    }

    /**
     * @brief Compute the movement, add vehicleState m to vs.
     * @return Resulting state.
     */
    VehicleState move(VehicleState &vs, VehicleState &m) override {

        VehicleState state;

        state.x = vs.x + m.x;
        state.y = vs.y + m.y;

        float dt = m.dt;

        state.v = this->applyConst(m.v, this->constrains.max_v);
        state.w = this->applyConst(m.w, this->constrains.max_w);

        if(std::abs(state.w) < 0.01){
            state.x = (state.v / -0.0001) * (-std::sin(vs.angle) + std::sin(vs.angle - 0.0001 * dt));
            state.y = (state.v / -0.0001) * (std::cos(vs.angle) - std::cos(vs.angle - 0.0001 * dt));
        } else {
            state.x = state.v / -state.w * (-std::sin(vs.angle) + std::sin(vs.angle - state.w * dt));
            state.y = state.v / -state.w * (std::cos(vs.angle) - std::cos(vs.angle - state.w * dt));
        }
        state.angle = this->normalizeAngle(vs.angle - state.w * dt); // TODO: Find better way, case for high w
        // state.angle = vs.angle + state.w * dt;

        return state;
    }
};
