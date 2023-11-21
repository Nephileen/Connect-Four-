#include <stdio.h>
#include <stdlib.h>

//constant variables 
const int ROWS = 6;
const int COLUMNS = 7;
const char PLAYER1 = 'X'; 
const char PLAYER2 = 'O'; 

//Create Struct to represent game players 
struct players {
    char player1; 
    char player2; 
    char activePlayer; 
    char Winner; 

}; 

//create struct to represent possible outcomes 
struct game {
    char isDraw; 
    char hasWinner; 
}; 

// Function prototypes (these functions display the game status)
void announceWinner(char winner);
void printArr(char** grid);



/* Function to create a 2D grid array and initialize it with empty spaces
   Returns a dynamically allocated 2D array for the game grid
****/
char** createArr(){
    char **gridArr = malloc(sizeof(char*) * ROWS); 
    for (int i = 0; i < ROWS; i++){
        gridArr[i] = malloc(sizeof(char) * COLUMNS); 

        for (int j = 0; j < COLUMNS; j++){
            gridArr[i][j] = ' ';  
        }
        
    }
    return gridArr; 
}

/* Function to initialize game players
   Returns a struct containing player information
****/
struct players* initializePlayers() {
    struct players *gamePlayers = malloc(sizeof(struct players)); 
    gamePlayers -> player1 = 'X'; 
    gamePlayers -> player2 = 'O'; 
    gamePlayers -> activePlayer = PLAYER1; 
    gamePlayers -> Winner = ' ';

    return gamePlayers;

}

/* Function to initialize game
   Returns a struct containing possible outcomes
****/
struct game* initializeGame() {
    struct game *gameStatus = malloc(sizeof(struct game)); 
    gameStatus -> isDraw = 'N';
    gameStatus -> hasWinner = 'N';  
    return gameStatus;
}


/***************Functions to check for winning conditions*******************/
/* Parameters:
   grid: The game grid
   gamePlayers: Struct containing player information
   currCol: The current column of the move
   currRow: The current row of the move
   Each of these functions checks for a winning pattern on the grid
****/


// Function to check for a diagonal win from the bottom-left to top-right
int diagonalRight (char** grid, struct players *gamePlayers, int currCol, int currRow){
   
    int counter = 1; 
    for (int j = 1; j < 4; j++){
        if ((currRow-j < 0) || (currCol-j < 0) || (grid[currRow - j][currCol - j] != (gamePlayers -> activePlayer))){
            break; 
        }
        counter++; 
    } 

    for (int i = 1; i < 4; i++){
        //checking out of bounce to the right ?
        if((currRow+i > 5) || (currCol + i > 6) || (grid[currRow + i][currCol + i] != (gamePlayers -> activePlayer))){ 
            break; 
        }
        counter++; 
    }
    return (counter >= 4); 


}

// Function to check for a diagonal win from the bottom-right to top-left
int diagonalLeft (char** grid, struct players *gamePlayers, int currCol, int currRow){

    int counter = 1; 
    for (int i = 1; i < 4; i++){
        if((currRow-i < 0) || (currCol + i > 6) || (grid[currRow - i][currCol + i] != (gamePlayers -> activePlayer))){ 
            break; 
        }
         
        counter++; 
    }
     
     for (int j = 1; j < 4; j++){

        if ((currRow+j > 5) || (currCol-j < 0) || (grid[currRow + j][currCol - j] != (gamePlayers -> activePlayer))){
            break; 
        }
        counter++; 
    } 
    return (counter >= 4); 
}

// Function to check for a horizontal win
int horizontal (char** grid, struct players *gamePlayers, int currCol, int currRow){

    int counter = 0; 
   
    for (int i = 0; i < COLUMNS; i++){
       if (grid[currRow][i] != (gamePlayers -> activePlayer)){
        counter = 0;
        continue;     
       } 
       counter++;
       if (counter == 4) {
        break;
       }
    }
     
    return counter == 4; 

}

// Function to check for a vertical win
int vertical (char** grid, struct players *gamePlayers, int currCol, int currRow){
    int counter = 0;
    if(currRow < 3){
        int i = 0; 
        while(i<4){
            if ((currRow+i > 5) || grid[currRow+i][currCol] != (gamePlayers -> activePlayer)){
                break; 
            }
            i++; 
            counter++;
        }
    }
    return counter >= 4; 
}

/* Function to check if the current move results in a win for the active player
   by calling the vertical, horizontal, diagonalRight and diagonalLeft functions.
   set the winner in gamePlayers.
****/
void winCase (char** grid, struct players *gamePlayers, int currCol, int currRow){

    if (diagonalLeft(grid, gamePlayers, currCol, currRow) || diagonalRight(grid, gamePlayers, currCol, currRow) ||
        vertical (grid, gamePlayers, currCol, currRow) || horizontal (grid, gamePlayers, currCol, currRow)){
         gamePlayers -> Winner = gamePlayers -> activePlayer; 
         
    } 
}



/***********Functions for updating the game state***********/


/* Function to update the grid with a player's move
   Parameters:
   - col: The column where the player wants to make a move (1-7).
   - grid: The game grid (2D array).
   - gamePlayers: Struct representing game players, including the active player.
****/
void updateGrid(int col, char** grid, struct players *gamePlayers){
    col = col - 1; 
    //error handling------
    for(int i = ROWS-1; i >= 0; i--){
        if (grid[i][col] == ' '){
            grid[i][col] = gamePlayers -> activePlayer; 
            winCase(grid, gamePlayers, col, i);
            break; 
        }
        continue; 
    }

}

/* Function to change the active player
   Parameters:
   - gamePlayers: Struct representing game players, including the active player.
****/
void changePlayers(struct players *gamePlayers){
    if(gamePlayers -> activePlayer == PLAYER1){
        gamePlayers -> activePlayer = PLAYER2; 
    } else {
        gamePlayers -> activePlayer = PLAYER1;
    }
    printf("PLAYER %c TURN\n", gamePlayers -> activePlayer); 
}



/***********Game Management Functions.***********/

   
/* Function to free memory allocated for the grid and gamePlayers
   Parameters:
   grid: The game grid to be freed
   gamePlayers: Struct containing player information to be freed
****/
 void freeGame(char** grid, struct players *gamePlayers) {
    for (int i = 0; i < ROWS; i++){
        free(grid[i]);
    }
    free(grid);
    free(gamePlayers);
} 

/* Function to check if a move is valid
   Parameters:
   -input: The column where the player wants to place their piece
   -grid: The game grid
   Returns 1 if the move is valid, 0 otherwise
****/
int isMoveValid(int input, char** grid) {
    //checks if input is not within the range 
    if (!(input > 0 && input <= COLUMNS)) {
        return 0;
    }

    //check when the column is full-----
    if (grid[0][input-1] != ' '){
        return 0; 
    }
    
    return 1;
}

/* Function to check if there is a winner
   Parameters:
   gamePlayers: Struct containing players information
   Returns 1 if there's a winner, 0 otherwise
****/
int isWinner(struct players *gamePlayers) {
    return (gamePlayers -> activePlayer == gamePlayers -> Winner);
}


int isDraw (struct game *gameStatus){
    return (gameStatus -> isDraw == 'Y'); 
}



// Function to implement the game loop
void playGame(struct game *gameStatus) {
    
    char** grid = createArr();
    printArr(grid);
    
    
    struct players* gamePlayers = initializePlayers();
    //might need a while------------

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        int input;
        printf("Player %c, enter your move (1-7): \n", gamePlayers->activePlayer);
        //whatif the user enter non-int type------------
        
        
        int result = scanf("%d", &input);

    
        
        if ((result == 1)&&isMoveValid(input, grid)) {

            updateGrid(input, grid, gamePlayers);
            printArr(grid);
            
            if (isWinner(gamePlayers)) {
                announceWinner(gamePlayers->Winner);
                gameStatus -> hasWinner = 'Y'; 
                break;
            }

            changePlayers(gamePlayers);

        } else {
            // Clear the input buffer to avoid infinite loops 
            while (getchar() != '\n'); 
            printf("Invalid move. Please try again.\n");
            i--;
           

        }
    }
    //draw posibility-------
    if (gameStatus -> hasWinner != 'Y'){
        gameStatus -> isDraw = 'Y'; 
    }

    freeGame(grid, gamePlayers);
}




/****************Functions for displaying and managing the game*****************/


/* Function to announce the winner
   Parameters:
   - winner: The character representing the winning player ('X' or 'O').
****/
void announceWinner(char winner) {
    printf("\n");
    printf("**************************************\n");
    printf("*          CONGRATULATIONS,          *\n");
    printf("*          PLAYER %c WINS!            *\n", winner);
    printf("*             .-'''''-.              *\n");
    printf("*           .' _     _ '.            *\n");
    printf("*          /   (_) (_)   \\           *\n");
    printf("*         | ,           , |          *\n");
    printf("*         |   (   -   )   |          *\n");
    printf("*         | ,           , |          *\n");
    printf("*          \\   __|__     /           *\n");
    printf("*           `._      _.'`            *\n");
    printf("*              `''''`                *\n");
    printf("**************************************\n");
    printf("\n");
}

/* Function to print the grid
   Parameters:
   -grid: The game grid to be printed
****/
void printArr(char** grid){
    for (int row = 0; row < ROWS; row++) {
        printf("+---+---+---+---+---+---+---+\n");
        printf("|");
        for (int col = 0; col < COLUMNS; col++) {
            printf(" %c |", grid[row][col]);
        }
        printf("\n");
    }
    printf("+---+---+---+---+---+---+---+\n");
    printf("  1   2   3   4   5   6   7\n");
}

/* Print game introduction
   Call playGame function
   Print game over message
****/

int main() {


    printf("********************* CONNECT FOUR *********************\n \n"); 
    printf("--> Grab a partner! It's a two player game. \n"); 
    printf("--> Player 1 will go first and input a number from 1 - 7 \n"); 
    printf("--> Then Player 2 will do the same thing \n"); 
    printf("--> Whoever connects 4 first, WINS! \n \n"); 
    printf("*********************************************************\n \n");

    struct game* gameStatus = initializeGame(); 
    playGame (gameStatus);


    printf("%d \n", isDraw(gameStatus)); 
   
    if (isDraw(gameStatus)){
       printf("IT'S A DRAW!!\n");   
    }
    free(gameStatus);
    printf("\t\tGame Over!\n***********Thanks for Playing***********\n"); 
    
    return 0;
}




