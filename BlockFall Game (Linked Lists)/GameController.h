#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    int currentCol=0;
    int currentRow;
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    bool canPlace(const BlockFall& game); // Function that returns true if block can be placed
    bool canMoveRight(const BlockFall& game, int col); // Function that returns true if block can move right
    bool canMoveLeft(const BlockFall& game, int col); // Function that returns true if block can move left
    bool canRotateRight(const BlockFall& game, int col); // Function that returns true if block can rotate right 
    bool canRotateLeft(const BlockFall& game, int col); // Function that returns true if block can rotate left
    bool completedRow(const BlockFall& game, int row); // Function that checks if that certain row is completed
    bool canMoveDown(const BlockFall& game, int col, int row); // Function that checks if block can move down while dropping
    int howManyCells(const BlockFall& game); // Function that finds how many cells that block contains for scoring
};


#endif //PA2_GAMECONTROLLER_H
