#pragma once

#include <string>
#include <iostream>
#include "raylib.h"

// Day 3 RAII Texture Wrapper
class TextureWrapper {
    private:
        Texture2D m_texture;
        bool m_loaded;

    public:
        TextureWrapper(const std::string& filePath) : m_loaded{false} {
            m_texture = LoadTexture(filePath.c_str());
            if (m_texture.id != 0) {
                m_loaded = true;
            } else {
                std::cerr << "[Warning!] Failed to load texture: " << filePath << std::endl;
            }
        }

        ~TextureWrapper() {
            if (m_loaded) {
                UnloadTexture(m_texture);
            }
        }

        //Disable copy semantics to prevent double free GPU memory errors
        TextureWrapper(const TextureWrapper&) = delete;
        TextureWrapper& operator=(const TextureWrapper&) = delete;

        Texture2D Get() const {
            return m_texture;
        }

        const Texture2D* GetPtr() const {
            return m_loaded ? &m_texture : nullptr;
        }

        bool IsLoaded() const {
            return m_loaded;
        }
};

// Day 3 RAII Sound Wrapper
class SoundWrapper {
    private:
        Sound m_sound;
        bool m_loaded;

    public:
        SoundWrapper(const std::string& filePath) : m_loaded{false} {
            m_sound = LoadSound(filePath.c_str());
            if (m_sound.stream.buffer != nullptr) {
                m_loaded = true;
            } else {
                std::cerr << "[Warning!] Failed to load sound: " << filePath << std::endl;
            }
        }

        ~SoundWrapper() {
            if (m_loaded) {
                UnloadSound(m_sound);
            }
        }

        //Disable copy semantics to prevent double free GPU memory errors
        SoundWrapper(const SoundWrapper&) = delete;
        SoundWrapper& operator=(const SoundWrapper&) = delete;

        void Play() const {
            if (m_loaded) {
                PlaySound(m_sound);
            }
        }
};