#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"

// aiDEBUG::MarkOnBhvEnd @ 0x83195A80  (?MarkOnBhvEnd@aiDEBUG@@QAAXPBD@Z)
//
// Record (or clear) the "OnBhvEnd(<name>)" debug line shown for this brain.
void aiDEBUG::MarkOnBhvEnd(const char *name)
{
    if (name)
    {
        dsTSTRING<char> line;
        dsSPrintf(&line, " OnBhvEnd(%s)", name);
        this->lineOnBhvEnd = line;
    }
    else
    {
        this->lineOnBhvEnd.Clear();
    }
}
