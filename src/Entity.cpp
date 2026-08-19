#include "Entity.hpp"
#include <cmath>

Entity::Entity(float x, float y, float speed, float speedMult, float sizeMult, EntityType type)
    : m_position({x, y}), m_basespeed(speed), m_speedMultiplier(speedMult), m_sizeMultiplier(sizeMult), m_type(type), m_active(true) {}

void Entity::Update(float deltaTime, int screenHeight) {
    m_position.y += (m_basespeed * m_speedMultiplier) * deltaTime;

    if (m_position.y > static_cast<float>(screenHeight) + 50.0f) {
        Deactivate();
    }
}

void Entity::Draw(const Texture2D* starTex, const Texture2D* cloudTex, const Texture2D* hibiscusTex) const {
    if (!m_active) return;

    if (m_type == EntityType::STAR) {
        // Star glow effect
        DrawCircleGradient(m_position, 22.0f, Fade(GOLD, 0.4f), Fade(GOLD, 0.0f));

        if (starTex && starTex->id != 0) {
            Rectangle src = {0.0f, 0.0f, static_cast<float>(starTex->width), static_cast<float>(starTex->height)};
            Rectangle dest = {m_position.x, m_position.y, 28.0f, 28.0f};
            Vector2 origin = {14.0f, 14.0f}; // Center of the texture
            DrawTexturePro(*starTex, src, dest, origin, m_rotation, WHITE);
        } else {
            DrawCircleV(m_position, 12.0f, GOLD);
        }
       
    } else if (m_type == EntityType::HIBISCUS) {
        // Hibiscus Aura glow effect
        DrawCircleGradient(m_position, 25.0f, Fade(PINK, 0.5f), Fade(PINK, 0.0f));

        if (hibiscusTex && hibiscusTex->id != 0) {
            Rectangle src = {0.0f, 0.0f, static_cast<float>(hibiscusTex->width), static_cast<float>(hibiscusTex->height)};
            Rectangle dest = {m_position.x, m_position.y, 32.0f, 32.0f};
            Vector2 origin = {16.0f, 16.0f}; // Center of the texture
            DrawTexturePro(*hibiscusTex, src, dest, origin, m_rotation, WHITE);
        } else {
            DrawCircleV(m_position, 14.0f, PINK);
        }
        
    } else {
        //Cloud grows bigger based on size multiplier
        float radius = 18.75f * m_sizeMultiplier;
        float diameter = radius * 2.0f;

        // Dark stormy aura under cloud
        DrawCircleGradient(
            m_position, radius + 4.0f,
            Fade(DARKGRAY, 0.4f), Fade(BLACK, 0.1f)
        );

        if (cloudTex && cloudTex->id != 0) {
            Rectangle src = {0.0f, 0.0f, static_cast<float>(cloudTex->width), static_cast<float>(cloudTex->height)};
            Rectangle dest = {m_position.x - radius, m_position.y - radius, diameter, diameter};
            DrawTexturePro(*cloudTex, src, dest, {0.0f, 0.0f}, 0.0f, WHITE);
        } else {
            DrawCircleV(m_position, radius, DARKGRAY);
        }
    
    }
}

Rectangle Entity::GetBounds() const {
    if (m_type == EntityType::STAR) {
        return {m_position.x - 14.0f, m_position.y - 14.0f, 28.0f, 28.0f};
    } else if (m_type == EntityType::HIBISCUS) {
        return {m_position.x - 16.0f, m_position.y - 16.0f, 32.0f, 32.0f};
    } else {
        float radius = 18.75f * m_sizeMultiplier;
        return {m_position.x - radius, m_position.y - radius, radius * 2.0f, radius * 2.0f};
    }
}