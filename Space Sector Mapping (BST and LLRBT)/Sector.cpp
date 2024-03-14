#include "Sector.h"
#include <cmath>
#include <iostream>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code

        //Calculating Distance
        double distance = sqrt(((x-0)*(x-0)) + ((y-0)*(y-0)) + ((z-0)*(z-0)));
        distance_from_earth = distance;
        std::string xcode, ycode, zcode, sectorcode;
        int floor = static_cast<int>(distance);
        sectorcode += std::to_string(floor);

        
        //Generating X Code
        if (x>0){
            xcode = "R";
        }
        else if (x==0){
            xcode = "S";
        }
        else if (x<0){
            xcode = "L";
        }
        sectorcode += xcode;

        //Generating X Code
        if (y>0){
            ycode = "U";
        }
        else if (y==0){
            ycode = "S";
        }
        else if (y<0){
            ycode = "D";
        }
        sectorcode += ycode;

        //Generating Z Code
        if (z>0){
            zcode = "F";
        }
        else if (z==0){
           zcode = "S";
        }
        else if (z<0){
            zcode = "B";
        }
        sectorcode += zcode;

        //Generating Sector Code
        sector_code = sectorcode;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        color = other.color;
        left = other.left;
        right = other.right;
        parent = other.parent;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}