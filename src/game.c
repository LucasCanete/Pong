#include "game.h"
#include "field.h"

void game_update_score(Ball *ball,Racket *racket_1, Racket *racket_2 ) {
    //Ball reaches left side of the field
    bool ball_touched_left_court = ball->position.x - ball->radius <= FIELD_X;
    bool ball_touched_right_court = ball->position.x + ball->radius >= SCREEN_WIDTH - FIELD_X;
    if (ball_touched_left_court) racket_1->score++;
    else if (ball_touched_right_court) racket_2->score++;
}