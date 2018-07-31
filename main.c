#include <stdio.h>
#include "parser.h"


int main(int argc, char *argv[]) {

    int seed;
    gameParams *game;

    seed = 5;
    if (atoi(argv[argc - 1]) != 0) {
        seed = atoi(argv[argc - 1]);
    }
    srand(seed);

    game = (gameParams *) malloc(sizeof(gameParams));
    if (game == NULL) {
        printf("Error: malloc has failed\n");
        free(game);
        exit(0);
    }
    createNewGame(game);
    if (!(initGame(game))) {
        return 0;
    }

    while (getCommandFromUser(game)) {}


    return 0;
}





