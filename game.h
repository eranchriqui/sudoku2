

#ifndef SUDOKU2_GAME_H
#define SUDOKU2_GAME_H

typedef struct {

    int **userBoard;
    int **solution;
    int **fixedValues;
    int isSolved;
    int counter;
} gameParams;


typedef struct {

    int values[9];
    int num;

} optionalValues;


/* Initializes the game */
int initGame(gameParams *game);

/* Fills the fixed values (the hints) */
void fillFixedValues(gameParams *game, int numHints);

/* Gets the number of hint from the user */
int getNumHintsFromUser();

/* Validates if the game is solvable.
 * if it is, replaces the new solution for the gameParam solution */
int validate(gameParams *game);

void restart(gameParams *game);

/* Exits the game */
void exitGame(gameParams *game);

/* Gives a hint for the user from the current solution*/
void hint(int x, int y, gameParams *game);

/* Sets new value for cell X Y if possible */
int set(int x, int y, int z, gameParams *game);

int checkIfIsOver(int **userBoard);

/* Checks if cell <X,Y> is a fixed value */
int checkIfFixed(int x, int y, int **fixedValues);

/* Checks if Z is a valid value for non-fixed cell <X,Y> */
int checkIfValid(int x, int y, int z, int **userBoard);

/* Creates an instance of the optionalValues struct  */
optionalValues *createOptionalValues(int x, int y, int **solution);

/* Deletes an optional value by index */
void deleteOptionalValueByIndex(optionalValues *val, int index);

/* Prints the board */
void printBoard(gameParams *game);

/* Allocates memory for matrix mat */
int **allocateMatrix(int **mat);

/* Frees memory allocated for matrix mat of size n*/
void freeMatrix(int **mat, int n);

/* Creates a new instance for the gameParams struct */
gameParams *createNewGame(gameParams *game);


#endif
