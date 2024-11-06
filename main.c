#include "include/game.h"
#include "include/raylib.h"
#include <stdbool.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(void) {
    InitWindow(800, 600, "Game");
    SetTargetFPS(60);
    SetRandomSeed(GetTime());
    Game *game = MemAlloc(sizeof(Game));

    startGame(game);


    while (!WindowShouldClose()) {
        BeginDrawing();
        UpdateGame(game);
        DrawGame(game);
        EndDrawing();
    }

    CloseWindow();
    nob_da_free(game->bricks);
    MemFree(game);
    return 0;
}

void UpdateGame(Game *game) {
    float dt = GetFrameTime();
    if (IsKeyPressed(KEY_R)) {
        startGame(game);
        game->over = false;
        return;
    }

    if (game->over) {
        game->ball.rect.x = ((float)GetScreenWidth()/2)-BALL_WIDTH/2;
        game->ball.rect.y = (float)GetScreenHeight()/2;
        game->ball.vel = (Vector2){BALL_VEL, BALL_VEL};
        game->bricks.count = 0;
        return;
    }

    if (IsKeyDown(KEY_A)) {
        if (game->pad.rect.x > 0)
            game->pad.rect.x -= PAD_VEL_X;
    }else if (IsKeyDown(KEY_D)) {
        if(game->pad.rect.x + PAD_WIDTH <= GetScreenWidth())
            game->pad.rect.x += PAD_VEL_X;
    }else if (IsKeyPressed(KEY_SPACE)) {
        game->ball.control = !game->ball.control;
    }

    if(game->ball.control){
        Vector2 mouse_pos = GetMousePosition();
        game->ball.rect.y = mouse_pos.y;
        game->ball.rect.x = mouse_pos.x;
        return;
    }

    if(game->ball.rect.y + game->ball.rect.height > GetScreenHeight() ){
            game->over=true;
            return;
        }

    if (game->ball.rect.y < 0 ) {
        game->ball.vel.y *= -1;
    }

    if (game->ball.rect.x + game->ball.rect.width > GetScreenWidth() || game->ball.rect.x < 0) {
        game->ball.vel.x *= -1;
    }

    if(CheckCollisionRecs(game->ball.rect, game->pad.rect)){
        game->ball.vel.y *= -1;
    }

    for (size_t i = 0; i<game->bricks.count; ++i) {
        if(game->bricks.items[i].dead) continue;
        Rectangle rec = GetCollisionRec(game->bricks.items[i].rect, game->ball.rect);
        if (rec.y == game->ball.rect.y ){
            game->bricks.items[i].dead = true;
            game->ball.vel.y *= -1;
        } else  if (rec.x == game->ball.rect.x ){
            game->bricks.items[i].dead = true;
            game->ball.vel.x *= -1;
        }
    }

    game->ball.rect.y += game->ball.vel.y * dt;
    game->ball.rect.x += game->ball.vel.x * dt;


}

void startGame(Game* game){
    game->background = BLACK;
    game->pad = (Pad){(Rectangle){((float)GetScreenWidth()/2)-PAD_WIDTH/2, GetScreenHeight()-50,  PAD_WIDTH, PAD_HEIGTH}, RED};
    game->ball = (Ball){(Rectangle){((float)GetScreenWidth()/2)-BALL_WIDTH/2, (float)GetScreenHeight()/2,  BALL_WIDTH, BALL_HEIGTH}, (Vector2){BALL_VEL, BALL_VEL}, RED};
    int line_count = 0;
    int brick_count = 0;
    for(int i = 0; i<BRICK_COUNT; ++i){
        if(brick_count >= BRICK_COUNT_PER_LINE){
            brick_count = 0;
            line_count += 1;
        }

        float x = BRICK_GAP + brick_count + (brick_count * (BRICK_WIDTH + BRICK_GAP));
        float y = BRICK_GAP + (line_count * (BRICK_HEIGTH + BRICK_GAP));

        Brick brick = {
            (Rectangle){
                .height=BRICK_HEIGTH,
                .width=BRICK_WIDTH,
                .x=x,
                .y=y
            },
        };

        nob_da_append(&game->bricks, brick);

        brick_count++;
    }
}

void DrawGame(Game *game) {
    ClearBackground(game->background);
    if (game->over) {
        DrawText("Game Over", (GetScreenWidth() / 2) - 150, GetScreenHeight() / 2,
                50, WHITE);
        return;
    }
    for (size_t i = 0; i<game->bricks.count; ++i) {
        if(game->bricks.items[i].dead)continue;
        DrawRectangleRec(game->bricks.items[i].rect, RED);
    }
    DrawRectangleRec(game->ball.rect, game->ball.color);
    DrawRectangleRec(game->pad.rect, game->pad.color);
}
