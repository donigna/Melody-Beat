#include "header/note/note_factory.h"

Note *NoteFactory::CreateNote(NoteType type, Shipping shipping, Vector2 size, Color color)
{
    Note *newNote = nullptr;
    switch (type)
    {
    case TAP:
        newNote = new TapNote(shipping, size, color);
        break;
    case HOLD:
        newNote = new HoldNote(shipping, size, color);
        break;
    default:
        return nullptr; // Invalid note type
    }
    return newNote;
}