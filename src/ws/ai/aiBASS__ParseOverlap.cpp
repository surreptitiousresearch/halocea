#include "../../headers/ws/ai/aiBASS.h"
#include <cstring>

// aiBASS::ParseOverlap @ 0x83240DD8
// ?ParseOverlap@aiBASS@@IAA?AW4exOVERLAP_RES@1@ABV?$dsTSTRING@D@@W421@Z
//
// Map an overlap-policy string ("SKIP"/"DELAY"/"REPLACE") to its enum. Unknown strings return the
// caller-supplied fallback. (The original open-codes three strcmp loops; reproduced with strcmp.)
aiBASS::exOVERLAP_RES aiBASS::ParseOverlap(const dsTSTRING<char> *text, exOVERLAP_RES fallback)
{
    const char *s = text->pBuffer->str;
    if (!strcmp(s, "SKIP"))    return exOVERLAP_SKIP;
    if (!strcmp(s, "DELAY"))   return exOVERLAP_DELAY;
    if (!strcmp(s, "REPLACE")) return exOVERLAP_REPLACE;
    return fallback;
}
