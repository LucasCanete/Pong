
#include <raylib.h>
#include "ball.h"
#include "game.h"


void ball_init(Ball *ball) {
    ball->position.x = (float)SCREEN_WIDTH/2;
    ball->position.y = (float)SCREEN_HEIGHT/2;
    ball->radius = 10.0f;
    ball->velocity_x = 120.0f;
    ball->velocity_y = 120.0f;
}
void ball_updatePosition(Ball *ball) {
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
