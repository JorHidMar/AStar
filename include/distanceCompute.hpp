#pragma once

#include "extras.hpp"

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
        return (1 + f*cost) * (std::abs(b.x-a.x) + std::abs(b.y-a.y));
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
 * @brief Compute euclidean distance and time to move.
 */
class timeDistance : public computeDistance {
public:
    timeDistance(float f_=1.5) : computeDistance(f_){}

    float compute(VehicleState &a, VehicleState &b, float cost=0.) override {
        return (1+this->f*cost) * (1 + std::sqrt(std::pow(b.x-a.x, 2) + std::pow(b.y-a.y, 2)));
    }
};

/**
 * @brief Compute euclidean distance and time to move for multiple steps
 */
class multiTimeDistance : public computeDistance {
public:
    multiTimeDistance(float f_=1.5) : computeDistance(f_){}

    float compute(VehicleState &a, VehicleState &b, float cost=0.) override {
        return (1+this->f*cost) * (std::max(std::abs(b.x-a.x),std::abs(b.y-a.y)) + std::sqrt(std::pow(b.x-a.x, 2) + std::pow(b.y-a.y, 2)));
    }
};
