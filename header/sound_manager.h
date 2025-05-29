#pragma once
#include "raylib.h"
#include "assets.h"

class SoundManager
{
private:
    SoundManager() {};
    Sound hitSFXs[4];
    Sound holdSFX;
    float volume = 0.3f; // Default volume

public:
    static SoundManager &GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

    void LoadSounds()
    {
        for (int i = 0; i < 4; ++i)
        {
            hitSFXs[i] = LoadSound(Assets::GetInstance().hit_sfxs[i]);
            SetSoundVolume(hitSFXs[i], volume);
        }
        holdSFX = LoadSound(Assets::GetInstance().hold_sfx);
        SetSoundVolume(holdSFX, volume);
    };
    void PlayHitSFX(int index)
    {
        if (index < 0 || index >= 4)
        {
            TraceLog(LOG_WARNING, "Invalid hit SFX index: %d", index);
            return;
        }
        PlaySound(hitSFXs[index]);
    };
    void PlayHoldSFX()
    {
        PlaySound(holdSFX);
    };

    SoundManager(SoundManager const &) = delete;
    void operator=(SoundManager const &) = delete;
};