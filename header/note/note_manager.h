#pragma once
#include "note.h"
#include "raylib.h"
#include <vector>

#define MAX_NOTES 500

// Factory class for creating notes
class NoteManager
{
private:
    std::vector<Note *> notes;

public:
    void AddNote(Note *note);
    void UpdateAllNotes(Player &player);
    void DrawAllNotes();
    void CleanUpNotes();
    void ClearNotes();
    bool CheckHits(Player &player, bool hitLeft, bool hitRight);
    ~NoteManager() {};
};