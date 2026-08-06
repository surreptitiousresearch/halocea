/* playback_codec @ 0x84177D24 (.data, 16 bytes)
 * DB applied_types: animation_playback *playback_codec[4];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x84177D1C -> v1_playback
 *   +0x0004 = 0x84177D1C -> v1_playback
 *   +0x0008 = 0x84177D1C -> v1_playback
 *   +0x000C = 0x84177D14 -> current_playback
 */
#include "../headers/animation_playback.h"
/* DEVIATION: the entries were authored in the reverse order (current x3 then v1); the image
 * words above are v1_playback x3 then current_playback. Consumers index this table as
 * playback_codec[recording->version - 1] (recorded_animation_verify.c:27,
 * recorded_animation_play_internal.c:69), so recording versions 1-3 decode through the v1 codec
 * and version 4 through the current one — the inverted table sent every legacy recording to
 * recorded_animation_apply_event_stream and every current recording to the _v1 entry point. */
extern animation_playback current_playback;
extern animation_playback v1_playback;
animation_playback *playback_codec[4] = {
    &v1_playback,       /* version 1 */
    &v1_playback,       /* version 2 */
    &v1_playback,       /* version 3 */
    &current_playback,  /* version 4 */
};
