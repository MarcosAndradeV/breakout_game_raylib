#ifndef GAME_H_
#define GAME_H_

#include "raylib.h"
#include "stddef.h"

#define BALL_VEL 300.0f
#define BALL_WIDTH 20.0f
#define BALL_HEIGTH 20.0f

#define PAD_VEL_X 10.0f
#define PAD_WIDTH 150.0f
#define PAD_HEIGTH 20.0f

#define BRICK_WIDTH 80.0f
#define BRICK_HEIGTH 20.0f
#define BRICK_COUNT 32
#define BRICK_COUNT_PER_LINE  8
#define BRICK_GAP 18

typedef struct {
    Rectangle rect;
    Color color;
    bool dead;
} Brick;

typedef struct {
    Brick * items;
    size_t capacity, count;
} Bricks;

typedef struct {
    Rectangle rect;
    Color color;
} Pad;

typedef struct {
    Rectangle rect;
    Vector2 vel;
    Color color;
    bool control;
} Ball;

typedef struct {
    Color background;
    Pad pad;
    Ball ball;
    Bricks bricks;
    bool over;
} Game;

void UpdateGame(Game* game);
void DrawGame(Game* game);
void startGame(Game* game);

#endif // GAME_H_
