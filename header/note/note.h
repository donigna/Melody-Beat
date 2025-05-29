#pragma once
#include "raylib.h"
#include "raymath.h"
#include "header/player/player.h"
#include "header/score_manager.h"
#include "header/assets.h"
#include "header/sound_manager.h"

enum NoteType
{
    TAP,
    HOLD,
};

enum class Shipping
{
    LEFT,
    RIGHT
};

class Player;

class Note
{
protected:
    Vector2 position = {0, 380};
    Shipping shipping;
    Vector2 size;
    Color color;
    float speed = 5.0f; // Speed of the note
    Vector2 target = {GetScreenWidth() / 2.0f, 380};
    Rectangle hitboxRect;
    bool active = true;
    bool hit = false;
    bool miss = false;

    // Note Tex
    Texture2D tex;
    Rectangle noteFrame;

public:
    // Hitbox

    Note(Shipping shipping, Vector2 size, Color color) : shipping(shipping), size(size), color(color)
    {
        if (shipping == Shipping::LEFT)
        {
            position = {-50.0f, 380};
            target = {target.x + size.x / 2, target.y};
            tex = LoadTexture(Assets::GetInstance().tap_note_left);
        }
        else
        {
            position = {GetScreenWidth() + 50.0f, 380};
            target = {target.x - size.x / 2, target.y};
            tex = LoadTexture(Assets::GetInstance().tap_note_right);
        }

        hitboxRect = {
            position.x - size.x / 2,
            position.y - size.y / 2,
            size.x,
            size.y};

        noteFrame = {10, 10, (float)tex.width - 30, (float)tex.height - 30};
    };

    bool CheckCollision(Rectangle hitbox);
    bool IsHit() const { return hit; }
    bool IsMiss() const { return miss; }
    void MarkHit() { hit = true; }

    virtual void MarkMiss(Player &player);
    virtual void DrawNote() = 0;
    virtual void UpdateNote(Player &player) = 0;
    virtual void NoteHit(Rectangle playerHitbox) = 0;
    virtual void UnHitNote() = 0;
    virtual void DestroyNote() = 0;
    virtual ~Note() {};
};

// ------------------------------------------------------
// Concreate class for a specific type of note
class TapNote : public Note
{
private:
public:
    TapNote(Shipping shipping, Vector2 size, Color color) : Note(shipping, size, color) {};
    void DrawNote() override;
    void UpdateNote(Player &player) override;
    void NoteHit(Rectangle playerHitbox) override;
    void UnHitNote() override;
    void DestroyNote() override;
};

class HoldNote : public Note
{

public:
    float holdTime = 0.0f;
    float currentHoldLength;
    float addPointTimer = 0.0f;
    float addPointInterval = .2f;
    float takeDamageTimer = 0.0f;
    bool beingHeld = false;

    HoldNote(Shipping shipping, Vector2 size, Color color) : Note(shipping, size, color)
    {
        currentHoldLength = size.x;

        if (shipping == Shipping::RIGHT)
        {
            hitboxRect = {
                position.x + currentHoldLength,
                position.y,
                currentHoldLength,
                size.y,
            };
            tex = LoadTexture(Assets::GetInstance().hold_note_right);
        }
        else
        {
            hitboxRect =
                {position.x - currentHoldLength,
                 position.y - size.y / 2.0f,
                 currentHoldLength,
                 size.y};
            tex = LoadTexture(Assets::GetInstance().hold_note_left);
        }
        SetTextureWrap(tex, TEXTURE_WRAP_REPEAT);
        noteFrame = {10, 10, (float)tex.width - 30, (float)tex.height - 30};
    };
    void DrawNote() override;
    void UpdateNote(Player &player) override;
    void NoteHit(Rectangle playerHitbox) override;
    void UnHitNote() override;
    void DestroyNote() override;

    // local methods
    void DecreaseLength();
};

// -----------------------------------------------------
