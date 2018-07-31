

#include "parser.h"

/* Initializes the game */

int initGame(gameParams *game) {

    int numHints;
    numHints = getNumHintsFromUser();

    /* in case of EOF numHints will be -1 */
    if (numHints == -1) {
        exitGame(game);
        return 0;
    }

    randBacktrack(game->solution);
    fillFixedValues(game, numHints);
    game->counter = numHints;
    game->isSolved = 0;
    printBoard(game);
    return 1;
}


/* Fills the fixed values (the hints) */

void fillFixedValues(gameParams *game, int numHints) {

    int i = 0;
    int z;
    while (i < numHints) {
        int y = rand() % 9;
        int x = rand() % 9;
        if (checkIfFixed(x, y, game->fixedValues)) {
            continue;
        }
        z = game->solution[x][y];
        game->fixedValues[x][y] = z;
        game->userBoard[x][y] = z;
        i++;
    }

}


/* Gets the number of hint from the user */

int getNumHintsFromUser() {

    int numHints;
    int retry = 1;
    int readData;
    while (retry) {
        printf("Please enter the number of cells to fill [0-80]:\n");
        readData = scanf("%d", &numHints);
        if (readData != 1) {
            printf("Error: scanf has failed\n");
            exit(0);
        }
        if (readData == EOF) {
            return -1;
        }
        if (numHints >= 0 && numHints <= 80) {
            retry = 0;
        } else {
            printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
        }
    }
    /* clear buffer after using scanf */
    while ((getchar()) != '\n');
    return numHints;
}


/* Allocates memory for matrix mat */
int **allocateMatrix(int **mat) {

    int i;
    mat = calloc(9, sizeof(int *));
    if (mat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < 9; i++) {
        mat[i] = calloc(9, sizeof(int));
        if (mat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
    }

    return mat;
}

/* Frees memory allocated for matrix mat of size n*/
void freeMatrix(int **mat, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

/* Validates if the game is solvable.
 * if it is, replaces the new solution for the gameParam solution */

int validate(gameParams *game) {

    if (!(solveUsingDetBacktracking(game))) {
        printf("Validation failed: board is unsolvable\n");
        return 0;
    }

    /* at gameSolve we update the array*/

    printf("Validation passed: board is solvable\n");
    return 1;
}

/* frees game memory and allocates it again */

void restart(gameParams *game) {

    /* free all allocated memory */

    freeMatrix(game->userBoard, 9);
    freeMatrix(game->solution, 9);
    freeMatrix(game->fixedValues, 9);

    createNewGame(game);
    initGame(game);

}


/* Exits the game */
void exitGame(gameParams *game) {

    printf("Exiting...\n");
    freeMatrix(game->userBoard, 9);
    freeMatrix(game->solution, 9);
    freeMatrix(game->fixedValues, 9);
    free(game);

}


/* Gives a hint for the user from the current solution*/
void hint(int x, int y, gameParams *game) {

    int z = game->solution[x - 1][y - 1];
    printf("Hint: set cell to %d\n", z);
}


/* Sets new value for cell X Y if possible */

int set(int x, int y, int z, gameParams *game) {


    if (checkIfFixed(x - 1, y - 1, game->fixedValues)) {
        printf("Error: cell is fixed\n");
        return 0;
    }

    if (!(checkIfValid(x - 1, y - 1, z, game->userBoard))) {
        printf("Error: value is invalid\n");
        return 0;
    }
    /* according to z value - increment or decrement game counter
     * if z was already set to (x,y) cell - don't change counter */
    if ((z == 0) && (game->userBoard[x - 1][y - 1] != 0)) { /* when a non-zero cell is set back to zero (emptied) */
        game->counter--;
    } else if ((z != 0) && (game->userBoard[x - 1][y - 1] == 0)) { /* when a zero cell is set to z (!=0) */
        game->counter++;
    }
    game->userBoard[x - 1][y - 1] = z;
    printBoard(game);
    /*printf("counter is now = %d", game->counter);*/


    if (game->counter == 81) {
        printf("Puzzle solved successfully\n");
        game->isSolved = 1;
    }
    return 1;

}


/* Checks if cell <X,Y> is a fixed value */
int checkIfFixed(int x, int y, int **fixedValues) {

    int value = fixedValues[x][y];
    if (value > 9 || value < 1) {
        return 0;
    }

    return 1;
}


/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid(int x, int y, int z, int **userBoard) {

    int i;
    int j;

    for (i = x - x % 3; i < x - x % 3 + 3; i++) {
        for (j = y - y % 3; j < y - y % 3 + 3; j++) {

            if (userBoard[i][j] == z) {
                if (!((i == x) && (j == y))) { /* exclude cell (x,y) from the square check */
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, int **userBoard) {

    int j;

    for (j = 0; j < 9; j++) {
        if (j != y) { /* exclude cell (x,y) from the square check */
            if (userBoard[x][j] == z) {
                return 0;
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, int **userBoard) {

    int i;

    for (i = 0; i < 9; i++) {
        if (i != x) { /* exclude cell (x,y) from the square check */
            if (userBoard[i][y] == z) {
                return 0;
            }
        }
    }
    return 1;
}

/* Checks if Z is a valid value for non-fixed cell <X,Y> */
int checkIfValid(int x, int y, int z, int **userBoard) {

    if (z == 0) return 1; /* always legal to set a non-fixed cell to 0 */

    if (!(checkIfSquareValid(x, y, z, userBoard))) {
        return 0;
    }

    if (!(checkIfRowValid(x, y, z, userBoard))) {
        return 0;
    }

    if (!(checkIfColumnValid(x, y, z, userBoard))) {
        return 0;
    }

    return 1;

}


/* Creates an instance of the optionalValues struct  */
optionalValues *createOptionalValues(int x, int y, int **solution) {

    int z;
    optionalValues *val = (optionalValues *) malloc(sizeof(optionalValues));
    if (val == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }
    val->num = 0;
    for (z = 1; z <= 9; z++) {
        if (checkIfValid(x, y, z, solution)) {
            val->values[val->num] = z;
            val->num++;
        }
    }
    return val;
}

/* Deletes an optional value by index */
void deleteOptionalValueByIndex(optionalValues *val, int index) {

    int c;
    for (c = index; c < (val->num) - 1; c++) {
        val->values[c] = val->values[c + 1];
    }
    val->num--;
}

/* Prints the board */
void printBoard(gameParams *game) {

    char rowSep[] = "----------------------------------\n";
    int i, j;

    for (i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            printf("%s", rowSep);
        }
        for (j = 0; j < 9; j++) {
            if (j == 0) {
                printf("|");
            } else if (j % 3 == 0) {
                printf(" |");
            }
            if (checkIfFixed(i, j, game->fixedValues)) {
                printf(" .%d", game->fixedValues[i][j]);
            } else if (game->userBoard[i][j] > 0 && game->userBoard[i][j] <= 9) {
                printf("  %d", game->userBoard[i][j]);
            } else {
                printf("   ");
            }


        }
        printf(" |\n");
    }
    printf("%s", rowSep);

}


/* Creates a new instance for the gameParams struct */
gameParams *createNewGame(gameParams *game) {


    game->userBoard = allocateMatrix(game->userBoard);
    game->solution = allocateMatrix(game->solution);
    game->fixedValues = allocateMatrix(game->fixedValues);
    game->isSolved = 0;
    game->counter = 0;

    return game;
}


