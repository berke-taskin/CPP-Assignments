#include "GameController.h"
#include <fstream>
#include <iostream>

bool GameController::play(BlockFall& game, const string& commands_file) {

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    Block* current_block = game.initial_block;
    //Placing the block on the grid
    currentCol=0;
    if (canPlace(game)){
        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                if (game.active_rotation->shape[i][j]==1) {
                    game.grid[i][j] = 1;
                }
            }
        }    
    }
    else if (!canPlace(game)){
        return false;
    }
    
    std::ifstream inputFile(commands_file);
    if (inputFile.is_open()) {
        std::string line;
        while (getline(inputFile, line)) {
            if (!line.empty()){
                if (line=="MOVE_RIGHT"){
                    if (canMoveRight(game, currentCol)){
                        //Clearing the initial position of block
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.grid[i][currentCol+j]==1) {
                                    game.grid[i][currentCol+j] = 0;
                                }
                            }
                        }

                        //Moving the block to the right
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.active_rotation->shape[i][j]==1) {
                                    game.grid[i][currentCol+j+1] = 1;
                                }
                            }
                        }
                        currentCol++;
                    }
                }
                else if (line=="MOVE_LEFT") {
                    if (canMoveLeft(game, currentCol)){
                        //Clearing the initial position of block
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.grid[i][currentCol+j]==1) {
                                    game.grid[i][currentCol+j] = 0;
                                }
                            }
                        }

                        //Moving the block to the left
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.active_rotation->shape[i][j]==1) {
                                    game.grid[i][currentCol+j-1] = 1;
                                }
                            }
                        }
                        currentCol--;
                    }
                }
                else if (line=="ROTATE_RIGHT") {
                    if (canRotateRight(game, currentCol)){
                        //Clearing the initial position of block
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.grid[i][currentCol+j]==1) {
                                    game.grid[i][currentCol+j] = 0;
                                }
                            }
                        }

                        //Rotating the block to the right
                        game.active_rotation = game.active_rotation->right_rotation;
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.active_rotation->shape[i][j]==1) {
                                    game.grid[i][currentCol+j] = 1;
                                }
                            }
                        }
                    }
                }
                else if (line=="ROTATE_LEFT") {
                    if (canRotateRight(game, currentCol)){
                        //Clearing the initial position of block
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.grid[i][currentCol+j]==1) {
                                    game.grid[i][currentCol+j] = 0;
                                }
                            }
                        }

                        //Rotating the block to the left
                        game.active_rotation = game.active_rotation->left_rotation;
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.active_rotation->shape[i][j]==1) {
                                    game.grid[i][currentCol+j] = 1;
                                }
                            }
                        }
                    }
                }
                else if (line=="PRINT_GRID") {
                    //Printing the scores and grid
                    std::cout << "Score: " << game.current_score << std::endl;
                    std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                    for (int i = 0; i < game.rows; ++i) {
                        for (int j = 0; j < game.cols; ++j) {
                            if (game.grid[i][j] == 1) {
                                std::cout << "\u2B1C";
                            } else {
                                std::cout << "\u2B1B";
                            }
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                    std::cout << std::endl;
                }
                else if (line=="GRAVITY_SWITCH") {
                    //Switching the gravity
                    if (game.gravity_mode_on){
                        //Gravity becomes off if it was on
                        game.gravity_mode_on=false;
                    } else {
                        //Gravity becomes on if it was off
                        game.gravity_mode_on=true;

                        bool all_clear = false;
                        while (!all_clear){
                            //Applying gravity
                            for (int i = game.rows-2; i >= 0; --i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (i >= game.active_rotation->shape.size() && game.grid[i][j]==1) {
                                        int count = i;
                                        while (count < game.rows-1 && game.grid[count+1][j]==0) {
                                            game.grid[count+1][j] = 1;
                                            game.grid[count][j] = 0;
                                            count++;
                                        }
                                    }
        
                                    if (i < game.active_rotation->shape.size() && game.grid[i][j]==1) {
                                        if (j < currentCol || j >= game.active_rotation->shape[0].size()+currentCol){
                                            int count = i;
                                            while (count < game.rows-1 && game.grid[count+1][j]==0) {
                                                game.grid[count+1][j] = 1;
                                                game.grid[count][j] = 0;
                                                count++;
                                            }
                                        }
                                    }         
                                }      
                            } 

                            all_clear=true;

                            //Checking if a row is completed
                            for (int i = game.active_rotation->shape.size(); i < game.rows; ++i) {
                                if (completedRow(game,i)){
                                    //Printing the grid
                                        std::cout << "Before clearing: " << std::endl;
                                        for (int i = 0; i < game.rows; ++i) {
                                            for (int j = 0; j < game.cols; ++j) {
                                                if (game.grid[i][j] == 1) {
                                                    std::cout << "\u2B1C";
                                                } else {
                                                    std::cout << "\u2B1B";
                                                }
                                            }
                                            std::cout << std::endl;
                                        }
                                        std::cout << std::endl;
                                        std::cout << std::endl;

                                    //Clearing row
                                    game.current_score += game.cols;
                                    for (int j = 0; j < game.cols; ++j) {
                                        game.grid[i][j]=0;
                                    }
                                    all_clear=false;
                                }
                            }
                                                
                        }    
                    }   
                }
                else if (line=="DROP") {
                    bool isPowerUp = true;
                    bool isHeart = false;

                    //Defining iconic heart shape
                    vector<vector<bool>> heart_shape = {
                            {false,  false, true, false, true, false, false}, 
                            {false, true,  true,  true, true, true, false},
                            {false, true,  true,  true, true, true, false},
                            {false, false, true, true,  true, false, false},
                            {false, false, false, true, false, false, false},
                    };

                    //If current active rotation is power up
                    for (int i = 0; i < game.power_up.size(); ++i) {
                            for (int j = 0; j < game.power_up[i].size(); ++j) {
                                if (i < game.active_rotation->shape.size() && j < game.active_rotation->shape[i].size()) {
                                    if (game.active_rotation->shape[i][j] != game.power_up[i][j]){
                                        isPowerUp=false;
                                    }
                                } else {
                                    isPowerUp=false;
                                }
                        }
                    }

                    //If the shape is heart
                    if (game.power_up == heart_shape){
                        isHeart=true;
                    }

                    //If the gravity mode is on
                    if (game.gravity_mode_on){
                        int minDrop = game.rows;
                        //Dropping the block with gravity on
                        for (int i = game.active_rotation->shape.size()-1; i >= 0; --i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.active_rotation->shape[i][j]==1) {
                                    int count = i;
                                    while (count < game.rows-1 && game.grid[count+1][currentCol+j]==0) {
                                        game.grid[count+1][currentCol+j] = 1;
                                        game.grid[count][currentCol+j] = 0;
                                        count++;
                                    }
                                    minDrop = std::min(minDrop, count-i);
                                }      
                            }        
                        }

                        game.current_score += howManyCells(game)*minDrop;

                        //Power up is false if shape is not preserved
                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                if (game.grid[minDrop+i][currentCol+j]!=game.active_rotation->shape[i][j]) {
                                    isPowerUp=false;
                                }
                            }
                        }

                        //If block is not power up
                        if (!isPowerUp){
                            bool all_clear = false;
                            while (!all_clear){
                                all_clear=true;

                                //Checking if a row is completed
                                for (int i = game.active_rotation->shape.size(); i < game.rows; ++i) {
                                    if (completedRow(game,i)){
                                        //Printing the grid
                                        std::cout << "Before clearing: " << std::endl;
                                        for (int i = 0; i < game.rows; ++i) {
                                            for (int j = 0; j < game.cols; ++j) {
                                                if (game.grid[i][j] == 1) {
                                                    std::cout << "\u2B1C";
                                                } else {
                                                    std::cout << "\u2B1B";
                                                }
                                            }
                                            std::cout << std::endl;
                                        }
                                        std::cout << std::endl;
                                        std::cout << std::endl;

                                        game.current_score += game.cols;
                                        for (int j = 0; j < game.cols; ++j) {
                                            game.grid[i][j]=0;
                                        }
                                        all_clear=false;
                                    }
                                }
                                
                                //Applying gravity again if a row is deleted
                                if (!all_clear){    
                                    for (int i = game.rows-2; i >= 0; --i) {
                                        for (int j = 0; j < game.cols; ++j) {
                                            if (game.grid[i][j]==1) {
                                                int count = i;
                                                while (count < game.rows-1 && game.grid[count+1][j]==0) {
                                                    game.grid[count+1][j] = 1;
                                                    game.grid[count][j] = 0;
                                                    count++;
                                                }
                                            }       
                                        }      
                                    } 
                                }
                            }     
                        } 
                        //If block is power up
                        else if (isPowerUp) {
                            //Printing the grid
                            std::cout << "Before clearing: " << std::endl;
                            for (int i = 0; i < game.rows; ++i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (game.grid[i][j] == 1) {
                                        std::cout << "\u2B1C";
                                    } else {
                                        std::cout << "\u2B1B";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;
                            std::cout << std::endl;

                            //Clearing the grid
                            int puCells=0;
                            for (int i = 0; i < game.grid.size(); ++i) {
                                for (int j = 0; j < game.grid[i].size(); ++j) {
                                    if (game.grid[i][j]==1){
                                        puCells++;
                                    }
                                    game.grid[i][j]=0;
                                }
                            }
                            game.current_score += 1000+puCells;
                        }
                        

                        //Checking if there is a next block to come
                        if (current_block->next_block != nullptr){
                            //Placing the new block on the grid
                            currentCol=0;
                            if (canPlace(game)){
                                current_block = current_block->next_block;
                                game.active_rotation = current_block;
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                        if (game.active_rotation->shape[i][j]==1) {
                                            game.grid[i][j] = 1;
                                        }
                                    }
                                }    
                            }
                            else if (!canPlace(game)){
                                //Printing the block
                                std::cout << "GAME OVER!" << std::endl;
                                std::cout << "Next block that couldn't fit: " << std::endl;
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                        std::cout << game.active_rotation->shape[i][j];
                                    }
                                    std::cout << std::endl;
                                }
                                std::cout << std::endl;
                                
                                //Printing the final grid and score
                                std::cout << "Final grid and score: " << std::endl;
                                std::cout << std::endl;
                                std::cout << "Score: " << game.current_score << std::endl;
                                std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                                for (int i = 0; i < game.rows; ++i) {
                                    for (int j = 0; j < game.cols; ++j) {
                                        if (game.grid[i][j] == 1) {
                                            std::cout << "\u2B1C";
                                        } else {
                                            std::cout << "\u2B1B";
                                        }
                                    }
                                    std::cout << std::endl;
                                }
                                std::cout << std::endl;

                                //Printing Leaderboard
                                LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                                game.leaderboard.insert_new_entry(leaderboardEntry);
                                //delete leaderboardEntry;
                                game.leaderboard.write_to_file(game.leaderboard_file_name);
                                std::cout << "Leaderboard" << std::endl;
                                std::cout << "-----------" << std::endl;
                                game.leaderboard.print_leaderboard();
                                return false;
                            }
                        }
                        //If blocks have ended
                        else if (current_block->next_block == nullptr){
                            //Printing the final grid and score
                            std::cout << "YOU WIN!" << std::endl;
                            std::cout << "No more blocks." << std::endl;
                            std::cout << "Final grid and score: " << std::endl;
                            std::cout << std::endl;
                            std::cout << "Score: " << game.current_score << std::endl;
                            std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                            for (int i = 0; i < game.rows; ++i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (game.grid[i][j] == 1) {
                                        std::cout << "\u2B1C";
                                    } else {
                                        std::cout << "\u2B1B";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;

                            //Printing Leaderboard
                            LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                            game.leaderboard.insert_new_entry(leaderboardEntry);
                            //delete leaderboardEntry;
                            game.leaderboard.write_to_file(game.leaderboard_file_name);
                            std::cout << "Leaderboard" << std::endl;
                            std::cout << "-----------" << std::endl;
                            game.leaderboard.print_leaderboard();
                            return true;
                        }
                    }

                    //If the gravity mode is off
                    if (!game.gravity_mode_on){
                        //Dropping the block with gravity off
                        currentRow = 0;
                        while (canMoveDown(game,currentCol,currentRow)) {
                            for (int i = currentRow+game.active_rotation->shape.size()-1; i >= currentRow; --i) {
                                for (int j = 0; j < game.active_rotation->shape[i-currentRow].size(); j++) {
                                    if (game.active_rotation->shape[i-currentRow][j]==1){
                                        game.grid[i+1][currentCol+j] = 1;
                                        game.grid[i][currentCol+j] = 0;    
                                    }
                                    
                                }
                            }
                            currentRow++;
                        }
                        
                        game.current_score += howManyCells(game)*currentRow;

                        //If power up shape is not heart
                        if (!isHeart){
                            for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                    if (game.grid[currentRow+i][currentCol+j]!=game.active_rotation->shape[i][j]) {
                                        isPowerUp=false;
                                    }
                                }
                            }    
                        }   

                        //If power up shape is heart
                        if (isHeart){
                            isPowerUp=false;
                            for (int i = 0; i < game.rows-5; ++i) {
                                for (int j = 0; j < game.cols-7; ++j) {
                                    bool match = true;
                                    for (int k = 0; k < 5; ++k) {
                                        for (int l = 0; l < 7; ++l) {
                                            if (game.grid[i + k][j + l] != game.power_up[k][l]) {
                                                match = false;
                                                break;
                                            }
                                        }
                                        if (!match) {
                                            break;
                                        }
                                    }
                                    if (match) {
                                        isPowerUp=true;
                                    }
                                }
                            }
                        }

                        //If power up is not fit to clear the grid
                        if (!isPowerUp){
                            bool all_clear = false;
                            while (!all_clear){
                                all_clear=true;
                                int whichRow=0;

                                //Checking if a row is completed
                                for (int i = game.active_rotation->shape.size(); i < game.rows; ++i) {
                                    if (completedRow(game,i)){
                                        //Printing the scores and grid
                                        std::cout << "Before clearing: " << std::endl;
                                        for (int i = 0; i < game.rows; ++i) {
                                            for (int j = 0; j < game.cols; ++j) {
                                                if (game.grid[i][j] == 1) {
                                                    std::cout << "\u2B1C";
                                                } else {
                                                    std::cout << "\u2B1B";
                                                }
                                            }
                                            std::cout << std::endl;
                                        }
                                        std::cout << std::endl;
                                        std::cout << std::endl;

                                        game.current_score += game.cols;
                                        for (int j = 0; j < game.cols; ++j) {
                                            game.grid[i][j]=0;
                                        }
                                        all_clear=false;
                                        whichRow=i;
                                        break;
                                    }
                                }
                                
                                //Shifting all rows accordingly
                                if (!all_clear){
                                    for (int i = whichRow-1; i >= 0; --i) {
                                        for (int j = 0; j < game.cols; ++j) {
                                            if (game.grid[i][j]==1) {
                                                game.grid[i+1][j] = 1;
                                                game.grid[i][j] = 0;
                                            }       
                                        }      
                                    } 
                                }
                            }    
                        }
                        //If power up is fit to clear the grid
                        else if (isPowerUp) {
                            //Printing the scores and grid
                            std::cout << "Before clearing: " << std::endl;
                            for (int i = 0; i < game.rows; ++i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (game.grid[i][j] == 1) {
                                        std::cout << "\u2B1C";
                                    } else {
                                        std::cout << "\u2B1B";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;
                            std::cout << std::endl;

                            //Clearing the grid
                            int puCells=0;
                            for (int i = 0; i < game.grid.size(); ++i) {
                                for (int j = 0; j < game.grid[i].size(); ++j) {
                                    if (game.grid[i][j]==1){
                                        puCells++;
                                    }
                                    game.grid[i][j]=0;
                                }
                            }
                            game.current_score += 1000+puCells;
                        }
                        

                        //Checking if there is a next block to come
                        if (current_block->next_block != nullptr){
                            //Placing the new block on the grid
                            currentCol=0;
                            current_block = current_block->next_block;
                            game.active_rotation = current_block;
                            if (canPlace(game)){
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                        if (game.active_rotation->shape[i][j]==1) {
                                            game.grid[i][j] = 1;
                                        }
                                    }
                                }
                            }
                            else if (!canPlace(game)){
                                //Printing the block
                                std::cout << "GAME OVER!" << std::endl;
                                std::cout << "Next block that couldn't fit: " << std::endl;
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
                                        std::cout << game.active_rotation->shape[i][j];
                                    }
                                    std::cout << std::endl;
                                }
                                std::cout << std::endl;
                                
                                //Printing the final grid and score
                                std::cout << "Final grid and score: " << std::endl;
                                std::cout << std::endl;
                                std::cout << "Score: " << game.current_score << std::endl;
                                std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                                for (int i = 0; i < game.rows; ++i) {
                                    for (int j = 0; j < game.cols; ++j) {
                                        if (game.grid[i][j] == 1) {
                                            std::cout << "\u2B1C";
                                        } else {
                                            std::cout << "\u2B1B";
                                        }
                                    }
                                    std::cout << std::endl;
                                }
                                std::cout << std::endl;

                                //Printing Leaderboard
                                std::cout << "Leaderboard" << std::endl;
                                std::cout << "-----------" << std::endl;
                                LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                                game.leaderboard.insert_new_entry(leaderboardEntry);
                                //delete leaderboardEntry;
                                game.leaderboard.write_to_file(game.leaderboard_file_name);
                                game.leaderboard.print_leaderboard();
                                return false;
                            }
                        }
                        //If blocks have ended
                        else if (current_block->next_block == nullptr){
                            //Printing the final grid and score
                            std::cout << "YOU WIN!" << std::endl;
                            std::cout << "No more blocks." << std::endl;
                            std::cout << "Final grid and score: " << std::endl;
                            std::cout << std::endl;
                            std::cout << "Score: " << game.current_score << std::endl;
                            std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                            for (int i = 0; i < game.rows; ++i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (game.grid[i][j] == 1) {
                                        std::cout << "\u2B1C";
                                    } else {
                                        std::cout << "\u2B1B";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;

                            //Printing Leaderboard
                            LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                            game.leaderboard.insert_new_entry(leaderboardEntry);
                            //delete leaderboardEntry;
                            game.leaderboard.write_to_file(game.leaderboard_file_name);
                            std::cout << "Leaderboard" << std::endl;
                            std::cout << "-----------" << std::endl;
                            game.leaderboard.print_leaderboard();
                            return true;
                        }
                    }
                } else { //If commmand is not "MOVE_LEFT", "MOVE_RIGHT", "ROTATE_LEFT", "ROTATE_RIGHT","PRINT_GRID", "GRAVITY_SWITCH" or "DROP"
                    std::cout << "Unknown Command: " << line <<std::endl;
                    std::cout << std::endl;
                }
            }
        }
    }

    //Deleting the current active block from top left of the grid
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (game.active_rotation->shape[i][j]==1){
                game.grid[i][j] = 0; 
            }
        }
    }
    
    //Printing the final grid and score
    std::cout << "GAME FINISHED!" << std::endl;
    std::cout << "No more commands." << std::endl;
    std::cout << "Final grid and score: " << std::endl;
    std::cout << std::endl;
    std::cout << "Score: " << game.current_score << std::endl;
    std::cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    for (int i = 0; i < game.rows; ++i) {
        for (int j = 0; j < game.cols; ++j) {
            if (game.grid[i][j] == 1) {
                std::cout << "\u2B1C";
            } else {
                std::cout << "\u2B1B";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    //Printing Leaderboard
    LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
    game.leaderboard.insert_new_entry(leaderboardEntry);
    std::cout << "Leaderboard" << std::endl;
    std::cout << "-----------" << std::endl;
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    return true;
}

bool GameController::canPlace(const BlockFall& game) {
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (game.active_rotation->shape[i][j]==1){
                if (game.grid[i][j]==1) {
                    return false;
                }
            }  
        }
    }
    return true;
}

bool GameController::canMoveRight(const BlockFall& game, int col) {
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (col+game.active_rotation->shape[i].size() >= game.cols) {
                return false;
            }
            else if (j!=game.active_rotation->shape[i].size()-1){
                if (game.grid[i][col+j]==1 && game.grid[i][col+j+1]==1 && (game.active_rotation->shape[i][j+1])==0){
                    return false;
                }    
            }
            else if (j==game.active_rotation->shape[i].size()-1) {
                if (game.grid[i][col+j]==1 && game.grid[i][col+j+1]==1){
                    return false;
                }   
            } 
        }
    }
    return true;
}

bool GameController::canMoveLeft(const BlockFall& game, int col) {
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (col<=0) {
                return false;
            }
            else if (j!=0){
                if (game.grid[i][col+j]==1 && game.grid[i][col+j-1]==1 && (game.active_rotation->shape[i][j-1])==0){
                    return false;
                }    
            }
            else if (j==0) {
                if (game.grid[i][col+j]==1 && game.grid[i][col+j-1]==1){
                    return false;
                }   
            } 
        }
    }
    return true;
}

bool GameController::canRotateRight(const BlockFall& game, int col) {
    Block* right_rotation = game.active_rotation->right_rotation;
    if (col + right_rotation->shape[0].size() > game.cols) {
        return false;
    }

    for (int i = 0; i < right_rotation->shape.size(); ++i) {
        for (int j = 0; j < right_rotation->shape[i].size(); ++j) {
                    if (right_rotation->shape[i][j] == 1) {
                        if (game.grid[i][col + j] == 1 && (game.active_rotation->shape[i][j])==0) {
                            return false;
                        }
                    }     
        }
    }
    return true;
}

bool GameController::canRotateLeft(const BlockFall& game, int col) {
    Block* left_rotation = game.active_rotation->left_rotation;
    if (col + left_rotation->shape[0].size() > game.cols) {
        return false;
    }

    for (int i = 0; i < left_rotation->shape.size(); ++i) {
        for (int j = 0; j < left_rotation->shape[i].size(); ++j) {
                    if (left_rotation->shape[i][j] == 1) {
                        if (game.grid[i][col + j] == 1 && (game.active_rotation->shape[i][j])==0) {
                            return false;
                        }
                    }     
        }
    }
    return true;
}

bool GameController::completedRow(const BlockFall& game, int row) { 
    for (int j = 0; j < game.cols; ++j) {
        if (game.grid[row][j]==0) {
            return false;
        }
    }
    return true;
}

bool GameController::canMoveDown(const BlockFall& game, int col, int row) {
    for (int i = row; i < row+game.active_rotation->shape.size(); ++i) {
        if (i==game.grid.size()-1){
                return false;
        }
        for (int j = 0; j < game.active_rotation->shape[i-row].size(); ++j) {
            if (i-row != game.active_rotation->shape.size()-1){
               if (game.active_rotation->shape[i-row][j]==1 && game.grid[i+1][col+j]==1 && (game.active_rotation->shape[i-row+1][j])==0){
                    return false;
                } 
            }
            
            else if (i-row == game.active_rotation->shape.size()-1){
               if (game.active_rotation->shape[i-row][j]==1 && game.grid[i+1][col+j]==1){
                    return false;
                } 
            } 
        }
    }
    return true;
}

int GameController::howManyCells(const BlockFall& game) {
    int count=0;
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); ++j) {
            if (game.active_rotation->shape[i][j]==1) {
                count++;
            }
        }   
    }
    return count;
}