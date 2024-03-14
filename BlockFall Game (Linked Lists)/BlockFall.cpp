    #include "BlockFall.h"
    #include <fstream>
    #include <iostream>

    BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
            gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
        initialize_grid(grid_file_name);
        read_blocks(blocks_file_name);
        leaderboard.read_from_file(leaderboard_file_name);
    }

    void BlockFall::read_blocks(const string &input_file) {
        // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
        // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
        //       that represents the game blocks, as explained in the PA instructions.
        // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
        std::ifstream inputFile(input_file);
        if (inputFile.is_open()) {
            std::string line;
            bool initialized=false;
            vector<vector<bool>> oneBlock;
            Block *current = nullptr;
            while (getline(inputFile, line)) {
                if (!line.empty()){
                    vector<bool> oneLine;
                    for (char digit : line) {
                        if (isdigit(digit)) {
                            oneLine.push_back(digit - '0');
                        }
                    }
                    oneBlock.push_back(oneLine);

                    //Checking when block is completed
                    if (line[line.length()-1]==']'){
                        if (!getline(inputFile, line)) {
                            power_up = oneBlock;
                            break; // exit the loop as this is the last block
                        }
                        //Creating the block
                        Block *block = new Block();
                        block->shape = oneBlock;
                        block->next_block = nullptr;
                        block->right_rotation = nullptr;
                        block->left_rotation = nullptr;

                        //Creating the rotations of this block
                        block->right_rotation = new Block();
                        block->left_rotation = new Block();
                        block->right_rotation->right_rotation = new Block();
                        block->right_rotation->shape = rotateRight(block->shape);
                        block->left_rotation->shape = rotateLeft(block->shape);
                        block->right_rotation->right_rotation->shape = rotateRight(block->right_rotation->shape);

                        //Block with three rotations
                        if (block != (block->right_rotation) && block != (block->left_rotation) && block->right_rotation != block->left_rotation){
                            block->right_rotation->left_rotation = block;
                            block->left_rotation->right_rotation = block;


                            block->left_rotation->left_rotation = block->right_rotation->right_rotation;

                            block->right_rotation->right_rotation->right_rotation = block->left_rotation;
                            block->left_rotation->left_rotation->left_rotation = block->right_rotation;
                        }

                        //Block with one rotation
                        else if (block != block->right_rotation && block != block->left_rotation && block->right_rotation == block->left_rotation){
                            delete block->left_rotation;
                            delete block->right_rotation->right_rotation;
                            block->left_rotation = block->right_rotation;
                            block->right_rotation->left_rotation = block;
                            block->right_rotation->right_rotation = block;
                        }

                        //Block with no rotations
                        else if (block->shape == rotateRight(block->shape) && block->shape == rotateLeft(block->shape)){
                            delete block->right_rotation;
                            delete block->left_rotation;
                            delete block->right_rotation->right_rotation;
                            block->right_rotation = block;
                            block->left_rotation = block;
                        }

                        //If this is the first block in the input file
                        if (!initialized){
                            initial_block = block;
                            active_rotation = block;
                            current = initial_block;
                            initialized=true;
                        } 
                        //If this is NOT the first block in the input file
                        else if (initialized){
                            current->next_block=block;
                            current = block;
                        }
                        oneBlock.clear();
                    }
                }
            }

            inputFile.close();
        }
    }

    void BlockFall::initialize_grid(const string &input_file) {
        // TODO: Initialize "rows" and "cols" member variables
        // TODO: Initialize "grid" member variable using the command-line argument 1 in main
        std::ifstream inputFile(input_file);
        if (inputFile.is_open()){
            int rowCount = 0;
            int colCount = 0;
            std::string line;

            //Getting number of rows for grid
            while (std::getline(inputFile, line)) {
                rowCount++;
            }

            //Getting number of cols for grid
            for (int i = 0; i < line.length(); ++i) {
                char cell = line[i];
                if (cell == '0' || cell == '1'){
                    colCount++;
                }
            }

            //Initializing row and col variables
            rows = rowCount;
            cols = colCount;
            
            //Preparing the grid
            grid.resize(rows, vector<int>(cols));
            inputFile.clear();
            inputFile.seekg(0);
            int rowCount2 = 0;

            while (std::getline(inputFile, line)) {
                int colCount2 = 0;
                for (char cell : line) {
                    if (isdigit(cell)){
                        grid[rowCount2][colCount2] = cell-'0';
                        colCount2++;
                    }
                }
                rowCount2++;
            }

            inputFile.close();
        }
    }


    BlockFall::~BlockFall() {
        // TODO: Free dynamically allocated memory used for storing game blocks
        Block* current_block = initial_block;
        Block* next_block;

        while (current_block != nullptr) {
            //Free memory for left rotation
            if (current_block->left_rotation != nullptr && current_block != current_block->left_rotation){
                Block* left_rotation = current_block->left_rotation;
                delete left_rotation;
                current_block->left_rotation=nullptr;
            }
            
            //Free memory for inverse rotation
            if (current_block->right_rotation != nullptr && current_block->right_rotation->right_rotation != nullptr && current_block != current_block->right_rotation->right_rotation){
                Block* inverse_rotation = current_block->right_rotation->right_rotation;
                delete inverse_rotation; 
                current_block->right_rotation->right_rotation=nullptr;
            }

            //Free memory for right rotation
            if (current_block->right_rotation != nullptr && current_block != current_block->right_rotation){
                Block* right_rotation = current_block->right_rotation;
                delete right_rotation;
                current_block->right_rotation=nullptr;
            }

            
            //Move to the next block in the linked list
            next_block = current_block->next_block;

            //Delete the current block
            delete current_block;

            //Move to the next block in the linked list
            current_block = next_block;
        }
    }

    vector<vector<bool>> BlockFall :: rotateRight(const vector<vector<bool>> &shape) {
        vector<vector<bool>> rightRotated(shape[0].size(), vector<bool>(shape.size(), false));

        for (int i = 0; i < shape.size(); ++i) {
            for (int j = 0; j < shape[0].size(); ++j) {
                rightRotated[j][shape.size()-1-i] = shape[i][j];
            }
        }

        return rightRotated;
    }

    vector<vector<bool>> BlockFall :: rotateLeft(const vector<vector<bool>> &shape) {
        vector<vector<bool>> leftRotated(shape[0].size(), vector<bool>(shape.size(), false));

        for (int i = 0; i < shape.size(); ++i) {
            for (int j = 0; j < shape[0].size(); ++j) {
                leftRotated[shape[0].size()-1-j][i] = shape[i][j];
            }
        }

        return leftRotated;
    }
