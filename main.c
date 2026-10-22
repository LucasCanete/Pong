#include <stdio.h>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


//Padding of x
#define FIELD_X 10

//Padding of y
#define FIELD_Y 10

#define RACKET_MARGIN (2*FIELD_X)
#define RACKET_WIDTH 10

typedef enum {
    PLAYER_1,
    PLAYER_2,
} Player;


// ====================================== RACKET =================================================
typedef struct {
    Vector2 position;
    int width;
    int height;
    float velocity;
    Player player;
    int score;
} Racket;

void moveRacket(Racket *racket) {
    switch (racket->player) {
        case PLAYER_1:
            //Go Down
            if (IsKeyDown(KEY_DOWN)) {
                if (racket->position.y + (float)racket->height >= SCREEN_HEIGHT - FIELD_Y) {
                    racket->position.y = (float)(SCREEN_HEIGHT - FIELD_Y) - (float)racket->height;
                }
                else {
                    racket->position.y += racket->velocity * GetFrameTime();
                }
            }
            //Go Up
            if (IsKeyDown(KEY_UP)) {
                if (racket->position.y <= FIELD_Y) {
                    racket->position.y = FIELD_Y;
                }
                else {
                    racket->position.y -= racket->velocity * GetFrameTime();
                }
            }
            break;

        case PLAYER_2:
            //Go Down
            if (IsKeyDown(KEY_S)) {
                if (racket->position.y + (float)racket->height >= SCREEN_HEIGHT - FIELD_Y) {
                    racket->position.y = (float)(SCREEN_HEIGHT - FIELD_Y) - (float)racket->height;
                }
                else {
                    racket->position.y += racket->velocity * GetFrameTime();
                }
            }
            //Go Up
            if (IsKeyDown(KEY_W)) {
                if (racket->position.y <= FIELD_Y) {
                    racket->position.y = FIELD_Y;
                }
                else {
                    racket->position.y -= racket->velocity * GetFrameTime();
                }
            }
            break;
    }

}

// ==================================== BALL ========================================================

typedef struct {
    Vector2 position;
    float radius;
    float velocity_x;
    float velocity_y;
} Ball;

void updateBall(Ball *ball) {
    ball->position.x += ball->velocity_x * GetFrameTime();
    ball->position.y += ball->velocity_y * GetFrameTime();

    //changes on x position when colliding with wall
    if (ball->position.x - ball->radius <= FIELD_X || ball->position.x + ball->radius >= SCREEN_WIDTH - FIELD_X  ) {
        ball->velocity_x *= -1;
    }
    //changes on y position when colliding with wall
    if (ball->position.y - ball->radius <= FIELD_Y || ball->position.y + ball->radius >= SCREEN_HEIGHT - FIELD_Y ) {
        ball->velocity_y *= -1;
    }

}
// ==================================== Collision with Rackets ========================================================

//Bug: When ball collides with upper or lower horizontal part of the racket then it either slides up or down lagging several times
void detectCollision(Racket *racket, Ball *ball) {
    switch (racket->player) {
        case PLAYER_1:
            //Racket on Right side
            bool ball_and_racket_at_same_x = ball->position.x + ball->radius >= racket->position.x && ball->position.x - ball->radius <= racket->position.x + (float)racket->width;
            bool ball_at_height_of_racket = ball->position.y + ball->radius  >= racket->position.y && ball->position.y - ball->radius <= racket->position.y + (float)racket->height;
            if (ball_and_racket_at_same_x && ball_at_height_of_racket && ball->velocity_x < 0){
                ball->velocity_y *= -1;
            }
            //only collide if ball is going to the right
             else if (ball_and_racket_at_same_x && ball_at_height_of_racket && ball->velocity_x > 0) {
                //only modify velocity vector in x
                ball->velocity_x *= -1;
            }

            break;

        case PLAYER_2:
            //Racket on Left side
            bool ball_and_racket_with_same_x = ball->position.x - ball->radius <= racket->position.x + (float)racket->width && ball->position.x + ball->radius >= racket->position.x;
            bool ball_at_height_of_racket_2 = ball->position.y + ball->radius >= racket->position.y && ball->position.y - ball->radius <= racket->position.y + (float)racket->height;

            if (ball_and_racket_with_same_x && ball_at_height_of_racket_2 && ball->velocity_x > 0){
                ball->velocity_y *= -1;
            }
            // detect collision with racket on left field-side
            else if (ball_and_racket_with_same_x && ball_at_height_of_racket_2 && ball->velocity_x < 0) {
                //only modify velocity vector in x
                ball->velocity_x *= -1;
            }

            break;
    }


}

// =================================== FIELD ========================================================
typedef struct {
    Rectangle rec;
    float thickness;
    Color color;
} Field;

// =================================== SCORE ========================================================

void updateScore(Ball *ball,Racket *racket_1, Racket *racket_2 ) {
    //Ball reaches left side of the field
    bool ball_touched_left_court = ball->position.x - ball->radius <= FIELD_X;
    bool ball_touched_right_court = ball->position.x + ball->radius >= SCREEN_WIDTH - FIELD_X;
    if (ball_touched_left_court) racket_1->score++;
    else if (ball_touched_right_court) racket_2->score++;
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
    Racket racket = {
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



    while (!WindowShouldClose()) {
        //Update game
        moveRacket(&racket);
        moveRacket(&racket_2);

        updateBall(&ball);

        detectCollision(&racket, &ball);
        detectCollision(&racket_2, &ball);

        updateScore(&ball, &racket, &racket_2);


        //Update Drawings
        BeginDrawing();
            ClearBackground(BLACK);


            //Ball
            DrawCircleV(ball.position, ball.radius, WHITE);

            //Racket
            DrawRectangle((int)racket.position.x, (int)racket.position.y, racket.width, racket.height, WHITE); // Racket 1 (Right)
            DrawRectangle((int)racket_2.position.x, (int)racket_2.position.y, racket_2.width, racket_2.height, WHITE); // Racket 2 (Left)

            //Field
            DrawRectangleLinesEx(field.rec,field.thickness,field.color);
            DrawLine(screenWidth/2,FIELD_Y,screenWidth/2,screenHeight - FIELD_Y,WHITE);

            // Score
            DrawText(TextFormat("%02i", racket.score), 3*(screenWidth/4), 5*FIELD_Y, 50, WHITE); //player 1
            DrawText(TextFormat("%02i", racket_2.score), (screenWidth/4), 5*FIELD_Y, 50, WHITE); //player 2

        EndDrawing();
    }
    return 0;
}
