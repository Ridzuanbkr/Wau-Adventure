//Game Loop & Spawner engine
#include "raylib.h"
#include "Player.hpp"
#include "Entity.hpp"
#include <vector>
#include <memory>
#include <cstdlib>

enum class GameState {
    MENU,
    TUTORIAL,
    PLAYING,
    GAMEOVER
};

int main() {
    // 9:16 Aspect Ratio for Mobile Devices
    const int screenWidth = 450;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Malaysia Day: Wau Adventure");
    SetTargetFPS(60);

    GameState currentState = GameState::MENU;
    Player player(screenWidth / 2.0f - 20.0f, screenHeight - 120.0f);

    std::vector<std::unique_ptr<Entity>> entities;
    float spawnTimer = 0.0f;
    float gameTime = 0.0f;
    int highScore = 0;

    Rectangle startButton = {
        screenWidth / 2.0f - 90.0f, 400.0f, 180.0f, 50.0f
    };

    Rectangle quitButton = {
        screenWidth / 2.0f - 90.0f, 480.0f, 180.0f, 50.0f
    };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Vector2 mousePoint = GetMousePosition();

        switch (currentState) {
            case GameState::MENU: {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePoint, startButton)) {
                        currentState = GameState::TUTORIAL;
                    } else if (CheckCollisionPointRec(mousePoint, quitButton)) {
                        CloseWindow();
                        return 0;
                    }
                }
                break;
            }

            case GameState::TUTORIAL: {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    player.Reset(screenWidth / 2.0f - 20.0f, screenHeight - 120.0f);
                    entities.clear();
                    gameTime = 0.0f;
                    currentState = GameState::PLAYING;
                }
                break;
            }

            case GameState::PLAYING: {
                gameTime += deltaTime;

                //Difficulty scaling engine
                //Every 20 seconds, add +0.5x scaling
                int difficultyLevel = static_cast<int>(gameTime / 20.0f);

                //Speed Multiplier (capped at 3.0x)
                float curreSpeedMult = 1.0f + (difficultyLevel * 0.4f);
                if (curreSpeedMult > 3.0f) curreSpeedMult = 3.0f;

                //Cloud Size Multiplier (capped at 2.5x)
                float currentCLoudSizeMult = 1.0f + (difficultyLevel * 0.3f);
                if (currentCLoudSizeMult > 2.5f) currentCLoudSizeMult = 2.5f;

                // Spawn interval scaling: shrink faster so more entity appear as speed increases
                //Start at 0.60 seconds, decrease by 0.08 seconds per difficulty level, with a minimum of 0.18 seconds
                float currentSpawnInterval = 0.60f - (difficultyLevel * 0.08f);
                if (currentSpawnInterval < 0.18f) currentSpawnInterval = 0.18f;

                // Spawner Logic
                spawnTimer += deltaTime;
                if (spawnTimer >= currentSpawnInterval) {
                    spawnTimer = 0.0f;
                    float spawnX = static_cast<float>(rand() % (screenWidth - 60));
                    int roll = rand() % 100;

                    EntityType type;
                    if (roll < 55) {
                        type = EntityType::STAR;
                    } else if (roll < 80) {
                        type = EntityType::OBSTACLE;
                    } else {
                        type = EntityType::HIBISCUS;
                    }

                    entities.push_back(std::make_unique<Entity>(spawnX, -50.0f, 220.0f, curreSpeedMult, currentCLoudSizeMult, type));
                }

                player.Update(deltaTime, screenWidth, screenHeight);

                for (auto& entity : entities) {
                    entity->Update(deltaTime, screenHeight);

                    if (entity->IsActive() && CheckCollisionRecs(player.GetBounds(), entity->GetBounds())) {
                        entity->Deactivate();

                        if (entity->GetType() == EntityType::STAR) {
                            player.AddScore(10);
                        } else if (entity->GetType() == EntityType::HIBISCUS) {
                            player.ApplySpeedBoost(14.0f);
                        } else {
                            player.TakeDamage(1);
                        }
                    }
                }

                for (auto it = entities.begin(); it != entities.end();) {
                    if (!(*it)->IsActive()) {
                        it = entities.erase(it);
                    } else {
                        ++it;
                    }
                }

                if (player.GetHealth() <= 0) {
                    if (player.GetScore() > highScore) {
                        highScore = player.GetScore();
                    }
                    currentState = GameState::GAMEOVER;
                }
                break;
            }

            case GameState::GAMEOVER: {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    currentState = GameState::MENU;
                }
                break;
            }
        }

        BeginDrawing();
        ClearBackground(DARKBLUE);

        if (currentState == GameState::MENU) {
            DrawText("Malaysia Day", screenWidth / 2 - MeasureText("Malaysia Day", 32) / 2, 220, 32, YELLOW);
            DrawText("Wau Adventure", screenWidth / 2 - MeasureText("Wau Adventure", 18) / 2, 260, 18, YELLOW);
            DrawRectangleRec(startButton, CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN);
            DrawText("Start Game", startButton.x + 25, startButton.y + 15, 20, BLACK);
            DrawRectangleRec(quitButton, CheckCollisionPointRec(mousePoint, quitButton) ? RED : RAYWHITE);
            DrawText("Quit", quitButton.x + 65, quitButton.y + 15, 20, BLACK);
        } else if (currentState == GameState::TUTORIAL) {
            DrawText("HOW TO PLAY?", screenWidth / 2 - MeasureText("HOW TO PLAY?", 28) / 2, 60, 28, YELLOW);
            DrawText("CONTROLS:", 30, 120, 20, RAYWHITE);
            DrawText("Use W/A/S/D or Arrow Keys to move the Wau.", 30, 160, 16, LIGHTGRAY);
            DrawText("OBJECTIVE & ITEMS:", 30, 210, 20, RAYWHITE);

            DrawCircle(50, 260, 10, GOLD);
            DrawText("Star (+10 points)", 75, 252, 18, GOLD);
            DrawText("Collect Stars for points.", 75, 275, 14, RAYWHITE);

            DrawRectangle(40, 395, 24, 24, GRAY);
            DrawText("Cloud (Dangerous)", 75, 392, 18, RED);
            DrawText("Avoid Clouds! They reduce health by 1 point.", 75, 415, 14, RAYWHITE);

            DrawCircle(50, 330, 12, PINK);
            DrawText("Hibiscus (Speed Boost; 2.2x speed)", 75, 322, 18, PINK);
            DrawText("Collect Hibiscus for a speed boost for 14 seconds!", 75, 345, 14, RAYWHITE);

            DrawRectangle(30, 680, 390, 60, RAYWHITE);
            DrawText("Press ENTER or SPACE to start playing.", screenWidth / 2 - MeasureText("Press ENTER or SPACE to start playing.", 20) / 2, 695, 20, DARKBLUE);
            
        } else if (currentState == GameState::PLAYING) {
            player.Draw();
            for (auto& entity : entities) {
                entity->Draw();
            }

            DrawText(TextFormat("Score: %d", player.GetScore()), 10, 10, 18, YELLOW);
            DrawText(TextFormat("Wau Health: %d", player.GetHealth()), 10, 32, 18, RAYWHITE);
            DrawText(TextFormat("Time: %.1f", gameTime), 10, 54, 18, RAYWHITE);

            if (player.HasSpeedBoost()) {
                DrawText(TextFormat("Speed Boost: %.1f", player.GetBoostTimeRemaining()), 10, 76, 18, ORANGE);
            }
        } else if (currentState == GameState::GAMEOVER) {
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 36) / 2, 200, 36, RED);
            DrawText(TextFormat("Final Score: %d", player.GetScore()), screenWidth / 2 - 70, 290, 20, YELLOW);
            DrawText(TextFormat("High Score: %d", highScore), screenWidth / 2 - 70, 330, 20, YELLOW);
            DrawText(TextFormat("Time Survived:", gameTime), screenWidth / 2 - 70, 370, 20, YELLOW);
            DrawText(TextFormat("%.1f seconds", gameTime), screenWidth / 2 - 65, 400, 20, YELLOW);
            DrawText("Press ENTER or SPACE", screenWidth / 2 - MeasureText("Press ENTER or SPACE", 18) / 2, 500, 18, RAYWHITE);
            DrawText("to return to Menu", screenWidth / 2 - MeasureText("to return to Menu", 18) / 2, 525, 18, RAYWHITE);
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                currentState = GameState::MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
