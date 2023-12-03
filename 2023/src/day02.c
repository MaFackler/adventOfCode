#include "advent.h"

typedef struct Game {
    int red;
    int green;
    int blue;
} Game;

bool game_possible(Game *game, int r, int g, int b) {
    return game->red <= r && game->green <= g && game->blue <= b;
}

int main() {
    int res = 0;
    char *line = NULL;
    while (readline(&line)) {
        int id = 0;
        char *games_string = buffer(1024);
        sscanf(line, "Game %d:%[^\n]", &id, games_string);
        // print("Game id=%d", id);        
        int min_red = 0;
        int min_green = 0;
        int min_blue = 0;
        for strsplit(game_string, games_string, ";") {
            Game game = {0};
            for strsplit(roll, game_string, ",") {
                // print("%s", roll);
                char *color = buffer(16);
                int value;
                sscanf(roll, " %d %s", &value, color);
                if (streql(color, "red")) {
                    game.red = value;
                } else if (streql(color, "green")) {
                    game.green = value;
                } else if (streql(color, "blue")) {
                    game.blue = value;
                } else {
                    INVALID;
                }
            }
            // print("Game(%d, %d, %d)", game.red, game.green, game.blue);
            min_red = MAX(min_red, game.red);
            min_blue = MAX(min_blue, game.blue);
            min_green = MAX(min_green, game.green);
            if (!game_possible(&game, 12, 13, 14)) {
                // print("not possible %d", id);
                // goto skip;
            }
        }
        // print("min(%d, %d, %d)", min_red, min_blue, min_green);
        // res += id;
        res += min_red * min_blue * min_green;
// skip:
    }

    print("%d", res);
}
