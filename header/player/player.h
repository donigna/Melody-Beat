#pragma once
#include "raylib.h"
#include "header/note/note.h"
#include "header/ui/health_ui.h"
#include "header/assets.h"
#include <vector>
#include <algorithm>
class Note;

class Player : public IHealthSubject
{
private:
    Vector2 position;
    Vector2 size;
    Rectangle leftPlayerHitbox;
    Rectangle rightPlayerHitbox;
    Rectangle drawLeftHitbox;
    Rectangle drawRightHitbox;
    Rectangle playerRect;
    int maxHealth = 100;
    int health;
    bool playerAlive = true;

    float playerHeight = 120.0f;
    float playerWidth = 120.0f;
    Vector2 hitbox = {50, 50};

    // sprite frame setting
    Texture2D charTex;
    float charWidth;
    Rectangle frameRec;
    Rectangle drawPlayerRect;
    bool seeRight = true;

    bool lastHitLeft = false;
    bool lastHitRight = false;

    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;

    // Observer UI
    std::vector<IHealthObserver *> healthObservers;

public:
    // Local Methods
    void InitPlayer(float screenWidth);
    void RandomPlayerAnimation()
    {
        int rng = GetRandomValue(1, 5);

        switch (rng)
        {
        case 1:
            charTex = LoadTexture(Assets::GetInstance().hit_1);
            break;
        case 2:
            charTex = LoadTexture(Assets::GetInstance().hit_2);
            break;
        case 3:
            charTex = LoadTexture(Assets::GetInstance().hit_3);
            break;
        case 4:
            charTex = LoadTexture(Assets::GetInstance().hit_4);
            break;
        case 5:
            charTex = LoadTexture(Assets::GetInstance().hit_5);
            break;
        default:
            charTex = LoadTexture(Assets::GetInstance().idle);
            break;
        }
    }

    bool IsAlive() { return playerAlive; }
    void TakeDamage(float damage);
    void Heal(int amount);
    void DrawPlayer(bool hitLeft, bool hitRight);
    void PlayerGameOver();
    int GetCurrentHealth() { return health; };
    Rectangle GetHurtbox() const { return playerRect; }
    Rectangle GetHitboxLeft() const { return leftPlayerHitbox; }
    Rectangle GetHitboxRight() const { return rightPlayerHitbox; }

    // Interface Implementation
    void Attach(IHealthObserver *observer) override;
    void Detach(IHealthObserver *observer) override;
    void Notify() override;
};