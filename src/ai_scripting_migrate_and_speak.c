/* ai_scripting_migrate_and_speak @0x83770AD8 — migrate the source encounter's actors into the target and
 * have them speak an advance/retreat line (generate_stimulus = 1, advancing = the speech type is
 * "advance"). */

#include <stdint.h>

extern int stricmp(const char *a, const char *b);
extern void ai_scripting_migrate_internal(int source_index, int target_index, uint8_t generate_stimulus, uint8_t advancing_stimulus);

void ai_scripting_migrate_and_speak(int source_index, int target_index, const char *speech_type)
{
    unsigned __int8 advancing;
    if ( stricmp(speech_type, "advance") )
    {
        stricmp(speech_type, "retreat");   /* result unused — retreat is the default */
        advancing = 0;
    }
    else
    {
        advancing = 1;
    }
    ai_scripting_migrate_internal(source_index, target_index, 1u, advancing);
}
