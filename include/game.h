#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "racket.h"
#include "field.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


//Padding of x
#define FIELD_X 10

//Padding of y
#define FIELD_Y 10

#define RACKET_MARGIN (2*FIELD_X)
#define RACKET_WIDTH 10
#define RACKET_HEIGHT 60

// Game only has three states.
// Menu: When window first opens
// Gameplay: when playing the game
// Pause:  when game has been pause (same as menu but with option to resume game)
typedef enum {
    MENU,
    GAMEPLAY,
    PAUSE,
    CONFIG,
}GameState;

// All options shown in the Menu
//Rename MenuOptions???
typedef enum {
    NEW_GAME,
    RESUME_GAME, //Only shown when GameState is Pause
    CONFIGURATION,
    EXIT
}MenuState;

typedef enum {
    CONFIG_LEVEL_1,
    CONFIG_LEVEL_2,
    CONFIG_LEVEL_3,
}ConfigOption;


// resets all positions and scores
void game_reset_game_values (Ball *ball, Racket *racket_1, Racket *racket_2);

//Updates the score of all players
void game_update_score(Ball *ball,Racket *racket_1, Racket *racket_2 );

//when selecting a new level updates the velocities at which everything moves
void game_update_velocities(ConfigOption level,Ball *ball,Racket *racket_1, Racket *racket_2);

//updates the level of the game
void game_update_level(ConfigOption *level);

//updates the moving of rackets and ball
void game_update_game(Ball *ball ,Racket *racket_1, Racket *racket_2);

//Menu update with different behaviour if gamestate is menu or pause
void game_update_menu(GameState g_state,MenuState *m_state);

//draws the menu of the game
void game_draw_menu(GameState g_state, MenuState m_state);

//Draws the configuration window (to select the game level)
void game_draw_config(ConfigOption level);

void game_draw_game(Field field,Ball ball,Racket racket_1,Racket racket_2);
#endif