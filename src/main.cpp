#include <iostream>
#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

struct Bird {
    Vector2 position;
    Vector2 size;
};

struct Pipe {
    Vector2 botTopPosition;
    Vector2 botTopSize;
    Vector2 botPipePosition;
    Vector2 botPipeSize;

    Vector2 topTopPosition;
    Vector2 topTopSize;
    Vector2 topPipePosition;
    Vector2 topPipeSize;
    bool passed = false;

    Color color;
};

enum Stages {
    MENU, GAME, GAME_OVER
};

const int screenWidth = 800;
const int screenHeight = 600;

Pipe spawnPipe(float x, float y) {
    Pipe pipe;
    pipe.botTopPosition = {x, y};
    pipe.botTopSize = {50, 20};
    pipe.botPipePosition = {pipe.botTopPosition.x + 12, y + pipe.botTopSize.y};
    pipe.botPipeSize = { 25, screenHeight - pipe.botPipePosition.y};

    pipe.topTopPosition = {x, y - 300};
    pipe.topTopSize = {pipe.botTopSize.x, pipe.botTopSize.y};
    pipe.topPipePosition = {pipe.topTopPosition.x + 12, 0};
    pipe.topPipeSize = { 25, pipe.topTopPosition.y};

    pipe.color = GREEN;

    return pipe;
}

bool checkCollision(Bird bird, Pipe pipe) {
    Rectangle birdRect = {bird.position.x, bird.position.y, bird.size.x, bird.size.y};
    Rectangle botTopRect = {pipe.botTopPosition.x, pipe.botTopPosition.y, pipe.botTopSize.x, pipe.botTopSize.y};
    Rectangle botPipeRect = {pipe.botPipePosition.x, pipe.botPipePosition.y, pipe.botPipeSize.x, pipe.botPipeSize.y};
    Rectangle topTopRect = {pipe.topTopPosition.x, pipe.topTopPosition.y, pipe.topTopSize.x, pipe.topTopSize.y};
    Rectangle topPipeRect = {pipe.topPipePosition.x, pipe.topPipePosition.y, pipe.topPipeSize.x, pipe.topPipeSize.y};
    
    return CheckCollisionRecs(birdRect, botTopRect) || CheckCollisionRecs(birdRect, botPipeRect) || CheckCollisionRecs(birdRect, topTopRect) || CheckCollisionRecs(birdRect, topPipeRect);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Flappy Bird");

    std::vector<Pipe> pipeContainer;
    int speed = 0;
    int score = 0;
    srand(time(0));

    Stages currentStage = MENU;
    SetTargetFPS(60);

    // Setup bird
    Bird bird;
    bird.position = {screenWidth / 5, screenHeight / 2};
    bird.size = {20, 20};

    while (!WindowShouldClose()) {

        switch (currentStage) {
            case MENU:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    speed = 18;
                    score = 0;
                    bird.position = {screenWidth / 5, screenHeight / 2};
                    pipeContainer.clear();
                    pipeContainer.push_back(spawnPipe(screenWidth, rand() % (561) + 120));
                    currentStage = GAME;
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("FLAPPY BIRD", screenWidth/2 - 100, screenHeight/2 - 20, 30, ORANGE);
                DrawText("Press Enter or Space to play", screenWidth/2 - 130, screenHeight/2 + 20, 20, BLACK);
                DrawRectangle(bird.position.x, bird.position.y, bird.size.x, bird.size.y, ORANGE);
                EndDrawing(); 
                break;

            case GAME:
                if (IsKeyPressed(KEY_SPACE)) {
                    speed = 18;
                }
                
                speed -= 1;
                bird.position.y -= speed;

                for (int i = 0; i < pipeContainer.size(); i++) {
                    pipeContainer[i].botPipePosition.x -= 3;
                    pipeContainer[i].botTopPosition.x -= 3;
                    pipeContainer[i].topPipePosition.x -= 3;
                    pipeContainer[i].topTopPosition.x -= 3;
                }

                if (bird.position.y > screenHeight - bird.size.y || bird.position.y < 0) {
                    currentStage = GAME_OVER;
                }

                for (int i = 0; i < pipeContainer.size(); i++) {
                    if (checkCollision(bird, pipeContainer[i])) {
                        currentStage = GAME_OVER;
                    }

                    if (pipeContainer[i].botTopPosition.x + pipeContainer[i].botTopSize.x < bird.position.x && !pipeContainer[i].passed) {
                        score += 1;
                        pipeContainer[i].passed = true;
                    }

                    if (pipeContainer[i].botTopPosition.x + pipeContainer[i].botTopSize.x < 0) {
                        pipeContainer.erase(pipeContainer.begin() + i);
                        pipeContainer.push_back(spawnPipe(screenWidth, rand() % (561) + 120));
                    }
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawRectangle(bird.position.x, bird.position.y, bird.size.x, bird.size.y, ORANGE);
                for (int i = 0; i < pipeContainer.size(); i++) {
                    DrawRectangle(pipeContainer[i].botTopPosition.x, pipeContainer[i].botTopPosition.y, pipeContainer[i].botTopSize.x, pipeContainer[i].botTopSize.y, pipeContainer[i].color);
                    DrawRectangle(pipeContainer[i].botPipePosition.x, pipeContainer[i].botPipePosition.y, pipeContainer[i].botPipeSize.x, pipeContainer[i].botPipeSize.y, pipeContainer[i].color);
                    DrawRectangle(pipeContainer[i].topTopPosition.x, pipeContainer[i].topTopPosition.y, pipeContainer[i].topTopSize.x, pipeContainer[i].topTopSize.y, pipeContainer[i].color);
                    DrawRectangle(pipeContainer[i].topPipePosition.x, pipeContainer[i].topPipePosition.y, pipeContainer[i].topPipeSize.x, pipeContainer[i].topPipeSize.y, pipeContainer[i].color);
                }
                DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
                EndDrawing();
                break;

            case GAME_OVER:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    currentStage = MENU;
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 20, 30, ORANGE);
                DrawText("Press Space or Enter to try again", screenWidth/2 - 130, screenHeight/2 + 20, 20, BLACK);
                DrawText(TextFormat("Your score: %i", score), screenWidth/2 - 50, screenHeight/2 + 60, 20, RED);
                EndDrawing();
                break;
        }
    }

    CloseWindow();

    return 0;
}