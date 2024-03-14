#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <stack>

class SpaceSectorLLRBT {
public:
    Sector* root;
    std::stack<Sector*> pathStack;
    std::vector<Sector*> earthPath;
    std::vector<Sector*> sectorPath;
    bool bool1=false;
    bool bool2=false;
    bool once=true;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* recursiveLLRBT (Sector* root, int x, int y, int z);
    Sector* leftRotation (Sector* root);
    Sector* rightRotation (Sector* root);
    void inorderTraversal (Sector* root);
    void preorderTraversal (Sector* root);
    void postorderTraversal (Sector* root);
    void postorderDeleteTree(Sector* root);
    int postorderStellarPath(Sector* root, const std::string& sector_code);
    void pathFindingChoice(Sector* root, const std::string& sector_code, int where, int choice, std::string sector_code2);
    int findingEarth(Sector* root);
};

#endif // SPACESECTORLLRBT_H
