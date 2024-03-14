#include "SpaceSectorLLRBT.h"
#include <stack>

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    postorderDeleteTree(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    if (root == nullptr){
        root = recursiveLLRBT (root, x, y, z);
        root->color = BLACK;
    } else {
        recursiveLLRBT (root, x, y, z);
    }
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postorderTraversal(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    bool1 = false; bool2 = false; once = true; earthPath.clear(); sectorPath.clear();
    postorderStellarPath(root,sector_code);
    while (!pathStack.empty()){
        Sector* sector = pathStack.top();
        path.push_back(sector);
        pathStack.pop();
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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

Sector* SpaceSectorLLRBT::recursiveLLRBT (Sector* root, int x, int y, int z){
    if (root == nullptr) {
        Sector* sector = new Sector(x, y, z);
        return sector;
    }

    //If X is greater
    if (x > root->x){
        root->right = recursiveLLRBT(root->right, x, y, z);
        root->right->parent = root;
    }
    //If X is smaller
    else if (x < root->x){
        root->left = recursiveLLRBT(root->left, x, y, z);
        root->left->parent = root;
    } 
    //If X is equal
    else {
        //If Y is greater
        if (y > root->y){
            root->right = recursiveLLRBT(root->right, x, y, z);
            root->right->parent = root;
        }
        //If Y is smaller
        else if (y < root->y){
            root->left = recursiveLLRBT(root->left, x, y, z);
            root->left->parent = root;
        } 
        //If Y is equal 
        else {
            //If Z is greater
            if (z > root->z){
                root->right = recursiveLLRBT(root->right, x, y, z);
                root->right->parent = root;
            }
            //If Z is smaller
            else if (z < root->z){
                root->left = recursiveLLRBT(root->left, x, y, z);
                root->left->parent = root;
            } else {
                return root;
            }
        }
    }

    //Left Rotation
    if (root->right!=nullptr){
        if (root->right->color==RED && (root->left==nullptr || root->left->color==BLACK)){
            root = leftRotation(root);
        }    
    }

    //Right Rotation
    if (root->left!=nullptr && root->left->left!=nullptr){
        if (root->left->left->color==RED && root->left->color==RED){
            root = rightRotation(root);
        }    
    }
        
    //Color Flip
    if (root->left!=nullptr && root->right!=nullptr){
        if (root->right->color==RED && root->left->color==RED){
            if (root->parent!=nullptr){
                root->color = !root->color;  
            } else {
                root->color = BLACK;
            }        
            root->left->color=BLACK;
            root->right->color=BLACK;
        }    
    }

    return root;
}

Sector* SpaceSectorLLRBT::leftRotation (Sector* rootx){
    //Left Rotation Algorithm
    Sector* rotatedSector = rootx->right;
    Sector* rotatedSectorLeft = rotatedSector->left;
    if (rootx->parent!=nullptr){
        if (rootx->parent->left == rootx){
            rotatedSector->parent = rootx->parent;
            rootx->parent->left = rotatedSector;
        }
        if (rootx->parent->right == rootx){
            rotatedSector->parent = rootx->parent;
            rootx->parent->right = rotatedSector;
        }
    } else {
        rotatedSector->parent = nullptr;
        root = rotatedSector;
    }
    rootx->parent = rotatedSector;
    rotatedSector->left = rootx;
    if (rotatedSectorLeft!=nullptr){
        rotatedSectorLeft->parent = rootx;
    }
    rootx->right = rotatedSectorLeft;

    //Color Change
    if (rootx->color==BLACK){
        rootx->color = rotatedSector->color;
        rotatedSector->color = BLACK;   
    }

    return rotatedSector;
}

Sector* SpaceSectorLLRBT::rightRotation (Sector* rootx){
    //Right Rotation Algorithm
    Sector* rotatedSector = rootx->left;
    Sector* rotatedSectorRight = rotatedSector->right;
    if (rootx->parent!=nullptr){
        if (rootx->parent->left == rootx){
            rotatedSector->parent = rootx->parent;
            rootx->parent->left = rotatedSector;
        }
        if (rootx->parent->right == rootx){
            rotatedSector->parent = rootx->parent;
            rootx->parent->right = rotatedSector;
        }
    } else {
        rotatedSector->parent = nullptr;
        root = rotatedSector;
    }
    rootx->parent = rotatedSector;
    rotatedSector->right = rootx;
    if (rotatedSectorRight!=nullptr){
        rotatedSectorRight->parent = rootx;
    }
    rootx->left = rotatedSectorRight;

    //Color Change
    rootx->color = rotatedSector->color;
    rotatedSector->color = BLACK;

    return rotatedSector;
}

void SpaceSectorLLRBT::inorderTraversal(Sector* root){
    if (root != nullptr) {
        inorderTraversal(root->left);
        if (root->color==RED){
            std::cout << "RED sector: " << root->sector_code << std::endl;
        }
        if (root->color==BLACK){
            std::cout << "BLACK sector: " << root->sector_code << std::endl;
        }
        inorderTraversal(root->right);
    }
}

void SpaceSectorLLRBT::preorderTraversal(Sector* root){
    if (root != nullptr) {
        if (root->color==RED){
            std::cout << "RED sector: " << root->sector_code << std::endl;
        }
        if (root->color==BLACK){
            std::cout << "BLACK sector: " << root->sector_code << std::endl;
        }
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void SpaceSectorLLRBT::postorderTraversal(Sector* root){
    if (root != nullptr) {
        postorderTraversal(root->left);    
        postorderTraversal(root->right);
        if (root->color==RED){
            std::cout << "RED sector: " << root->sector_code << std::endl;
        }
        if (root->color==BLACK){
            std::cout << "BLACK sector: " << root->sector_code << std::endl;
        }
    }
}

void SpaceSectorLLRBT::postorderDeleteTree(Sector* root){
    if (root != nullptr) {
        postorderDeleteTree(root->left);    
        postorderDeleteTree(root->right);
        delete root;
    }
}

int SpaceSectorLLRBT::postorderStellarPath(Sector* root, const std::string& sector_code){
    if (root != nullptr) {
        postorderStellarPath(root->left, sector_code);   
        postorderStellarPath(root->right, sector_code);
        //When the Earth is found
        if (root->sector_code == "0SSS"){
            Sector* temp = root;
            earthPath.push_back(temp);
            while (temp->parent!=nullptr){
                temp = temp->parent;
                earthPath.push_back(temp);
            }
            bool1=true;
        }
        //When Dr. Elara's sector is found
        if (root->sector_code == sector_code){
            Sector* temp = root;
            sectorPath.push_back(temp);
            while (temp->parent!=nullptr){
                temp = temp->parent;
                sectorPath.push_back(temp);
            }
            bool2=true;
        }
        //When both sectors are found for the first time
        if (bool1 && bool2 && once){
            int where;
            once=false;
            for (int i = 0; i < earthPath.size(); i++) {
                Sector* sector1 = earthPath[i];
                //If Dr. Elara's sector is an ancestor of the Earth
                if (sector1->sector_code == sector_code){
                    where = i;
                    pathFindingChoice(this->root, sector_code, where, 1, sector1->sector_code);
                    return 1;
                }
                for (int j = 0; j < sectorPath.size(); j++) {
                    Sector* sector2 = sectorPath[j];
                    //When the lowest common ancestor of both sectors is found
                    if (sector1->sector_code == sector2->sector_code){
                        where = j;
                        pathFindingChoice(this->root, sector_code, where, 3, sector2->sector_code);
                        return 1;
                    }
                    //If the Earth is an ancestor of Dr. Elara's sector
                    if (sector2->sector_code == "0SSS"){
                        where = j;
                        pathFindingChoice(this->root, sector_code, where, 2, sector2->sector_code);
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
}
void SpaceSectorLLRBT::pathFindingChoice(Sector* root, const std::string& sector_code, int where, int choice, std::string sector_code2){
    if (root!=nullptr){
        //If Dr. Elara's sector is an ancestor of the Earth
        if (choice==1){
            for (int i = where; i >=0; i--) {
                Sector* sector = earthPath[i];
                pathStack.push(sector);
            }
        }
        //If the Earth is an ancestor of Dr. Elara's sector
        if (choice==2){
            for (int i = 0; i < where+1; i++) {
                Sector* sector = sectorPath[i];
                pathStack.push(sector);
            }
        }
        //If the Earth and Dr. Elara's sectors are not each of theirs ancestors
        if (choice==3){
            pathFindingChoice(root->left, sector_code, where, 3, sector_code2);
            pathFindingChoice(root->right, sector_code, where, 3, sector_code2);
            if (root->sector_code == sector_code){
                pathStack.push(root);
                while (root->parent!=nullptr){
                    root = root->parent;
                    pathStack.push(root);
                    //When lowest common ancestor is found, break
                    if (root->sector_code==sector_code2){
                        break;
                    }
                }
                //Finding the path from Earth to lowest common ancestor
                findingEarth(root);
            }
        }    
    }
    
}

int SpaceSectorLLRBT::findingEarth(Sector* root){
    if (root->sector_code=="0SSS"){
        return 1;
    } else {
        //If X is greater
        if (root->x>0){
            pathStack.push(root->left);
            findingEarth(root->left);   
        }
        //If X is smaller
        else if (root->x<0){
            pathStack.push(root->right);
            findingEarth(root->right);  
        }
        //If X is equal
        else {
            //If Y is greater
            if (root->y>0){
                pathStack.push(root->left);
                findingEarth(root->left);  
            }
            //If Y is smaller
            else if (root->y<0){
                pathStack.push(root->right);
                findingEarth(root->right);  
            }
            //If Y is equal
            else {
                //If Z is greater
                if (root->z>0){
                    pathStack.push(root->left);
                    findingEarth(root->left);   
                }
                //If Z is smaller
                else if (root->z<0){
                    pathStack.push(root->right);
                    findingEarth(root->right);  
                }
            }
        }    
    }   
}