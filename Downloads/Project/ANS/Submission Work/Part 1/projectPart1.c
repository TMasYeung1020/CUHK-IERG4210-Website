/**
 * ENGG1110 Problem Solving by Programming
 *
 * Course Project
 *
 * I declare that the project here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   https://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name  : Yeung Man
 * Student ID    : 1155203181
 * Class/Section : B
 * Date          : 17/11
 */

#include <stdio.h>

/* NO other header files are allowed */

/* NO global variables are allowed */

// Done!!
// Initialize the specified game board.
void initGameBoard(int gameBoard[][9], int puzzle[][9]) {
    // TODO: Complete this part
    // Hint: Copies the content of puzzle to gameBoard
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            gameBoard[i][j] = puzzle[i][j];
        }
    }
}

// Done!!
/* Display the specified game board on the screen.
   You are required to exactly follow the output format stated in the project specification.
   IMPORTANT: Using other output format will result in mark deduction. */
void printGameBoard(int gameBoard[][9]) {
    // TODO: Complete this part
    // Hint:
    // 1. First, try to prints the content of gameBoard
    // 2. Use conditional statement to print ' ' or 'X' for the two cases (empty or selected cell).
    // 3. Add the borders, col/row numbers
    printf("  012 345 678 \n");
    printf(" +---+---+---+\n");
    for (int i = 0; i < 9; i++){
        if (i == 3 || i == 6){
            printf(" +---+---+---+\n");
        }
        printf("%d|", i);
        for (int j = 0; j < 9; j++){
            if(j == 3 || j == 6){
                printf("|");
            }
            if(gameBoard[i][j] == 0){
                printf(" ");
            }
            else if (gameBoard[i][j] == 'X'){
                printf("X");
            }
            else{
                printf("%d", gameBoard[i][j]);
            }
        }
        printf("|\n");
    }
    printf(" +---+---+---+\n");
}


// Done!!
/* inputBoard() reads a char '1' to '9', or 'H' from the player.
    You need to apply input validation, output error message, and ask the player to retry if its input is invalid.
    gameMode indicates if the game is run in Easy (0) or Hard (1) mode, so that you can determine if hint is allowed.
    *** The function returns 1 if 'H' is inputted (i.e., the player uses a hint), returns -1 if an invalid input is read, and 0 otherwise (valid input). ***
    IMPORTANT: Using other output format will result in mark deduction. */
int inputBoard(int gameBoard[][9], int x, int y, int sol[][9], int gameMode){
    // TODO: Complete this part
    // Hint: Use while loop to keep scanning input from the player
    char c;
    
    while (1) {
        printf("Input a number [or H: hint]: ");
        scanf(" %c", &c);
        fflush(stdin);
        
        if ((int)c >= 49 && (int)c <= 57){
            if(gameMode == 0){
                if((int)c - '0' == sol[x][y]){
                    gameBoard[x][y] = c - '0';
                    return 0;
                }
                else{
                    printf("Sorry , %d should not be placed at (%d, %d).\n", c - '0', x, y);
                    return -1;
                }
            }
            if(gameMode == 1){
                gameBoard[x][y] = c - '0';
                return 0;
            }
        }
        else if (c == 'H') {
            if (gameMode == 1) {
                printf("No hint in Hard mode.\n");
                return 1;
            }else{
                gameBoard[x][y] = sol[x][y];
            }
            return 1;
        }
        else {
            printf("Invalid Input.\n");
            return -1;
        }
    }
}

// Done!!
// This function outputs 1 if the gameBoard is finished (and identical to the solution), and 0 otherwise.
int checkFinish(int gameBoard[][9], int sol[][9]){
    // TODO: Complete this part
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if(gameBoard[i][j] != sol[i][j]){
                return 0;
            }
        }
    }

    return 1;
}

// Done!!
/* ** Hard Mode **
    Check the (x, y) cell in the game board to see if it conflicts with other numbers in the same row/column/subgrid.
    You need to output messages to indicate incorrect answer.
    if (x,y) cell causes conflict in 2 or more categories, all such messages will be printed.
    The function returns 1 if (x, y) causes conflicts, and 0 otherwise.
    Hint: The return value can be used to update chances in main.
    IMPORTANT: Using other output format will result in mark deduction. */

int checkSolutionDetail(int gameBoard[][9], int x, int y){
    // TODO: Complete this part
    int Rvalue = 0;
    int subgridX = 3 * (x / 3);
    int subgridY = 3 * (y / 3);

    // (a) How do you check collision in row and column?
    // ANS: i used for loop to read from [x][1-9] and [1-9][y], without the cell itself, if there is any cell number same with user input, it will return 1 and print a statement
    for (int i = 0; i < 9; i++){
        if(gameBoard[x][y] == gameBoard[x][i]){
            if(y != i){
                printf("Check again the numbers in row %d.\n", x);
                Rvalue = 1;
            }
        }
    }
    
    for (int k = 0; k < 9; k++){
        if(gameBoard[x][y] == gameBoard[k][y]){
            if(x != k){
                printf("Check again the numbers in colume %d.\n", y);
                Rvalue = 1;
            }
        }
    }
    
    // (b) How do you check collision for the 3x3 subgrid for the selected cell?
    // ANS: i seperated 9 parts, for 9 blocks by using 3*(position/3), if position is 0 - 2, it will be 0. If 3 - 5, it will be 3. If 6 - 8, it will be 6. Then the for loop will start to check all the value inside the block from the starting X/Y position + 2, which is the endpoint of the block. If any cell value same with user input, a statement will be printed and return 1 also.

    for (int i = subgridX; i < subgridX + 3; i++){
        for (int j = subgridY; j < subgridY + 3; j++){
            if(x!=i || y!=j){
                if(gameBoard[i][j] == gameBoard[x][y]){
                    printf("Check again the numbers in the subgrid where (%d, %d) is at.\n", x, y);
                    Rvalue = 1;
                }
            }}
        }
    return Rvalue;
}

// Done!!
/* ** Part 2 **
    checkFillable() checks the row/column/subgrid that the cell (x, y) resides at given the current gameBoard.
    It determines if all the numbers in the same row + column + subgrid contain all numbers 1 - 9, i.e., the cell (x, y) cannot be filled (or is locked).
    It returns 0 if cell (x, y) is locked; and returns 1 if the cell (x, y) can be filled */
int checkFillable(int gameBoard[][9], int x, int y){
    // TODO: Complete this part
    // (a) How do you find the set of possible numbers for an empty cell in checkFillable()?
    // ANS: First, for the empty cell, i used if statement to confirm the cell is an empty cell, if not it will return 0 and print "Occupied". Then, i just use the logic of vote system to found the cell input existed at the same row + colume + subgrid, and count them by numlist[number inputed in the corresponding cell -1]++ if exist the corresponding number. At last, using for loop to determine if there exist any number that didn't filled by user, if all filled (>0), then the cell cannot be filled.
    
    int numlist[9] = {0};
    
    if ((x < 0 || x > 8) || (y < 0 || y > 8)){
        printf("Out of bound. Input Again.\n");
        return 0;
    }
    
    if (gameBoard[x][y] != 0) {
        printf("Occupied.\n");
        return 0;
    }

    for (int i = 0; i < 9; i++) {
        if (gameBoard[x][i] != 0) {
            numlist[gameBoard[x][i] - 1] = 1;
        }
        if (gameBoard[i][y] != 0) {
            numlist[gameBoard[i][y] - 1] = 1;
        }
    }
    int subgridX = 3 * (x / 3);
    int subgridY = 3 * (y / 3);
    for (int i = subgridX; i < subgridX + 3; i++){
        for (int j = subgridY; j < subgridY + 3; j++){
            if (gameBoard[i][j] != 0) {
                numlist[gameBoard[i][j] - 1] = 1;
            }
        }
    }
    for (int k = 0; k < 9; k++){
        if(numlist[k] == 0){
            return 1;
        }
    }
    return 0;
}

// Done!!
/*
    isLockBoard() determines if the input gameBoard contains a locked cell.
    If there is a locked cell, it prints a locked cell and returns 1.
    It returns 0 if the game board is free of locked cell
    Hint: loop through all empty cell and use checkFillable to aid in checking the cell.
    */
int isLockBoard(int gameBoard[][9]){
    // TODO: Complete this part
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (gameBoard[i][j] == 0) {
                if(!checkFillable(gameBoard, i, j)) {
                    printf("Cell (%d, %d) is locked.\n", i, j);
                    printf("Board is locked.\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* The main function */
int main(void)
{
    /* Local variables */

    // Assume both mode uses same set of puzzle
    // In Part 2, read the puzzle and solution from puzzle.txt
    // You may input your puzzle to debug, e.g., replace some entries of the solution by 0
    int myPuzzle[9][9]={
        {5,0,0,0,6,3,4,0,0},
        {0,0,0,7,0,0,0,0,0},
        {1,0,0,0,5,0,8,3,0},
        {0,0,0,0,1,8,0,0,7},
        {0,0,6,9,0,0,0,0,0},
        {0,4,3,0,0,0,9,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,7,0,0,2,0},
        {3,2,0,6,4,0,5,0,0}
    };
    // The solution
    int mySolution[9][9] = {
        {5,9,8,1,6,3,4,7,2},
        {6,3,2,7,8,4,1,5,9},
        {1,7,4,2,5,9,8,3,6},
        {2,5,9,4,1,8,3,6,7},
        {8,1,6,9,3,7,2,4,5},
        {7,4,3,5,2,6,9,8,1},
        {4,6,5,8,9,2,7,1,3},
        {9,8,1,3,7,5,6,2,4},
        {3,2,7,6,4,1,5,9,8}
    };
    // Game Board
    int gameBoard[9][9];

    // gameMode: {0: Easy  1: Hard}
    // chances is the no. of chances in hard mode
    // hintCount stores the no. of hint used.
    int gameMode, chances = 3, hintCount = 0;

    // store x, y coordinate of a cell
    int x, y;

    // Read the Game mode to gameMode
    while (1){
        printf("Enter the game mode [0: Easy. 1: Hard]: ");
        scanf("%d", &gameMode);
    // Repeats the input process for an invalid input
    if (gameMode != 0 && gameMode != 1){
        printf("Invalid Input.\n");
        continue;
    }
    break;
    }

    // Set the Game mode and display the message
    // TODO: Read Game Mode and output selected mode
    if(gameMode == 1){
        printf("You have selected Hard mode.\n");
    }else{
        printf("You have selected Easy mode.\n");
    }

    // Call initGameBoard to read the puzzle to gameBoard
    initGameBoard(gameBoard, myPuzzle);

    // and call printGameBoard to print it
    printGameBoard(gameBoard);

    // TODO: Your Easy/Hard mode Game starts
    // The following is the suggested workflow
    
    // While the game is not finished:
    while(!checkFinish(gameBoard, mySolution)){
        // Let the player select a cell
        printf("Select a row and column: ");
        scanf("%d %d", &x, &y);
        
        // Repeat the cell selection process if it is invalid
        if(!checkFillable(gameBoard, x, y)){
            continue;
        }
        
        // Print Game Board After player chose a valid cell (displayed as 'X')
        gameBoard[x][y] = 'X';
        printGameBoard(gameBoard);
        
        switch(inputBoard(gameBoard, x, y, mySolution, gameMode)){
                // Use the return value from inputBoard() for program control flow.
        case 0:
            // check the answer for different game mode
            // e.g., call checkSolutionDetail() for Hard mode
            if(gameMode == 1){
                if(!checkSolutionDetail(gameBoard, x, y)){
                    break;
                }else{
                    chances --;
                    if(chances > 0){
                        gameBoard[x][y] = 0;
                        printf("You have %d chances left.\n", chances);
                    }else{
                        printf("You lose.\n");
                        return 0;
                    }
                }
            }
            break;
        // If inputBoard indicates invalid input, go back to cell selection
    case -1:
        gameBoard[x][y] = 0;
        continue;
    case 1:
        // using hint
        if(gameMode == 0){
            hintCount++;
        }
        if(gameMode == 1){
            gameBoard[x][y] = 0;
        }
        break;
    }
        // print the game board
        printGameBoard(gameBoard);
        
        // if locked = end game, print you lose
        if(isLockBoard(gameBoard)){
            printf("You lose.\n");
            return 0;
        }
    }

    // If the player wins, exit the while loop, else continue the game.
    // Output the winning or losing message
    if(gameMode == 0){
        printf("Congratulations! You have finished a puzzle in easy mode and used %d hints.\n", hintCount);
    }else if(gameMode == 1){
        printf("Congratulations! You have finished a puzzle in hard mode with %d chances left\n", chances);
    }
    
    return 0;
}

