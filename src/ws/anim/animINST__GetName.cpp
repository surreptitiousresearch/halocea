#include "animINST.h"

// Fallback returned when the instance carries no name: a global byte at 0x8200155A
// (IDA empty_string). The byte's content was not resolvable through the query surface; it
// is the shared placeholder name string the engine hands back for unnamed instances.
extern char anim_kUnnamedName[]; // = &(global byte @0x8200155A)  boundary

// animINST::GetName @ 0x823CD8E0
char *animINST::GetName()
{
    char *name = this->name;
    if (!name)
        return anim_kUnnamedName;
    return name;
}
