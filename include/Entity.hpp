#pragma once

#include "raylib.h"

enum class EntityType {STAR, OBSTACLE, HIBISCUS};

class Entity {
    private:
        EntityType m_type;
        Vector2 m_position;
        float m_basespeed;
        float m_speedMultiplier;
        float m_sizeMultiplier;
        bool m_active;

    public:
        Entity(float x, float y, float speed, float speedMult, float sizeMult, EntityType type);

        void Update(float deltaTime, int screenHeight);
        void Draw() const;

        bool IsActive() const {
            return m_active;
        }

        void Deactivate() {
            m_active = false;
        }

        Rectangle GetBounds() const;

        EntityType GetType() const {
            return m_type;
        }
};
