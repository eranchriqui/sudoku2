#ifndef PARSER_H
#define PARSER_H

#include "solver.h"
#include "game.h"

/* commandChar is a pre-allocated array in the main function (of size 1024)
 * main function - while(getCommandFromUser(commandChar){}
 * toExit == 1 indicates game is over
 * */
int getCommandFromUser(gameParams *game);


/* gets a tokenized command and calls relevant function
 */
int resolveCommand(char *tokens[1024], gameParams *game);


#endif /* PARSER_H */
