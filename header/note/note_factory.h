#pragma once
#include "note.h"
#include "raylib.h"
#include <vector>

// Factory class for creating notes
class NoteFactory
{
public:
    Note *CreateNote(NoteType type, Shipping shipping, Vector2 size, Color color);
};
