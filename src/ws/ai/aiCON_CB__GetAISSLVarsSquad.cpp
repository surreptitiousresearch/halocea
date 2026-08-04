#include "../../headers/ws/ai/aiCON_CB.h"

// aiCON_CB::GetAISSLVarsSquad @ 0x83181E08
// Squad SSL-var listing is not implemented; overwrite `list` with a fixed placeholder.
void aiCON_CB::GetAISSLVarsSquad(dsTSTRING<char> &list)
{
    list = dsTSTRING<char>("SQUAD list is not supported for now");
}
