#include "Player.hpp"

Player::Player(float startX, float startY)
    : m_position{startX, startY}, m_basespeed{350.0f}, m_currentSpeed{350.0f}, m_speedBoostTimer{0.0f}, m_score{0}, m_health{3} {}

void Player::ApplySpeedBoost(float duration) {
    m_speedBoostTimer = duration; // Reset timer to 14 seconds
}

void Player::Reset(float startX, float startY) {
    m_position = {startX, startY};
    m_score = 0;
    m_health = 3;
    m_speedBoostTimer = 0.0f;
    m_currentSpeed = m_basespeed; // Reset speed to base speed
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
}

void Player::Draw() const {
    // Speed Boost Visual Indicator
    if (m_speedBoostTimer > 0.0f) {
        DrawCircleLines(m_position.x + 20.0f, m_position.y + 20.0f, 30.0f, ORANGE);
        DrawCircleLines(m_position.x + 20.0f, m_position.y + 20.0f, 32.0f, GOLD);
    }

    // Wau Bulan Placeholder
    Color playerColor = (m_speedBoostTimer > 0.0f) ? YELLOW : RED; // Change color when speed boost is active
    DrawTriangle({m_position.x + 20.0f, m_position.y}, 
                 {m_position.x, m_position.y + 40.0f}, 
                 {m_position.x + 40.0f, m_position.y + 40.0f}, 
                 playerColor);
}
/*
void Player::AddScore(int amount) {
    m_score += amount;
}

void Player::TakeDamage(int amount) {
    m_health -= amount;
    if (m_health < 0) m_health = 0;
}*/

Rectangle Player::GetBounds() const {
    return {m_position.x, m_position.y, 40.0f, 40.0f};
}