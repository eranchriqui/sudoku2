/*
 * parser.c
 *
 *  Created on: May 11, 2018
 *      Author: amirlande
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "game.h"
#include "parser.h"

/* gets a tokenized command and calls relevant function
 * uses atoi(tokens[i]) to convert strings to ints
 * */
int resolveCommand(char *tokens[1024], gameParams *game) {
    if (strcmp(tokens[0], "set") == 0) {
        if (tokens[1] != NULL && tokens[2] != NULL && tokens[3] != NULL && (game->isSolved) == 0) {

            /* ### X-(token1) and Y-(token2) was replaced ### */

            set(atoi(tokens[2]), atoi(tokens[1]), atoi(tokens[3]), game);
            return 1;
        }
        printf("Error: invalid command\n");
        return 1;
    } else if (strcmp(tokens[0], "hint") == 0) {
        if (tokens[1] != NULL && tokens[2] != NULL && (game->isSolved) == 0) {

            /* ### X-(token1) and Y-(token2) was replaced ### */


            hint(atoi(tokens[2]), atoi(tokens[1]), game);
            return 1;

        }
        printf("Error: invalid command\n");
        return 1;

    } else if (strcmp(tokens[0], "validate") == 0) {
        if ((game->isSolved) == 0) {
            validate(game);
            return 1;
        }
        printf("Error: invalid command\n");
        return 1;
    } else if (strcmp(tokens[0], "restart") == 0) {
        restart(game);
        return 1;
    } else if (strcmp(tokens[0], "exit") == 0) {
        exitGame(game);
        return 0;
    } else {
        printf("Error: invalid command\n");
        return 1;
    }
}


/* commandChar is a pre-allocated array in the main function (of size 1024)
 * main function - while(getCommandFromUser(commandChar){}
 * toExit == 1 indicates game is over
 * */
int getCommandFromUser(gameParams *game) {
    char *tokens[1024];
    char commandChar[1024];
    int i = 0;

    if (!(fgets(commandChar, 1024, stdin))) {

        printf("Exiting...");
        freeMatrix(game->userBoard, 9);
        freeMatrix(game->solution, 9);
        freeMatrix(game->fixedValues, 9);
        free(game);
        return 0;
    }


    tokens[0] = strtok(commandChar, " \t\r\n");
    while (tokens[i] != NULL) {
        i++;
        tokens[i] = strtok(NULL, " \t\r\n");
    }

    /* at this stage tokens is a string array containing all tokens read from user's command */
    return resolveCommand(tokens, game);
}
