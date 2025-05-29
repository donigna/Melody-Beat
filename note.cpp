#include "header/note/note.h"

bool Note::CheckCollision(Rectangle hitbox)
{
    return CheckCollisionRecs(hitboxRect, hitbox);
}

void Note::MarkMiss(Player &player)
{
    miss = true;
    ScoreManager::GetInstance().ResetCombo();
    player.TakeDamage(10);
}

void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint)
{
    if ((texture.id <= 0) || (scale <= 0.0f))
        return; // Wanna see a infinite loop?!...just delete this line!
    if ((source.width == 0) || (source.height == 0))
        return;

    int tileWidth = (int)(source.width * scale), tileHeight = (int)(source.height * scale);
    if ((dest.width < tileWidth) && (dest.height < tileHeight))
    {
        // Can fit only one tile
        DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, ((float)dest.height / tileHeight) * source.height},
                       (Rectangle){dest.x, dest.y, dest.width, dest.height}, origin, rotation, tint);
    }
    else if (dest.width <= tileWidth)
    {
        // Tiled vertically (one column)
        int dy = 0;
        for (; dy + tileHeight < dest.height; dy += tileHeight)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, source.height}, (Rectangle){dest.x, dest.y + dy, dest.width, (float)tileHeight}, origin, rotation, tint);
        }

        // Fit last tile
        if (dy < dest.height)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width / tileWidth) * source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                           (Rectangle){dest.x, dest.y + dy, dest.width, dest.height - dy}, origin, rotation, tint);
        }
    }
    else if (dest.height <= tileHeight)
    {
        // Tiled horizontally (one row)
        int dx = 0;
        for (; dx + tileWidth < dest.width; dx += tileWidth)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)dest.height / tileHeight) * source.height}, (Rectangle){dest.x + dx, dest.y, (float)tileWidth, dest.height}, origin, rotation, tint);
        }

        // Fit last tile
        if (dx < dest.width)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, ((float)dest.height / tileHeight) * source.height},
                           (Rectangle){dest.x + dx, dest.y, dest.width - dx, dest.height}, origin, rotation, tint);
        }
    }
    else
    {
        // Tiled both horizontally and vertically (rows and columns)
        int dx = 0;
        for (; dx + tileWidth < dest.width; dx += tileWidth)
        {
            int dy = 0;
            for (; dy + tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, source, (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, (float)tileHeight}, origin, rotation, tint);
            }

            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, dest.height - dy}, origin, rotation, tint);
            }
        }

        // Fit last column of tiles
        if (dx < dest.width)
        {
            int dy = 0;
            for (; dy + tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, (float)tileHeight}, origin, rotation, tint);
            }

            // Draw final tile in the bottom right corner
            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx) / tileWidth) * source.width, ((float)(dest.height - dy) / tileHeight) * source.height},
                               (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, dest.height - dy}, origin, rotation, tint);
            }
        }
    }
}

void DrawTextureTiledRight(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint)
{

    int tileWidth = (int)(source.width * scale);
    int tileHeight = (int)(source.height * scale);

    // Start from the right side of the destination
    int dx = 0;
    for (; dx + tileWidth < dest.width; dx += tileWidth)
    {
        int dy = 0;
        for (; dy + tileHeight < dest.height; dy += tileHeight)
        {
            Rectangle src = {source.x, source.y, -source.width, source.height}; // Flip horizontally
            Rectangle dst = {dest.x + dest.width - dx - tileWidth, dest.y + dy, (float)tileWidth, (float)tileHeight};
            DrawTexturePro(texture, src, dst, origin, rotation, tint);
            // DrawRectangleLinesEx(dst, 2, BLUE);
        }

        // Bottom strip
        if (dy < dest.height)
        {
            Rectangle src = {
                source.x, source.y,
                -source.width,
                ((float)(dest.height - dy) / tileHeight) * source.height};
            Rectangle dst = {
                dest.x, dest.y + dy,
                (float)tileWidth,
                dest.height - dy};
            DrawTexturePro(texture, src, dst, origin, rotation, tint);
        }
    }

    // Last column
    if (dx < dest.width)
    {
        int dy = 0;
        for (; dy + tileHeight < dest.height; dy += tileHeight)
        {
            Rectangle src = {
                source.x, source.y,
                -((float)(dest.width - dx) / tileWidth) * source.width,
                source.height};
            Rectangle dst = {
                dest.x + dest.width - dx - (dest.width - dx), dest.y + dy,
                dest.width - dx,
                (float)tileHeight};
            DrawTexturePro(texture, src, dst, origin, rotation, tint);
        }

        // Bottom-right tile
        if (dy < dest.height)
        {
            Rectangle src = {
                source.x, source.y,
                -((float)(dest.width - dx) / tileWidth) * source.width,
                ((float)(dest.height - dy) / tileHeight) * source.height};
            Rectangle dst = {
                dest.x + dest.width - dx - tileWidth, dest.y + dy,
                dest.width - dx,
                dest.height - dy};
            DrawTexturePro(texture, src, dst, origin, rotation, tint);
        }
    }
}

// Tap Note class implementation
void TapNote::DrawNote()
{
    if (!active)
        return;
    // Initializtion Target
    target = {GetScreenWidth() / 2.0f, target.y};

    // Rectangle noteRect = {
    //     position.x - size.x / 2,
    //     position.y - size.y / 2,
    //     size.x,
    //     size.y};

    // DrawRectangleRec(hitboxRect, color);
    DrawTexturePro(tex, noteFrame, hitboxRect, Vector2Zero(), 0, WHITE);

    // Debug
    // Pivot Debugginh
    // DrawCircle(position.x, position.y, 2, BLUE);
    // DrawText(hit ? "1" : "0", position.x, position.y + 10, 20, WHITE);

    // Hit Checker
}

void TapNote::UpdateNote(Player &player)
{
    if (!active)
        return;

    hitboxRect = {
        position.x - size.x / 2,
        position.y - (size.y / 2) - 10,
        size.x,
        size.y};

    // DrawRectangleLinesEx(hitboxRect, 4, BLUE);
    // Update logic for TapNote
    Vector2 direction = Vector2Subtract(target, position);
    if (Vector2Length(direction) > 0.1f && !miss && !hit)
    {
        // Note is tapped
        direction = Vector2Normalize(direction);
        position = Vector2Add(position, Vector2Scale(direction, speed));
    }
    else if (Vector2Length(direction) < 0.1f)
    {
        size = Vector2Add(size, {1.0f, 1.0f});
        if (color.a > 10)
            color.a -= 10; // Kurangi opacity
        else
        {
            color.a = 0;
            DestroyNote();
            MarkMiss(player);
        }
    }
    else if (hit)
    {
        size = Vector2Add(size, {1.0f, 1.0f});
        position = Vector2Add(position, {0, -5});
        if (color.a > 10)
            color.a -= 10; // Kurangi opacity
        else
        {
            color.a = 0;
            DestroyNote();
        }
    }
    else
    {
        DestroyNote();
        MarkMiss(player);
    }
}

void TapNote::NoteHit(Rectangle playerHitbox)
{
    MarkHit();
    ScoreManager::GetInstance().AddScore(10);
}

void TapNote::UnHitNote()
{
}

void TapNote::DestroyNote()
{
    active = false;
}

// Hold Note class implementation
void HoldNote::DrawNote()
{
    if (!active)
        return;

    target = {GetScreenWidth() / 2.0f, target.y};

    noteFrame = {0, 10, (float)tex.width, (float)tex.height - 30};
    if (shipping == Shipping::RIGHT)
    {
        DrawTextureTiledRight(tex, noteFrame, hitboxRect, Vector2Zero(), 0, 2, WHITE);
        DrawRectangleRec({hitboxRect.x, hitboxRect.y + 10, hitboxRect.width, hitboxRect.height - 10}, {255, 106, 227, 100});
    }
    else
    {
        DrawTextureTiled(tex, noteFrame, hitboxRect, Vector2Zero(), 0, 2, WHITE);
        DrawRectangleRec({hitboxRect.x, hitboxRect.y + 10, hitboxRect.width, hitboxRect.height - 10}, {113, 63, 234, 100});
    }

    // hitbox debug
    // DrawRectangleLinesEx(hitboxRect, 2, BLUE);

    // DrawCircle(position.x, position.y, 8, BLUE);
}

void HoldNote::UpdateNote(Player &player)
{
    // Update logic for HoldNote
    if (!active)
        return;

    hitboxRect =
        {position.x + (shipping == Shipping::RIGHT ? 0 : -currentHoldLength),
         position.y - (size.y / 2.0f) - 10,
         currentHoldLength,
         size.y};

    if (!beingHeld)
    {
        position.y = target.y;
        Vector2 direction = Vector2Subtract(target, position);
        if (Vector2Length(direction) > 0.1f)
        {
            // Note is tapped
            direction = Vector2Normalize(direction);
            position = Vector2Add(position, Vector2Scale(direction, speed));
        }
        else
        {
            DecreaseLength();
            takeDamageTimer += GetFrameTime();

            if (takeDamageTimer >= addPointInterval)
            {
                player.TakeDamage(2);
                takeDamageTimer = 0;
            }
        }
    }
}

void HoldNote::NoteHit(Rectangle playerHitbox)
{
    holdTime += GetFrameTime();
    addPointTimer += GetFrameTime();

    Vector2 hitboxPosition = {
        playerHitbox.x + (shipping == Shipping::RIGHT ? 0 : playerHitbox.width),
        playerHitbox.y + (playerHitbox.height / 2) + GetRandomValue(-10, 10),
    };

    SoundManager::GetInstance().PlayHoldSFX();

    float distance = Vector2Length(Vector2Subtract(hitboxPosition, position));

    if (distance > 1.0f && !beingHeld)
    {
        currentHoldLength -= distance;
    }
    position = hitboxPosition;

    if (addPointTimer >= addPointInterval)
    {
        ScoreManager::GetInstance().AddScore(10);
    }
    beingHeld = true;

    DecreaseLength();
}

void HoldNote::UnHitNote()
{
    beingHeld = false;
}

void HoldNote::DestroyNote()
{
    active = false;
    MarkHit();
}

void HoldNote::DecreaseLength()
{
    float shrinkSpeed = 60.0f * speed; // pixel per detik
    currentHoldLength -= shrinkSpeed * GetFrameTime();

    if (currentHoldLength < 0)
    {
        currentHoldLength = 0;
        DestroyNote();
    }
}