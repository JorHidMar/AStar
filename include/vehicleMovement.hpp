#pragma once

#include "extras.hpp"

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

class SingleVehicleMovement : public VehicleMovement {
public:
    SingleVehicleMovement(VehicleConstrains &constrains_) : VehicleMovement(constrains_){
        compute_directions();
    }

    void compute_directions() {
        this->move_directions.clear();
        this->move_directions = {{1,0},{-1,0},{0,0,45},{0,0,-45}}; // Easier to keep track with degrees. // TODO: Better to keep internal calculation in degrees or move to radians?
    }

    VehicleState move(VehicleState &vs, const VehicleState &m) override { // TODO: Should it be static? Probably not..
        // TODO: This assumes that movements are the ones given in move_directions
        VehicleState state = vs;
        state.angle += m.angle;

        state.angle = state.angle % 360;

        if(state.angle <= -180){
            state.angle += 360;
        } else if(state.angle > 180){
            state.angle -= 360;
        }

        if(m.angle != 0) {
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
