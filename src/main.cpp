//Game Loop & Spawner engine
#include "raylib.h"
#include "Player.hpp"
#include "Entity.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <memory>
#include <cstdlib>

enum class GameState {
    MENU,
    TUTORIAL,
    PLAYING,
    GAMEOVER
};

void DrawTextureScaled(const Texture2D& texture, float posX, float posY, float scaleX, float scaleY) {
    DrawTextureEx(texture, {posX, posY}, 0.0f, scaleX / texture.width, WHITE);
}
int main() {
    // 9:16 Aspect Ratio for Mobile Devices
    const int screenWidth = 450;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Malaysia Day: Wau Adventure");
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(0.5f); // Set the master volume to 50%

    // RAII Asset Loader
    TextureWrapper bgTex("assets/background_twin_tower.png");
    TextureWrapper starTex("assets/star.png");
    TextureWrapper cloudTex("assets/cloud.png");
    TextureWrapper hibiscusTex("assets/hibiscus.png");
    TextureWrapper wauTex("assets/wau.png");

    SoundWrapper starsfx("assets/star_pickup_sfx.wav");
    SoundWrapper boostsfx("assets/boost_sfx.wav");
    SoundWrapper hitsfx("assets/cloud_hit_sfx.wav");

    Music bgm = LoadMusicStream("assets/background.wav");
    if (bgm.stream.buffer != nullptr) {
        PlayMusicStream(bgm);
    }

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
        UpdateMusicStream(bgm); //Keep BGM playing
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
                            starsfx.Play(); // Play star pickup sound effect
                        } else if (entity->GetType() == EntityType::HIBISCUS) {
                            player.ApplySpeedBoost(14.0f);
                            boostsfx.Play(); // Play speed boost sound effect
                        } else {
                            player.TakeDamage(1);
                            hitsfx.Play(); // Play cloud hit sound effect
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

        if (bgTex.IsLoaded()) {
            DrawTextureScaled(*bgTex.GetPtr(), 0, 0, screenWidth, screenHeight);
        } else {
            ClearBackground(DARKBLUE);
        }
        ClearBackground(DARKBLUE);

        if (currentState == GameState::MENU) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(DARKBLUE, 0.5f)); // Dim overlay
            DrawText("Malaysia Day", screenWidth / 2 - MeasureText("Malaysia Day", 32) / 2, 220, 32, YELLOW);
            DrawText("Wau Adventure", screenWidth / 2 - MeasureText("Wau Adventure", 18) / 2, 260, 18, YELLOW);
            DrawRectangleRec(startButton, CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN);
            DrawText("Start Game", startButton.x + 25, startButton.y + 15, 20, BLACK);
            DrawRectangleRec(quitButton, CheckCollisionPointRec(mousePoint, quitButton) ? RED : RAYWHITE);
            DrawText("Quit", quitButton.x + 65, quitButton.y + 15, 20, BLACK);

        } else if (currentState == GameState::TUTORIAL) {

            DrawRectangle(20, 40, screenWidth - 40, screenHeight - 120, Fade(BLACK, 0.82f)); // Dim overlay
            DrawRectangleLines(20, 40, screenWidth - 40, screenHeight - 120, YELLOW); // Border
            DrawText("HOW TO PLAY?", 90, 60, 28, YELLOW);
            int startY = 110;
            DrawText("CONTROLS:", 40, startY, 20, LIGHTGRAY);
             DrawText("Use W/A/S/D or Arrow Keys", 40, startY + 28, 16, WHITE);

            if (wauTex.IsLoaded()) {
                float frameWidth = static_cast<float>(wauTex.Get().width) / 4.0f; // Assuming 3 frames in the sprite sheet
                float frameHeight = static_cast<float>(wauTex.Get().height);
                Rectangle src = {0.0f, 0.0f, frameWidth, frameHeight};
                Rectangle dest = {30, static_cast<float>(startY + 55), 32.0f, 32.0f}; // Adjusted size for better visibility
                DrawTexturePro(wauTex.Get(), src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
            } 
            DrawText("move Wau Kite", 85, startY + 62, 16, WHITE);
           
            int objY = startY + 110;
            DrawText("OBJECTIVE & ITEMS:", 40, objY, 20, LIGHTGRAY);
            
            int starY = objY + 35;
            if (starTex.IsLoaded()) {
                Rectangle src = {0.0f, 0.0f, static_cast<float>(starTex.Get().width), static_cast<float>(starTex.Get().height)};
                Rectangle dest = {40, static_cast<float>(starY), 28.0f, 28.0f};
                DrawTexturePro(starTex.Get(), src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
            } 
            DrawText("Star (+10 points)", 80, starY, 18, GOLD);
            DrawText("Collect Stars for points.", 80, starY + 22, 14, RAYWHITE);

            int hibY = starY + 60;
            if (hibiscusTex.IsLoaded()) {
                Rectangle src = {0.0f, 0.0f, static_cast<float>(hibiscusTex.Get().width), static_cast<float>(hibiscusTex.Get().height)};
                Rectangle dest = {40, static_cast<float>(hibY), 28.0f, 28.0f};
                DrawTexturePro(hibiscusTex.Get(), src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
            } 
            DrawText("Hibiscus (Speed Boost; 2.2x speed)", 80, hibY, 18, PINK);
            DrawText("Collect Hibiscus for a speed boost for 14 seconds!", 80, hibY + 22, 14, RAYWHITE);

            int cloudY = hibY + 60;
            if (cloudTex.IsLoaded()) {
                Rectangle src = {0.0f, 0.0f, static_cast<float>(cloudTex.Get().width), static_cast<float>(cloudTex.Get().height)};
                Rectangle dest = {40, static_cast<float>(cloudY), 28.0f, 28.0f};
                DrawTexturePro(cloudTex.Get(), src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
            } 
            DrawText("Cloud (Dangerous)", 80, cloudY, 18, RED);
            DrawText("Avoid Clouds! They reduce health by 1 point.", 80, cloudY + 22, 14, RAYWHITE);

            DrawRectangle(30, screenHeight - 70, screenWidth - 60, 45, RAYWHITE);
            DrawText("Press ENTER or SPACE to start playing.", 50, screenHeight - 58, 18, DARKBLUE);
        } else if (currentState == GameState::PLAYING) {
            player.Draw(wauTex.GetPtr());
            for (auto& entity : entities) {
                entity->Draw(
                    starTex.GetPtr(),
                    cloudTex.GetPtr(),
                    hibiscusTex.GetPtr()
                );
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
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
