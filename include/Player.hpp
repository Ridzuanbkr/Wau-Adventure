#pragma once

#include "raylib.h"

class Player {
    private:
        Vector2 m_position;
        float m_basespeed;
        float m_currentSpeed;
        float m_speedBoostTimer;
        int m_score;
        int m_health;

        //Animation state variables
        int m_currentFrame;
        float m_frameTimer;
        float m_frameSpeed; //Delay between frames in seconds
        int totalFrames; //Total number of frames in the animation

    public:
        Player(float startX, float startY);

        void Update(float deltaTime, int screenWidth, int screenHeight);
        void Draw(const Texture2D* wauSpriteSheet) const;

        void AddScore(int amount) {
            m_score += amount;
        }
        void TakeDamage(int amount) {
            m_health -= amount;
        }
        void ApplySpeedBoost(float duration) {
            m_speedBoostTimer = duration;
        }

        int GetScore() const {
            return m_score;
        }
        int GetHealth() const {
            return m_health;
        }
        bool HasSpeedBoost() const {
            return m_speedBoostTimer > 0.0f;
        }
        float GetBoostTimeRemaining() const {
            return m_speedBoostTimer;
        }
        Rectangle GetBounds() const;

        void Reset(float startX, float startY);
};