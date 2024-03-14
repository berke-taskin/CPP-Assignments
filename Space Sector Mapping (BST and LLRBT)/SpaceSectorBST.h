#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <stack>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    std::stack<Sector*> pathStack;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* recursiveBST (Sector* root, int x, int y, int z);
    void inorderTraversal (Sector* root);
    void preorderTraversal (Sector* root);
    void postorderTraversal (Sector* root);
    void postorderDeleteTree (Sector* root);
    void postorderStellarPath (Sector* root, const std::string& sector_code);
    void postorderDeleteSector (Sector* root, const std::string& sector_code);
};

#endif // SPACESECTORBST_H
