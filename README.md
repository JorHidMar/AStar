# A* search algorithm

A* Search Algorithm in C++ using Standard Libraries.

## Installation

### Build instructions

    mkdir -p build && cd build
    cmake ..
    make

### Installing the package (Optional)

You can also create a `.deb` package using `cpack`:

    cpack
    sudo dpkg -i *.deb

## Functionalities

### Map
* Import a map from a CSV file. 
* Export the map to a CSV or image file.
* Expand obstacles in the map.
* Augment the size of the map.
* Support irregular and incomplete maps.
* Support for dynamic maps (under development).

### A* Algorithm
* Easy to customize cost and heuristic functions.
* Easy to customize vehicle's movement.
* Support for adding vehicle constraints (under development).

## Examples

Load board

    std::shared_ptr<FixedBoard> b = std::make_shared<FixedBoard>(0.95, 0.5);
    b->loadFromFile("../examples/example_map");
    
Apply kernel 

    std::vector<std::vector<float>> kernel_3x3 = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };
    b->expandBoard(kernel_3x3);

Augment map

    b->updateAugmentBoard(2);

Export map

    b->exportMap("output_image.ppm", 20);
    b->exportMapCSV("saved_map");

Configure A* algorithm

    AStar astar(b);
    
    VehicleConstrains constrains;
    std::shared_ptr<computeDistance> computeClass = std::make_shared<computeEuclideanDistance>();
    std::shared_ptr<VehicleMovement> vMove = std::make_shared<MultiDirectionVehicleMovement>(constrains);

    astar.setComputeCost(computeClass);
    astar.setComputeHeuristic(computeClass);
    astar.setVehicle(vMove);
    
    VehicleState iState = {0,0};
    VehicleState fState = {20,24};
    astar.setGoal(fState);
    
    astar.compute(iState, fState);

Get path

    std::vector<VehicleState> path;
    astar.getBestPath(path);

## Customization

Cost function

    class CustomDistance : public computeDistance {
    public:
    
        CustomDistance(float f_=1.5) : computeDistance(f_){}
    
        float compute(VehicleState &a, VehicleState &b, float cost=0.) override {
            // Implementation
        }
    };

Vehicle movement

    class CustomVehicleMovement : public VehicleMovement{
    public:
        CustomVehicleMovement(VehicleConstrains constrains_) : VehicleMovement(constrains_){
            compute_directions();
        }
    
        void compute_directions() {
            // Precompute movement of vehicle
        }
    
        VehicleState move(VehicleState &vs, VehicleState &m) override { 
            // How is next position calculated
        }
    };
