#include "Entity.hpp"

Entity::Entity(float x, float y, float speed, float speedMult, float sizeMult, EntityType type)
    : m_position({x, y}), m_basespeed(speed), m_speedMultiplier(speedMult), m_sizeMultiplier(sizeMult), m_type(type), m_active(true) {}

void Entity::Update(float deltaTime, int screenHeight) {
    m_position.y += (m_basespeed * m_speedMultiplier) * deltaTime;

    if (m_position.y > static_cast<float>(screenHeight) + 50.0f) {
        Deactivate();
    }
}

void Entity::Draw() const {
    if (!m_active) return;

    if (m_type == EntityType::STAR) {
        DrawCircleV(m_position, 12.0f, GOLD);
    } else if (m_type == EntityType::HIBISCUS) {
        DrawCircleV(m_position, 14.0f, PINK);
    } else {
        //Cloud grows bigger based on size multiplier
        float width = 35.0f * m_sizeMultiplier;
        float height = 35.0f * m_sizeMultiplier;
        DrawRectangleV(m_position, {width, height}, DARKGRAY);
    }
}

Rectangle Entity::GetBounds() const {
    if (m_type == EntityType::STAR) {
        return {m_position.x - 12.0f, m_position.y - 12.0f, 24.0f, 24.0f};
    } else if (m_type == EntityType::HIBISCUS) {
        return {m_position.x - 14.0f, m_position.y - 14.0f, 28.0f, 28.0f};
    } else {
        return {m_position.x, m_position.y, 35.0f * m_sizeMultiplier, 35.0f * m_sizeMultiplier};
    }
}