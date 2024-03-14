#include "SpaceSectorBST.h"
#include <iostream>
#include <stack>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    postorderDeleteTree(root);
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream inputFile(filename);
    bool once=true;
    while (inputFile.is_open()){
        std::string line;;
        while (getline(inputFile, line)){
            if (!once){
                //Creating Sector
                std::string strx, stry, strz; int intx, inty, intz;
                std::istringstream iss(line);
                getline(iss, strx, ','); getline(iss, stry, ','); getline(iss, strz, ',');
                intx = std::stoi(strx); inty = std::stoi(stry); intz = std::stoi(strz);
                
                insertSectorByCoordinates(intx, inty, intz);
            }   
            once=false;   
        }
        inputFile.close();
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    if (root == nullptr){
        root = recursiveBST (root, x, y, z);
    } else {
        recursiveBST (root, x, y, z);
    }
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    postorderDeleteSector(root,sector_code);
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postorderTraversal(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    postorderStellarPath(root,sector_code);
    while (!pathStack.empty()){
        Sector* sector = pathStack.top();
        path.push_back(sector);
        pathStack.pop();
    }
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if (path.empty()){
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
    } else {
        //Printing sectors in vector path
        std::cout << "The stellar path to Dr. Elara: ";
        int count = 1;
        for (const auto& sector : path) {
            std::cout << sector->sector_code;
            if (count!=path.size()){
                std::cout << "->"; 
            }
            count++;
        }
        std::cout << std::endl;
    }
    
}

Sector* SpaceSectorBST::recursiveBST(Sector* root, int x, int y, int z){
    if (root == nullptr) {
        Sector* sector = new Sector(x, y, z);
        return sector;
    }

    //If X is greater
    if (x > root->x){
        root->right = recursiveBST(root->right, x, y, z);
        root->right->parent = root;
    }
    //If X is smaller
    else if (x < root->x){
        root->left = recursiveBST(root->left, x, y, z);
        root->left->parent = root;
    } 
    //If X is equal
    else {
        //If Y is greater
        if (y > root->y){
            root->right = recursiveBST(root->right, x, y, z);
            root->right->parent = root;
        }
        //If Y is smaller
        else if (y < root->y){
            root->left = recursiveBST(root->left, x, y, z);
            root->left->parent = root;
        } 
        //If Y is equal 
        else {
            //If Z is greater
            if (z > root->z){
                root->right = recursiveBST(root->right, x, y, z);
                root->right->parent = root;
            }
            //If Z is smaller
            else if (z < root->z){
                root->left = recursiveBST(root->left, x, y, z);
                root->left->parent = root;
            } else {
                return root;
            }
        }
    }

    return root;
}

void SpaceSectorBST::inorderTraversal(Sector* root){
    if (root != nullptr) {
        inorderTraversal(root->left);
        std::cout << root->sector_code << std::endl;
        inorderTraversal(root->right);
    }
}

void SpaceSectorBST::preorderTraversal(Sector* root){
    if (root != nullptr) {
        std::cout << root->sector_code << std::endl;
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void SpaceSectorBST::postorderTraversal(Sector* root){
    if (root != nullptr) {
        postorderTraversal(root->left);    
        postorderTraversal(root->right);
        std::cout << root->sector_code << std::endl;
    }
}

void SpaceSectorBST::postorderDeleteTree(Sector* root){
    if (root != nullptr) {
        postorderDeleteTree(root->left);    
        postorderDeleteTree(root->right);
        delete root;
    }
}

void SpaceSectorBST::postorderStellarPath(Sector* root, const std::string& sector_code){
    if (root != nullptr) {
        postorderStellarPath(root->left, sector_code);   
        postorderStellarPath(root->right, sector_code);
        //When sector is found, the path going to the tree root is stored in stack
        if (root->sector_code == sector_code){
            pathStack.push(root);
            while (root->parent!=nullptr){
                root = root->parent;
                pathStack.push(root);
            }
        }
    }
}

void SpaceSectorBST::postorderDeleteSector(Sector* root, const std::string& sector_code){
    if (root != nullptr) {
        postorderDeleteSector(root->left, sector_code);    
        postorderDeleteSector(root->right, sector_code);
        if (root->sector_code == sector_code){
            //A sector with no child
            if (root->left == nullptr && root->right == nullptr){
                if (root->parent!=nullptr){
                    //If sector to delete is parent's left child
                    if (root->parent->left == root){
                        root->parent->left = nullptr;
                        delete root;
                    }
                    //If sector to delete is parent's right child
                    if (root->parent->right == root){
                        root->parent->right = nullptr;
                        delete root;
                    }    
                } 
                //If sector to delete is the tree root
                else {
                    this->root = nullptr;
                    delete root;
                }
            }
            //A sector with only right child
            else if (root->left == nullptr){
                if (root->parent!=nullptr){
                    //If sector to delete is parent's left child
                    if (root->parent->left == root){
                        root->right->parent = root->parent;
                        root->parent->left = root->right;
                        delete root;
                    }
                    //If sector to delete is parent's right child
                    if (root->parent->right == root){
                        root->right->parent = root->parent;
                        root->parent->right = root->right;
                        delete root;
                    }   
                } 
                //If sector to delete is the tree root
                else {
                    root->right->parent = nullptr;
                    this->root = root->right;
                    delete root;
                }
                
            }
            //A sector with only left child
            else if (root->right == nullptr){
                if (root->parent!=nullptr){
                    //If sector to delete is parent's left child
                    if (root->parent->left == root){
                        root->left->parent = root->parent;
                        root->parent->left = root->left;
                        delete root;
                    }
                    //If sector to delete is parent's right child
                    if (root->parent->right == root){
                        root->left->parent = root->parent;
                        root->parent->right = root->left;
                        delete root;
                    }    
                } 
                //If sector to delete is the tree root
                else {
                    root->right->parent = nullptr;
                    this->root = root->right;
                    delete root;
                }
                
            }
            //A sector with two children
            else {
                //Inorder successor and its parent is found
                Sector* inorderSucParent = root;
                Sector* inorderSuc = root->right;
                while (inorderSuc->left != nullptr){
                    inorderSucParent = inorderSuc;
                    inorderSuc = inorderSuc->left;
                }
                if (root->parent!=nullptr){
                    //If sector to delete is parent's left child
                    if (root->parent->left == root){
                        inorderSuc->parent = root->parent;
                        root->parent->left = inorderSuc;
                        inorderSuc->left = root->left;
                        root->left->parent = inorderSuc;
                        if (inorderSucParent != root){
                            inorderSucParent->left = inorderSuc->right;
                            if (inorderSuc->right!=nullptr){
                                inorderSuc->right->parent = inorderSucParent;    
                            }
                            inorderSuc->right = root->right;
                            root->right->parent = inorderSuc;
                        }
                        delete root;
                    }
                    //If sector to delete is parent's right child
                    if (root->parent->right == root){
                        inorderSuc->parent = root->parent;
                        root->parent->right = inorderSuc;
                        inorderSuc->left = root->left;
                        root->left->parent = inorderSuc;
                        if (inorderSucParent != root){
                            inorderSucParent->left = inorderSuc->right;
                            if (inorderSuc->right!=nullptr){
                                inorderSuc->right->parent = inorderSucParent;    
                            }
                            inorderSuc->right = root->right;
                            root->right->parent = inorderSuc;
                        }
                        delete root;
                    }   
                } 
                //If sector to delete is the tree root
                else {
                    inorderSuc->parent = nullptr;
                    inorderSuc->left = root->left;
                    root->left->parent = inorderSuc;
                    if (inorderSucParent != root){
                        inorderSucParent->left = inorderSuc->right;
                        if (inorderSuc->right!=nullptr){
                            inorderSuc->right->parent = inorderSucParent;    
                        }
                        inorderSuc->right = root->right;
                        root->right->parent = inorderSuc;
                    }
                    this->root = inorderSuc;
                    delete root;
                }            
            }
        }
    }
}