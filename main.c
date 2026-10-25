#include <stdio.h>
#include <raylib.h>
#include "game.h"
#include "field.h"
#include "ball.h"
#include "racket.h"

void update_velocities(ConfigOption level,Ball *ball,Racket *racket_1, Racket *racket_2) {
    switch (level) {
        case CONFIG_LEVEL_1:
                ball->velocity_x = 120.0f;
                ball->velocity_y = 120.0f;
                racket_adjust_velocity(racket_1,120.0f);
                racket_adjust_velocity(racket_2,120.0f);
            break;

        case CONFIG_LEVEL_2:
            ball->velocity_x = 200.0f;
            ball->velocity_y = 200.0f;
            racket_adjust_velocity(racket_1,200.0f);
            racket_adjust_velocity(racket_2,200.0f);
            break;

        case CONFIG_LEVEL_3:
            ball->velocity_x = 200.0f;
            ball->velocity_y = 200.0f;
            racket_adjust_velocity(racket_1,240.0f);
            racket_adjust_velocity(racket_2,240.0f);

            break;

    }
}


void update_config(ConfigOption *level) {
    switch (*level) {
        case CONFIG_LEVEL_1:
                if (IsKeyPressed(KEY_RIGHT)) *level = CONFIG_LEVEL_2;

            break;

        case CONFIG_LEVEL_2:
            if (IsKeyPressed(KEY_RIGHT)) *level = CONFIG_LEVEL_3;
            else if (IsKeyPressed(KEY_LEFT)) *level = CONFIG_LEVEL_1;
            break;

        case CONFIG_LEVEL_3:
            if (IsKeyPressed(KEY_LEFT)) *level = CONFIG_LEVEL_2;
            break;
    }
}
void draw_config(ConfigOption level) {

    const int one_third_width =  SCREEN_WIDTH / 3;
    const int half_height = SCREEN_HEIGHT / 2;

    const int rec_padding = 20;
    const int rec_width = one_third_width - 2*rec_padding;
    const int rec_height = 100;

    Color is_level_1_selected = level == CONFIG_LEVEL_1? RED : WHITE;
    Color is_level_2_selected = level == CONFIG_LEVEL_2? RED : WHITE;
    Color is_level_3_selected = level == CONFIG_LEVEL_3? RED : WHITE;
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Level 1",rec_padding + rec_width/6,half_height - rec_height/4,50, is_level_1_selected);
        DrawText("Level 2",one_third_width  + rec_width/6,half_height - rec_height/4,50, is_level_2_selected);
        DrawText("Level 3",2*one_third_width  + rec_width/6,half_height - rec_height/4,50, is_level_3_selected);

        //Draw Rectangless
        DrawRectangleLines(rec_padding,half_height - rec_height/2,rec_width,rec_height,is_level_1_selected);
        DrawRectangleLines(one_third_width + rec_padding,half_height - rec_height/2,rec_width,rec_height,is_level_2_selected);
        DrawRectangleLines(2*one_third_width + rec_padding,half_height - rec_height/2,rec_width,rec_height,is_level_3_selected);
    EndDrawing();
}

//modify: different behaviour if gamestate is menu or pause
void update_menu(GameState g_state,MenuState *m_state) {
    if (g_state == MENU) {
        switch (*m_state) {
            // case RESUME_GAME:
            //     if (IsKeyPressed(KEY_DOWN)) *m_state = NEW_GAME;
            //     break;

            case NEW_GAME:
                if (IsKeyPressed(KEY_DOWN)) *m_state = CONFIGURATION;
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
    else if (g_state == PAUSE) {
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
                DrawText("PAUSE", 2*SCREEN_WIDTH/5, SCREEN_HEIGHT/6, 50, WHITE);
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
    GameState previousGameState = MENU;
    MenuState menuState = NEW_GAME;
    ConfigOption config = CONFIG_LEVEL_1;

    while (!WindowShouldClose()) {
        // =================================== Update Logic =======================================
        switch (gameState) {

            case MENU:
                update_menu(gameState,&menuState);

                if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    reset_game_values(&ball, &racket_1, &racket_2);
                    previousGameState = MENU;
                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    previousGameState = MENU;
                    gameState = CONFIG;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                        return 0;
                }

                break;

            case GAMEPLAY:

                update_game(&ball, &racket_1, &racket_2);

                if (IsKeyPressed(KEY_SPACE)) {
                    TraceLog(LOG_INFO, "Cambiando a MENU");
                    previousGameState = GAMEPLAY;
                    gameState = PAUSE;
                    menuState = RESUME_GAME;
                }
                if (racket_1.score == 11 || racket_2.score == 11) {
                    gameState = MENU;
                }

                break;

            case PAUSE:
                update_menu(gameState,&menuState);
                if ( (IsKeyPressed(KEY_ENTER) && menuState == RESUME_GAME)) {
                    TraceLog(LOG_INFO, "Cambiando a GAMEPLAY");
                    previousGameState = PAUSE;
                    gameState = GAMEPLAY;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == NEW_GAME) {
                    //reset game values
                    reset_game_values(&ball, &racket_1, &racket_2);
                    previousGameState = PAUSE;
                    gameState = GAMEPLAY;

                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == CONFIGURATION) {
                    previousGameState = PAUSE;
                    gameState = CONFIG;
                }
                else if (IsKeyPressed(KEY_ENTER) && menuState == EXIT) {
                    return 0;
                }

                break;

            case CONFIG:
                update_config(&config);
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    update_velocities(config,&ball,&racket_1, &racket_2);
                    gameState = previousGameState; //Go to Pause or Menu
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

            case CONFIG:
                draw_config(config);
                break;
        }

    }
    return 0;
}


