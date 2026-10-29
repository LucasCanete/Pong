#include <stdio.h>
#include <raylib.h>
#include "game.h"
#include "field.h"
#include "ball.h"
#include "racket.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);


    // ------------ define field --------------------
    Field field = {
        .rec = {
            .x = FIELD_X,
            .y = FIELD_Y,
            .width = (float)screenWidth - 2*FIELD_X,
            .height = (float)screenHeight- 2*FIELD_Y,
        },
        .thickness = 3.0f,
        .color =  RAYWHITE
    };

    // ------------- define rackets ------------------
    Racket racket_1 = {};

    Racket racket_2 = {};

    racket_init(&racket_1, PLAYER_1);

    racket_init(&racket_2, PLAYER_2);

    // ------------- define ball ----------------------

    Ball ball = {};
    ball_init(&ball);

    GameState gameState = MENU;
    GameState previousGameState = MENU;
    MenuState menuState = NEW_GAME;
    ConfigOption config = CONFIG_LEVEL_1;

    while (!WindowShouldClose()) {
        // =================================== Update Logic =======================================
        switch (gameState) {

            case MENU:
                game_update_menu(gameState,&menuState);

                if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    TraceLog(LOG_INFO, "Changing to GAMEPLAY");
                    game_reset_game_values(&ball, &racket_1, &racket_2);
                    previousGameState = MENU;
                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    TraceLog(LOG_INFO, "Changing to CONFIG");
                    previousGameState = MENU;
                    gameState = CONFIG;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                        return 0;
                }

                break;

            case GAMEPLAY:

                game_update_game(&ball, &racket_1, &racket_2);

                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) {
                    TraceLog(LOG_INFO, "Changing to PAUSE");

                    previousGameState = GAMEPLAY;
                    gameState = PAUSE;
                    menuState = RESUME_GAME;
                }
                if (racket_1.score == 11 || racket_2.score == 11) {
                    TraceLog(LOG_INFO, "Changing to MENU");
                    gameState = MENU;
                }

                break;

            case PAUSE:
                game_update_menu(gameState,&menuState);
                if ( (IsKeyPressed(KEY_ENTER) && menuState == RESUME_GAME)) {
                    TraceLog(LOG_INFO, "Changing to GAMEPLAY");

                    previousGameState = PAUSE;
                    gameState = GAMEPLAY;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    TraceLog(LOG_INFO, "Changing to GAMEPLAY");

                    game_reset_game_values(&ball, &racket_1, &racket_2);
                    previousGameState = PAUSE;
                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    TraceLog(LOG_INFO, "Changing to CONFIG");
                    previousGameState = PAUSE;
                    gameState = CONFIG;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                    TraceLog(LOG_INFO, "Changing to EXIT");
                    return 0;
                }

                break;

            case CONFIG:
                game_update_level(&config);
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    const char *st = previousGameState == 1 ? "Changing to MENU" : "Changing to PAUSE";
                    TraceLog(LOG_INFO, "%s", st);

                    game_update_velocities(config,&ball,&racket_1, &racket_2);
                    gameState = previousGameState; //Go to Pause or Menu
                }


                break;
        }


        // =================================== Drawing Logic =======================================

        switch (gameState) {
            case MENU:
                game_draw_menu(gameState,menuState);
                break;

            case GAMEPLAY:
                game_draw_game(field, ball, racket_1, racket_2);
                break;

            case PAUSE:
                game_draw_menu(gameState,menuState);
                break;

            case CONFIG:
                game_draw_config(config);
                break;
        }

    }
    return 0;
}


