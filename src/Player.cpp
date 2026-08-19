#include "Player.hpp"

Player::Player(float startX, float startY)
    : m_position{startX, startY}, m_basespeed{350.0f}, m_currentSpeed{350.0f}, m_speedBoostTimer{0.0f}, m_score{0}, m_health{3}, m_currentFrame{0}, m_frameTimer{0.0f}, m_frameSpeed{0.1f}, totalFrames{3} {}

void Player::Reset(float startX, float startY) {
    m_position = {startX, startY};
    m_score = 0;
    m_health = 3;
    m_speedBoostTimer = 0.0f;
    m_currentSpeed = m_basespeed; // Reset speed to base speed
    m_currentFrame = 0;
    m_frameTimer = 0.0f;
}

void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // Speed Boost Logic
    if (m_speedBoostTimer > 0.0f) {
        m_speedBoostTimer -= deltaTime;
        m_currentSpeed = m_basespeed * 2.2f; // Increase speed by 120%
    } else {
        m_speedBoostTimer = 0.0f;
        m_currentSpeed = m_basespeed; // Reset to base speed
    }
    // 2D 4-way Movement Controls
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        m_position.y -= m_basespeed * deltaTime;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        m_position.y += m_basespeed * deltaTime;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        m_position.x -= m_basespeed * deltaTime;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        m_position.x += m_basespeed * deltaTime;
    }

    //Enforce Class In variant: Keep the player within the screen bounds
    if (m_position.x < 0.0f) m_position.x = 0.0f;
    if (m_position.x > screenWidth - 40.0f) m_position.x = screenWidth - 40.0f;
    if (m_position.y < 0.0f) m_position.y = 0.0f;
    if (m_position.y > screenHeight - 40.0f) m_position.y = screenHeight - 40.0f;

    // Animation Logic
    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frameSpeed) {
        m_frameTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % totalFrames;
    }
}

void Player::Draw(const Texture2D* wauSpriteSheet) const {
    // Speed Boost Visual Indicator
    if (HasSpeedBoost()) {
        Vector2 center = {m_position.x + 22.5f, m_position.y + 22.5f};
        DrawCircleGradient(center, 26.0f, Fade(ORANGE, 0.5f), Fade(GOLD, 0.0f));
        DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), 24.0f, ORANGE); 
    }

    // Wau Bulan animated sprite drawing
    if (wauSpriteSheet && wauSpriteSheet->id != 0) {

        float frameWidth = static_cast<float>(wauSpriteSheet->width) / static_cast<float>(totalFrames);
        float frameHeight = static_cast<float>(wauSpriteSheet->height);

        Rectangle src = {static_cast<float>(m_currentFrame * frameWidth), 0.0f, frameWidth, frameHeight};
        Rectangle dest = {m_position.x, m_position.y, 45.0f, 45.0f}; // Adjusted size for better visibility
        DrawTexturePro(*wauSpriteSheet, src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        
    Color playerColor = (m_speedBoostTimer > 0.0f) ? ORANGE : RED; // Change color when speed boost is active (2nd scenario)
    DrawTriangle({m_position.x + 22.5f, m_position.y}, 
                 {m_position.x, m_position.y + 45.0f}, 
                 {m_position.x + 45.0f, m_position.y + 45.0f}, 
                 playerColor);
    }
}

Rectangle Player::GetBounds() const {
    return {m_position.x, m_position.y, 45.0f, 45.0f};
}