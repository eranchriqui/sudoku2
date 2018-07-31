/*
 * solver.c
 *
 *  Created on: May 12, 2018
 *      Author: amirlande
 */

#include <stdlib.h>
#include "game.h"


/* userBoard is a 9x9 matrix (sudoku board), row and col are pointers to ints
 * finds first empty cell and assigns its coordinates to row and col
 * */
int findEmptyCell(int **userBoard, int *row, int *col) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (userBoard[i][j] == 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
    return 0;
}

/* Used to deep copy a matrix */

int **copyMatrix(int **oldMat) {

    int i, j;
    int **newMat;

    newMat = calloc(9, sizeof(int *));
    if (newMat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < 9; i++) {
        newMat[i] = calloc(9, sizeof(int));
        if (newMat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            newMat[i][j] = oldMat[i][j];
        }
    }


    return newMat;

}

/* solves a sudoku board using the deterministic Backtracking algorithm (if solvable)
 * returns "1" if solvable, "0" otherwise
 * */
int solveUsingDetBacktrackingRecursion(int **newSol) {
    int row, col;
    int val;

    /* if no empty cells exist then board is legally full - return 1 (true)
     * otherwise - (row, col) holds the first unassigned cell from (left to right and top to bottom) */
    if (findEmptyCell(newSol, &row, &col) == 0) {
        return 1;
    }

    /* check 1-9 values for (row,col) */
    for (val = 1; val <= 9; val++) {
        /* use checkIfValid function to check validity of assignment */
        if (checkIfValid(row, col, val, newSol)) {
            newSol[row][col] = val;
            /* if assigning (row, col) = val resulted in success return 1 (true), otherwise - remove val */
            if (solveUsingDetBacktrackingRecursion(newSol)) {
                return 1;
            } else {
                newSol[row][col] = 0;
            }

        }
    }
    /* return 0 (false) if 1-9 assignments to (row,col) returned false (an unsolvable board)
     * and backtrack (this return serves as one piece of the if (solveUsingDetBacktrackting(userBoard)) recursion chain */
    return 0;
}


/* finding a new solution using deterministic backtracking */

int solveUsingDetBacktracking(gameParams *game) {

    int **newSol;

    newSol = copyMatrix(game->userBoard);

    if (solveUsingDetBacktrackingRecursion(
            newSol)) { /* if board is solvable get rid of previous solution matrix and assign newSol */
        freeMatrix(game->solution, 9);
        game->solution = newSol;
        return 1;

    } else {
        freeMatrix(newSol, 9);
        return 0;
    }

}

/* Randomized backtracking */


int randBacktrackRecursion(int x, int y, int **solution) {

    int index;
    int i, j;
    optionalValues *options = createOptionalValues(x, y, solution);

    while (options->num > 0) {

        if (options->num == 1) {
            index = 0;

        } else {
            index = rand() % options->num;
        }
        if (checkIfValid(x, y, options->values[index], solution)) {
            solution[x][y] = options->values[index];

            if (x == 8 && y == 8) { /* check if reached end of board */
                free(options);
                return 1;
            }

        } else { /* values[index] isn't valid (in terms of square \ row \ column */
            deleteOptionalValueByIndex(options, index);
            continue;
        }

        /* get the next cell index like in nested for loop */
        if (y == 8) { /* reached end of row - increment row index and set column to first (0) */
            i = x + 1;
            j = 0;
        } else { /* otherwise just move to next (right) column */
            i = x;
            j = y + 1;
        }

        /* recurse to next cell */
        if (!(randBacktrackRecursion(i, j, solution))) {
            solution[x][y] = 0;
            deleteOptionalValueByIndex(options, index);
            continue;
        } else {

            /* free options memory and return */
            free(options);
            return 1;
        }
    }
    free(options);
    return 0;
}

/* envelope function */

int randBacktrack(int **solution) {

    return randBacktrackRecursion(0, 0, solution);
}



