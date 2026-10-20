#include <stdio.h>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FIELD_X 10
#define FIELD_Y 10

// ====================================== RACKET =================================================
typedef struct {
    Vector2 position;
    int width;
    int height;
    float velocity;
} Racket;

void moveRacket(Racket *racket) {
    if (IsKeyDown(KEY_DOWN)) {
        if (racket->position.y + (float)racket->height >= SCREEN_HEIGHT - FIELD_Y) {
            racket->position.y = (float)(SCREEN_HEIGHT - FIELD_Y) - (float)racket->height;
        }
        else {
            racket->position.y += racket->velocity * GetFrameTime();
        }

    }
    if (IsKeyDown(KEY_UP)) {
        if (racket->position.y <= FIELD_Y) {
            racket->position.y = FIELD_Y;
        }
        else {
            racket->position.y -= racket->velocity * GetFrameTime();
        }

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
    if (ball->position.x - ball->radius <= FIELD_X || ball->position.x + ball->radius >= SCREEN_WIDTH - 2*FIELD_X  ) {
        ball->velocity_x *= -1;
    }
    //changes on y position when colliding with wall
    if (ball->position.y - ball->radius <= FIELD_Y || ball->position.y + ball->radius >= SCREEN_HEIGHT - 2*FIELD_Y ) {
        ball->velocity_y *= -1;
    }

}

void detectCollision(Racket *racket, Ball *ball) {
    bool ball_and_racket_at_same_x = ball->position.x + ball->radius >= racket->position.x ? true : false;
    bool ball_at_height_of_racket = ball->position.y  >= racket->position.y && ball->position.y <= racket->position.y + (float)racket->height;

    // detect collision with racket on left field-side
    if (ball_and_racket_at_same_x && ball_at_height_of_racket) {
        //only modify velocity vector in x
        ball->velocity_x *= -1;

    }
}

// =================================== FIELD ========================================================
typedef struct {
    Rectangle rec;
    float thickness;
    Color color;
} Field;

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

    // ------------- define racket ------------------
    Racket racket = {
        .position = {(float)screenWidth - 3*FIELD_X, (float)screenHeight/2},
        .width = 10,
        .height = 60,
        .velocity = 120.0f,
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
        updateBall(&ball);
        detectCollision(&racket, &ball);

        //Update Drawings
        BeginDrawing();
            ClearBackground(BLACK);


            //Ball
            DrawCircleV(ball.position, ball.radius, WHITE);

            //Field
            DrawRectangleLinesEx(field.rec,field.thickness,field.color);
            DrawLine(screenWidth/2,FIELD_Y,screenWidth/2,screenHeight - FIELD_Y,WHITE);

            //Racket
            DrawRectangle((int)racket.position.x, (int)racket.position.y, racket.width,racket.height, WHITE);
        EndDrawing();
    }
    return 0;
}
