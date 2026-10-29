#ifndef BALL_H
#define BALL_H
#include <raylib.h>

typedef struct {
    Vector2 position;
    float radius;
    float velocity_x;
    float velocity_y;
} Ball;

void ball_init(Ball *ball);

void ball_updatePosition(Ball *ball);





#endif