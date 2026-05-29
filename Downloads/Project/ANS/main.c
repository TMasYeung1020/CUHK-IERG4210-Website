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
            else(){
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
    char input;
    
    while (1) {
        printf("Input a number [or H: hint]: ");
        scanf("%c", &input);
        fflush(stdin);
        
        if (input >= '1' && input <= '9'){
            if(gameBoard[x][y] == sol[x][y]){
                gameBoard[x][y] = input - '0';
                return 0;
            }
            else{
                printf("Sorry , %d should not be placed at (%d ,%d).",input , x, y);
                return -1;
            }
        } 
        else if (input == 'H') {
            if (gameMode == 1) {
                printf("No hint in Hard mode.\n");
                return -1;
            }else{
                gameBoard[x][y] == sol[x][y]
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
    Rvalue = 0;
    int subgridX = 3 * (x / 3);
    int subgridY = 3 * (y / 3);

    for (int i = 0; i < 9; i++){
        if(gameBoard[x][y] == gameBoard[x][i]){
            printf("Check again the numbers in row %d.", x);
            Rvalue = 1;
        }
        if(gameBoard[x][y] == gameBoard[i][y]){
            printf("Check again the numbers in colume %d.", y);
            Rvalue = 1;
        }
    }

    for (int i = subgridX; i < subgridX + 3; i++){
        for (int j = subgridY; j < subgridY + 3; j++){
            if(gameBoard[x][y] == gameBoard[i][j]){
                printf("Check again the numbers in the subgrid where (%d, %d) is at.", x, y);
                Rvalue = 1;
            }
        }
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
    int numlist[9] = {0};
    if (gameBoard[x][y] != 0) {
        printf("Occupied.")
        return 0;
    }
    if ((x < 0 || x > 8) || (y < 0 || y > 8)){
        printf("Out of bound. Input Again.")
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
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Done!!
/* ** Part 2: Save and Load** */
// the savePuzzle function saves the gameBoard and solution to the file "saveGame.txt"
// You may add parameter to this function, e.g, the file to save.
// The program returns 1 if it successfully saves the file, and 0 otherwise.
int savePuzzle(int gameBoard[][9], int sol[][9]){
    // TODO: Complete this part
    FILE *fptr;
    fptr = fopen("saveGame.txt", "w");

    if (fptr == NULL) {
        printf("Unable to open the file.\n");
        return 0;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fprintf(fptr, "%d ", gameBoard[i][j]);
        }
        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\n");

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fprintf(fptr, "%d ", solution[i][j]);
        }
        fprintf(fptr, "\n");
    }
    printf("Game Saved.")
    fclose(fptr);
    return 1;
}

// Done!!
// the loadPuzzle function load the gameBoard and solution from the file "saveGame.txt"
// You may add parameter to this function, e.g, the file to load.
// The program returns 1 if it successfully loads the file, and 0 otherwise.
int loadPuzzle(int gameBoard[][9], int sol[][9]){
    // TODO: Complete this part
    FILE *fptr;
    fptr = fopen("saveGame.txt", "r");

    if (fptr == NULL) {
        printf("Unable to open the file.\n");
        return 0;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num;
            if (fscanf(fptr, "%d", &num) != 1) {
                fclose(fptr);
                return 0;
            }
            gameBoard[i][j] = num;
        }
    }

    int ch;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            int nextCh = fgetc(fptr);
            if (nextCh == '\n') {
                continue;
            } else {
                ungetc(nextCh, fptr);
                break;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num;
            if (fscanf(fptr, "%d", &num) != 1) {
                fclose(fptr);
                return 0;
            }
            solution[i][j] = num;
        }
    }
    fclose(fptr);
    return 1;
}

// Done!!
// A helper function to craft a text file containing the inputs to fill in the puzzle according to the solution.
// Execute it to generate a text file that prints the inputs leading to a finished puzzle
// To Debug:
//  In Powershell, run: Get-Content .\textInput.txt | & .\main.exe
//  or simply run the program, copy and paste contents of the text file to console.
// You may modify the function as you wish
void printSolution(int puzzle[][9],int solution[][9]){
    FILE *fp;
    fp = fopen("textInput.txt", "w");
    if (fp == NULL) {
        printf("Error in writing file.");
        return;
    }

    fprintf(fp, "01\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                fprintf(fp, "%d %d\n", i, j);
                fprintf(fp, "%d\n", solution[i][j]);
            }
        }
    }
    fclose(fp);
}

/* The main function */
int main()
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

    // number will store the puzzle ID (player's puzzle selection)
    int number;

    // Create YOUR local variables
    /* Initialize the local variables */
    int win = 0, validIn = 0, inputValid = 0;
    int check = 0;

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
        printf("You have selected Hard mode.");
    }else{
        printf("You have selected Easy mode.");
    }

    // Call initGameBoard to read the puzzle to gameBoard
    initGameBoard(gameBoard, myPuzzle);

    // and call printGameBoard to print it
    printGameBoard(gameBoard);    

    // The following line calls the helper function, which print the user inputs leading to a finished puzzle
    printSolution(gameBoard, mySolution);

    // TODO: Your Easy/Hard mode Game starts
    // The following is the suggested workflow
    
    // While the game is not finished:
    while(!checkFinish(gameBoard, mySolution)){
        // Let the player select a cell
        printf("Select a row and column: ");
        scanf("%d %d", &x, &y);

        // Repeat the cell selection process if it is invalid
        checkFillable(gameBoard, x, y);
        if(!checkFillable(gameBoard, x, y)){
            continue;
        }

        // Print Game Board After player chose a valid cell (displayed as 'X')
        gameBoard[x][y] == 'X';
        printGameBoard(gameBoard);

        // Receive input from the player by calling inputBoard().
        inputBoard(gameBoard, x, y, mySolution);
        // Use the return value from inputBoard() for program control flow.
        if(inputBoard(gameBoard, x, y, mySolution) == 0){
            // check the answer for different game mode
            // e.g., call checkSolutionDetail() for Hard mode
            if(gameMode == 1){
                checkSolutionDetail(gameBoard, x, y);
                if(!checkSolutionDetail(gameBoard, x, y)){
                    gameBoard[x][y] == mySolution[x][y];
                }else{
                    chances -= checkSolutionDetail(gameBoard, x, y);
                    if(chances > 0){
                        printf("You have %d chances left .", chances);
                    }else{
                        printf("You lose.");
                        return 0;
                    }

                }
            }
        }
        // If inputBoard indicates invalid input, go back to cell selection
        else if(inputBoard(gameBoard, x, y, mySolution) == -1){
            gameBoard[x][y] == 0;
            continue;
        }

        else if(inputBoard(gameBoard, x, y, mySolution) == 1){
            // using hint
            hintCount++;
        }
        // print the game board
        printGameBoard(gameBoard);
    }

    // If the player wins, exit the while loop, else continue the game.
    // Output the winning or losing message
    if(gameMode == 0){
        printf("Congratulations! You have finished a puzzle in easy mode and used %d hints.", hintCount);
    }else if(gameMode == 1){
        printf("Congratulations! You have finished a puzzle in hard mode with %d chances left", chances);
    }
    
    return 0;
}