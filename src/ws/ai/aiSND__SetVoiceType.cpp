#include "../../headers/ws/ai/aiSND.h"
#include <cstring>

// aiSND::SetVoiceType @ 0x83299310
// ?SetVoiceType@aiSND@@IAAXABV?$dsTSTRING@D@@@Z
//
// Adopt `name` as this module's voice-type id. An empty name — or the sentinel "mute" — instead
// disables exclamations (clearing the ENABLE_EXCLAMATIONS permission) and leaves voiceType untouched.
void aiSND::SetVoiceType(const dsTSTRING<char> &name)
{
    if (name.pBuffer->strLen && strcmp(name.pBuffer->str, "mute") != 0)
    {
        dsSTRID id(name, false);
        voiceType.id = id.id;
    }
    else
    {
        permissions.val &= ~AI_SNDP_ENABLE_EXCLAMATIONS;
    }
}
