#include "header/player/player.h"
#include "header/note/note.h"

void Player::InitPlayer(float screenWidth)
{
    // First Load Texture
    charTex = LoadTexture("resources/assets/character/Idle_Right.png");

    frameRec = {0.0f, 0.0f, (float)charTex.width / 4, (float)charTex.height};
    charWidth = frameRec.width;
    position = (Vector2){screenWidth / 2, 380};
    size = (Vector2){playerHeight, playerWidth};

    health = maxHealth;
    playerAlive = true;
    Notify();

    leftPlayerHitbox = {
        (position.x + -10 - hitbox.x) - hitbox.x,
        position.y - hitbox.x / 2,
        hitbox.x,
        hitbox.y};

    // drawLeftHitbox = {leftPlayerHitbox.x - leftPlayerHitbox.width, leftPlayerHitbox.y - leftPlayerHitbox.height / 2, leftPlayerHitbox.width, leftPlayerHitbox.height};

    rightPlayerHitbox = {
        position.x + 10 + hitbox.x,
        position.y - hitbox.x / 2,
        hitbox.x,
        hitbox.y};

    // drawRightHitbox = {rightPlayerHitbox.x, rightPlayerHitbox.y - rightPlayerHitbox.height / 2, rightPlayerHitbox.width, rightPlayerHitbox.height};

    playerRect = {
        position.x - size.x / 2,
        position.y - size.y / 2,
        size.x,
        size.y};

    drawPlayerRect = {
        playerRect.x,
        playerRect.y,
        size.x,
        size.y};
}

void Player::TakeDamage(float damage)
{
    health -= damage;
    if (health < 0)
    {
        health = 0;
        PlayerGameOver();
    }

    // Notify Observers
    Notify();
}

void Player::PlayerGameOver()
{
    playerAlive = false;
}

void Player::Heal(int amount)
{
    if (health < maxHealth)
    {
        health += amount;
        if (health > maxHealth)
            health = maxHealth;

        // Notify Observers
        Notify();
    }
}

void Player::DrawPlayer(bool hitLeft, bool hitRight)
{
    framesCounter++;
    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > 3)
            currentFrame = 0;

        frameRec.x = (float)currentFrame * frameRec.width;
    }

    // Deteksi perubahan input (new attack)
    bool newAttack = (hitLeft || hitRight) && !(lastHitLeft || lastHitRight);

    if (hitLeft && hitRight)
    {
        charTex = LoadTexture(Assets::GetInstance().hit_loong);
    }
    if (newAttack)
    {
        RandomPlayerAnimation();
    }
    else if (!hitLeft && !hitRight)
    {
        charTex = LoadTexture(Assets::GetInstance().idle);
    }

    // Update arah lihat
    if (hitLeft)
    {
        seeRight = false;
        DrawCircle(leftPlayerHitbox.x + (hitbox.x / 2), leftPlayerHitbox.y + (hitbox.y / 2), 25, WHITE);
    }
    if (hitRight)
    {
        seeRight = true;
        DrawCircle(rightPlayerHitbox.x + (hitbox.x / 2), rightPlayerHitbox.y + (hitbox.y / 2), 25, WHITE);
    }

    frameRec.width = seeRight ? charWidth : -charWidth;

    // Draw hitbox
    // DrawRectangleLinesEx(leftPlayerHitbox, 3, RED);
    DrawRing({leftPlayerHitbox.x + (hitbox.x / 2), leftPlayerHitbox.y + (hitbox.y / 2)}, 25, 30, 0, 360, 64, WHITE);

    // DrawRectangleLinesEx(rightPlayerHitbox, 3, RED);
    DrawRing({rightPlayerHitbox.x + (hitbox.x / 2), rightPlayerHitbox.y + (hitbox.y / 2)}, 25, 30, 0, 360, 64, WHITE);

    DrawTexturePro(charTex, frameRec, drawPlayerRect, Vector2Zero(), 0, WHITE);
    // DrawRectangleLinesEx(playerRect, 1, BLUE);

    // Simpan state input sebelumnya
    lastHitLeft = hitLeft;
    lastHitRight = hitRight;
}

/// Interface Health Observer Implementation
void Player::Attach(IHealthObserver *observer)
{
    healthObservers.push_back(observer);
}

void Player::Detach(IHealthObserver *observer)
{
    healthObservers.erase(std::remove(healthObservers.begin(), healthObservers.end(), observer), healthObservers.end());
}

void Player::Notify()
{
    for (IHealthObserver *observer : healthObservers)
    {
        if (observer)
        {
            observer->OnHealthChanged(health, maxHealth);
        }
    }
}