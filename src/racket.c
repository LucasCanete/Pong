
#include <raylib.h>
#include "racket.h"
#include "ball.h"
#include "game.h"

void racket_move(Racket *racket) {
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


//Bug: When ball collides with upper or lower horizontal part of the racket then it either slides up or down lagging several times
void racket_detect_collision(Racket *racket, Ball *ball) {
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

void racket_adjust_velocity(Racket *racket, float new_velocity) {
    racket->velocity = new_velocity;
};