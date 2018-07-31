/*
 * solver.h
 *
 *  Created on: May 12, 2018
 *      Author: amirlande
 */


#ifndef SOLVER_H
#define SOLVER_H

#include "parser.h"
#include "game.h"


/* finds a new solution using deterministic backtracking */
int solveUsingDetBacktracking(gameParams *game);

/* finds a new solution using random backtracking */
int randBacktrack(int **solution);


#endif
