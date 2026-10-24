#include <stdio.h>
#include <raylib.h>
#include "game.h"
#include "field.h"
#include "ball.h"
#include "racket.h"


void update_menu(MenuState *m_state) {
    switch (*m_state) {
        case RESUME_GAME:
            if (IsKeyPressed(KEY_DOWN)) *m_state = NEW_GAME;
            break;

        case NEW_GAME:
            if (IsKeyPressed(KEY_UP)) *m_state = RESUME_GAME;
            else if (IsKeyPressed(KEY_DOWN)) *m_state = CONFIGURATION;
            break;

        case CONFIGURATION:
            if (IsKeyPressed(KEY_UP)) *m_state = NEW_GAME;
            else if (IsKeyPressed(KEY_DOWN)) *m_state = EXIT;
            break;

        case EXIT:
            if (IsKeyPressed(KEY_UP)) *m_state = CONFIGURATION;
            //else if (IsKeyPressed(KEY_ENTER)) return 0;
            break;

    }

}

void draw_menu(GameState g_state, MenuState m_state) {
    Color new_game_selected = m_state == NEW_GAME? RED : WHITE;
    Color resume_game_selected = m_state == RESUME_GAME? RED : WHITE;
    Color configurations_selected = m_state == CONFIGURATION ? RED : WHITE;
    Color exit_selected = m_state == EXIT ? RED : WHITE;

    //shows two kinds of menus according if game has been paused or it is a new game
    switch (g_state) {
        case MENU:
            BeginDrawing();
                ClearBackground(BLACK);
                DrawText("MENU", 2*SCREEN_WIDTH/5, SCREEN_HEIGHT/6, 50, WHITE);
                DrawText("New Game", 2*SCREEN_WIDTH/5, 2*SCREEN_HEIGHT/6, 30, new_game_selected);
                DrawText("Configurations", 2*SCREEN_WIDTH/5, 3*SCREEN_HEIGHT/6, 30, configurations_selected);
                DrawText("Exit", 2*SCREEN_WIDTH/5, 4*SCREEN_HEIGHT/6, 30, exit_selected);
            EndDrawing();
            break;
        case PAUSE:
            BeginDrawing();
                ClearBackground(BLACK);
                DrawText("MENU", 2*SCREEN_WIDTH/5, SCREEN_HEIGHT/6, 50, WHITE);
                DrawText("Resume Game", 2*SCREEN_WIDTH/5, 2*SCREEN_HEIGHT/6, 30, resume_game_selected);
                DrawText("New Game", 2*SCREEN_WIDTH/5, 3*SCREEN_HEIGHT/6, 30, new_game_selected);
                DrawText("Configurations", 2*SCREEN_WIDTH/5, 4*SCREEN_HEIGHT/6, 30, configurations_selected);
                DrawText("Exit", 2*SCREEN_WIDTH/5, 5*SCREEN_HEIGHT/6, 30, exit_selected);
            EndDrawing();
            break;
    }

}
void draw_game(Field field, Ball ball, Racket racket_1, Racket racket_2) {
    BeginDrawing();
        ClearBackground(BLACK);

        //Ball
        DrawCircleV(ball.position, ball.radius, WHITE);

        //Racket
        DrawRectangle((int)racket_1.position.x, (int)racket_1.position.y, racket_1.width, racket_1.height, WHITE); // Racket 1 (Right)
        DrawRectangle((int)racket_2.position.x, (int)racket_2.position.y, racket_2.width, racket_2.height, WHITE); // Racket 2 (Left)

        //Field
        DrawRectangleLinesEx(field.rec,field.thickness,field.color);
        DrawLine(SCREEN_WIDTH/2,FIELD_Y,SCREEN_WIDTH/2,SCREEN_HEIGHT - FIELD_Y,WHITE);

        // Score
        DrawText(TextFormat("%02i", racket_1.score), 3*(SCREEN_WIDTH/4), 5*FIELD_Y, 50, WHITE); //player 1
        DrawText(TextFormat("%02i", racket_2.score), (SCREEN_WIDTH/4), 5*FIELD_Y, 50, WHITE); //player 2

    EndDrawing();
}
void update_game(Ball *ball ,Racket *racket_1, Racket *racket_2) {
    racket_move(racket_1);
    racket_move(racket_2);

    ball_updatePosition(ball);

    racket_detect_collision(racket_1, ball);
    racket_detect_collision(racket_2, ball);

    game_update_score(ball, racket_1, racket_2);
}

void reset_game_values (Ball *ball, Racket *racket_1, Racket *racket_2) {
    racket_1->score = 0;
    racket_2->score = 0;

    ball->position.x = (float)SCREEN_WIDTH/2;
    ball->position.y = (float)SCREEN_HEIGHT/2;
}

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
    Racket racket_1 = {
        .position = {(float)screenWidth - RACKET_MARGIN - RACKET_WIDTH, (float)screenHeight/2},
        .width = RACKET_WIDTH,
        .height = 60,
        .velocity = 120.0f,
        .player = PLAYER_1,
        .score = 0
    };

    Racket racket_2 = {
        .position = {RACKET_MARGIN,(float)screenHeight/2},
        .width = RACKET_WIDTH,
        .height = 60,
        .velocity = 120.0f,
        .player = PLAYER_2,
        .score = 0
    };

    // ------------- define ball ----------------------

    Ball ball = {
        .position = {(float)screenWidth/2, (float)screenHeight/2},
        .radius = 10.0f,
        .velocity_x = 120.0f,
        .velocity_y = 120.0f,
    };

    GameState gameState = MENU;
    MenuState menuState = NEW_GAME;

    while (!WindowShouldClose()) {
        // =================================== Update Logic =======================================
        switch (gameState) {

            case MENU:
                update_menu(&menuState);

                if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    reset_game_values(&ball, &racket_1, &racket_2);

                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    //CONFIGURE SOMETHING
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                        return 0;
                }

                break;

            case GAMEPLAY:
                update_game(&ball, &racket_1, &racket_2);
                if (IsKeyPressed(KEY_SPACE)) {
                    TraceLog(LOG_INFO, "Cambiando a MENU");
                    gameState = PAUSE;
                    menuState = RESUME_GAME;
                }
                if (racket_1.score == 11 || racket_2.score == 11) {
                    gameState = MENU;
                }

                break;
            case PAUSE:
                update_menu(&menuState);
                if ( (IsKeyPressed(KEY_ENTER) && menuState == RESUME_GAME)) {
                    TraceLog(LOG_INFO, "Cambiando a GAMEPLAY");
                    gameState = GAMEPLAY;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    reset_game_values(&ball, &racket_1, &racket_2);

                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    //CONFIGURE SOMETHING
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                    return 0;
                }

                break;
        }


        // =================================== Drawing Logic =======================================

        switch (gameState) {
            case MENU:
                draw_menu(gameState,menuState);
                break;

            case GAMEPLAY:
                draw_game(field, ball, racket_1, racket_2);
                break;

            case PAUSE:
                draw_menu(gameState,menuState);
                break;
        }

    }
    return 0;
}


