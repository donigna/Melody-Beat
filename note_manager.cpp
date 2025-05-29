#include "header/note/note_manager.h"
#include <algorithm>

void NoteManager::AddNote(Note *note)
{
    notes.push_back(note);
}

void NoteManager::UpdateAllNotes(Player &player)
{
    if (notes.size() > 0)
    {
        for (auto note : notes)
        {
            note->UpdateNote(player);
        }
    }
}

void NoteManager::DrawAllNotes()
{
    for (auto note : notes)
    {
        note->DrawNote();
    }
}

void NoteManager::CleanUpNotes()
{
    notes.erase(
        std::remove_if(
            notes.begin(),
            notes.end(),
            [](Note *&note)
            {
                bool remove = note->IsHit() || note->IsMiss();

                if (remove)
                {
                    delete note;
                    note = nullptr;
                }

                return remove;
            }),
        notes.end());
}

void NoteManager::ClearNotes()
{
    notes.clear();
}

bool NoteManager::CheckHits(Player &player, bool hitLeft, bool hitRight)
{
    bool isCollided = false;
    for (auto note : notes)
    {
        if (note->IsHit() || note->IsMiss())
            continue;

        bool collidedPlayer = note->CheckCollision(player.GetHurtbox());
        bool collidedLeft = note->CheckCollision(player.GetHitboxLeft());
        bool collidedRight = note->CheckCollision(player.GetHitboxRight());

        if (collidedPlayer && !collidedLeft && !collidedRight)
        {
            note->MarkMiss(player);
            continue;
        }

        if ((hitLeft && collidedLeft) || (hitRight && collidedRight))
        {
            Rectangle hitboxRect = collidedLeft ? player.GetHitboxLeft() : player.GetHitboxRight();
            note->NoteHit(hitboxRect);
            isCollided = true;
            player.Heal(1);
        }
        else
        {
            note->UnHitNote();
        }
    }

    return isCollided;
}
