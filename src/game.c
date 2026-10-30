#include "game.h"
#include "field.h"

void game_update_score(Ball *ball,Racket *racket_1, Racket *racket_2 ) {
    //Ball reaches left side of the field
    bool ball_touched_left_court = ball->position.x - ball->radius <= FIELD_X;
    bool ball_touched_right_court = ball->position.x + ball->radius >= SCREEN_WIDTH - FIELD_X;
    if (ball_touched_left_court) racket_1->score++;
    else if (ball_touched_right_court) racket_2->score++;
}

void game_reset_game_values (Ball *ball, Racket *racket_1, Racket *racket_2) {
    racket_1->score = 0;
    racket_2->score = 0;

    racket_1->position.x = (float)SCREEN_WIDTH - RACKET_MARGIN - RACKET_WIDTH;
    racket_1->position.y = (float)SCREEN_HEIGHT/2;

    racket_2->position.x = RACKET_MARGIN;
    racket_2->position.y = (float)SCREEN_HEIGHT/2;

    ball->position.x = (float)SCREEN_WIDTH/2;
    ball->position.y = (float)SCREEN_HEIGHT/2;
}


void game_update_game(Ball *ball ,Racket *racket_1, Racket *racket_2) {
    racket_move(racket_1);
    racket_move(racket_2);

    ball_updatePosition(ball);

    racket_detect_collision(racket_1, ball);
    racket_detect_collision(racket_2, ball);

    game_update_score(ball, racket_1, racket_2);
}

void game_update_velocities(const ConfigOption level,Ball *ball,Racket *racket_1, Racket *racket_2) {
    switch (level) {
        case CONFIG_LEVEL_1:
            ball->velocity_x = 120.0f;
            ball->velocity_y = 120.0f;

            racket_1->velocity = 120.0f;
            racket_2->velocity = 120.0f;

            break;

        case CONFIG_LEVEL_2:
            ball->velocity_x = 220.0f;
            ball->velocity_y = 220.0f;

            racket_1->velocity = 220.0f;
            racket_2->velocity = 220.0f;

            break;

        case CONFIG_LEVEL_3:
            ball->velocity_x = 300.0f;
            ball->velocity_y = 300.0f;

            racket_1->velocity = 220.0f;
            racket_2->velocity = 220.0f;

            break;

    }
}
extern Sound selectSound;
void game_update_level(ConfigOption *level) {
    switch (*level) {
        case CONFIG_LEVEL_1:
            if (IsKeyPressed(KEY_RIGHT)){ *level = CONFIG_LEVEL_2; PlaySound(selectSound);}

            break;

        case CONFIG_LEVEL_2:
            if (IsKeyPressed(KEY_RIGHT)){ *level = CONFIG_LEVEL_3; PlaySound(selectSound);}
            else if (IsKeyPressed(KEY_LEFT)){ *level = CONFIG_LEVEL_1; PlaySound(selectSound);}
            break;

        case CONFIG_LEVEL_3:
            if (IsKeyPressed(KEY_LEFT)){ *level = CONFIG_LEVEL_2; PlaySound(selectSound);}
            break;
    }
}


void game_update_menu(const GameState g_state,MenuState *m_state) {
    if (g_state == MENU) {
        switch (*m_state) {

            case NEW_GAME:
                if (IsKeyPressed(KEY_DOWN)){ *m_state = CONFIGURATION; PlaySound(selectSound);}
                break;

            case CONFIGURATION:
                if (IsKeyPressed(KEY_UP)){ *m_state = NEW_GAME; PlaySound(selectSound);}
                else if (IsKeyPressed(KEY_DOWN)){ *m_state = EXIT; PlaySound(selectSound);}
                break;

            case EXIT:
                if (IsKeyPressed(KEY_UP)){ *m_state = CONFIGURATION; PlaySound(selectSound);}
                //else if (IsKeyPressed(KEY_ENTER)) return 0;
                break;

        }

    }
    else if (g_state == PAUSE) {
        switch (*m_state) {
            case RESUME_GAME:
                if (IsKeyPressed(KEY_DOWN)){ *m_state = NEW_GAME; PlaySound(selectSound);}
                break;

            case NEW_GAME:
                if (IsKeyPressed(KEY_UP)){ *m_state = RESUME_GAME; PlaySound(selectSound);}
                else if (IsKeyPressed(KEY_DOWN)){ *m_state = CONFIGURATION; PlaySound(selectSound);}
                break;

            case CONFIGURATION:
                if (IsKeyPressed(KEY_UP)){ *m_state = NEW_GAME; PlaySound(selectSound);}
                else if (IsKeyPressed(KEY_DOWN)){ *m_state = EXIT; PlaySound(selectSound);}
                break;

            case EXIT:
                if (IsKeyPressed(KEY_UP)){ *m_state = CONFIGURATION; PlaySound(selectSound);}
                //else if (IsKeyPressed(KEY_ENTER)) return 0;
                break;

        }

    }


}

void game_draw_menu(const GameState g_state, const MenuState m_state) {
    const Color new_game_selected = m_state == NEW_GAME? RED : WHITE;
    const Color resume_game_selected = m_state == RESUME_GAME? RED : WHITE;
    const Color configurations_selected = m_state == CONFIGURATION ? RED : WHITE;
    const Color exit_selected = m_state == EXIT ? RED : WHITE;

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

void game_draw_game(const Field field, const Ball ball, const Racket racket_1, const Racket racket_2) {
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


void game_draw_config(const ConfigOption level) {

    const int one_third_width =  SCREEN_WIDTH / 3;
    const int half_height = SCREEN_HEIGHT / 2;

    const int rec_padding = 20;
    const int rec_width = one_third_width - 2*rec_padding;
    const int rec_height = 100;

    const Color is_level_1_selected = level == CONFIG_LEVEL_1? RED : WHITE;
    const Color is_level_2_selected = level == CONFIG_LEVEL_2? RED : WHITE;
    const Color is_level_3_selected = level == CONFIG_LEVEL_3? RED : WHITE;
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