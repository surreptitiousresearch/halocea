#include "../headers/ws/ENGINE_CONTROL.h"

// ENGINE_CONTROL::IsSplitScreen @ 0x82768550 -- getter for the split-screen flag (byte @ 0x29).
bool ENGINE_CONTROL::IsSplitScreen()
{
    return isSplitScreen;
}
