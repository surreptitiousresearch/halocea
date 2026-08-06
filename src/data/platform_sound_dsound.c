/* platform_sound_dsound @ 0x84175EE8 (.data, 64 bytes)
 * DB applied_types: platform_sound_manager_definition platform_sound_dsound;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 platform_code              = 0x0000
 *   +0x04 initialize                 = 0x836B5B30 -> dsound_initialize
 *   +0x08 dispose                    = 0x836B5B38 -> dsound_dispose
 *   +0x0C set_listener_properties    = 0x836B9B68 -> set_listener_properties_dsound
 *   +0x10 begin_scene                = 0x836B5B40 -> begin_scene_dsound
 *   +0x14 end_scene                  = 0x836B5B48 -> end_scene_dsound
 *   +0x18 queue_sound_to_channel     = 0x836C4F10 -> dsound_virtual_queue
 *   +0x1C channel_update             = 0x836B5B50 -> dsound_virtual_update
 *   +0x20 stop_channel               = 0x836C3810 -> dsound_virtual_stop
 *   +0x24 get_channel_state          = 0x836C4680 -> dsound_virtual_get_state
 *   +0x28 set_pause                  = 0x836BD958 -> pause_dsound
 *   +0x2C flush                      = 0x836B5B58 -> flush_dsound
 *   +0x30 set_channel_location       = 0x836BF0A0 -> dsound_virtual_set_location
 *   +0x34 set_channel_properties     = 0x836BF118 -> dsound_virtual_set_properties
 *   +0x38 handle_audio_profile_changes = 0x836B5B60 -> dsound_handle_audio_profile_changes_fmod
 *   +0x3C hardware_reverb_enabled    = 0x836B5B68 -> dsound_hardware_reverb_enabled
 * second-order .data; platform_sound_manager_definition reconstructed from binary bytes.
 */
/* DEVIATION: task-designated header hcex/HALO_SOUND_SYSTEM_globals.h only forward-declares
 * platform_sound_manager_definition (opaque there); instantiating requires the full layout, so we
 * include the dedicated full-layout header src/headers/platform_sound_manager_definition.h instead. */
#include <stdint.h>
#include "../headers/platform_sound_manager_definition.h"

#include "headers/sound_permutation.h"
#include "headers/sound_location.h"
#include "headers/sound_preferences.h"
extern uint8_t dsound_initialize(sound_preferences *preferences);
extern void          dsound_dispose(void);
extern void          set_listener_properties_dsound(const platform_sound_listener_properties *properties);
extern void begin_scene_dsound(void);
extern void end_scene_dsound(void);
extern void dsound_virtual_queue(int16_t virtual_channel_index, const sound_permutation *sound, int identifier, uint8_t is_local_player, int16_t class_index, uint8_t lapping);
extern void          dsound_virtual_update(short, unsigned char, short);
extern void          dsound_virtual_stop(short);
extern int16_t dsound_virtual_get_state(int16_t virtual_channel_index);
extern void pause_dsound(uint8_t paused);
extern void flush_dsound(void);
extern void dsound_virtual_set_location(int16_t virtual_channel_index, uint8_t spatialize, const sound_location *location, float occlusion, float obstruction, uint8_t underwater, int16_t class_index);
extern void          dsound_virtual_set_properties(short, const platform_sound_channel_properties *, unsigned char, unsigned char, short);
extern void          dsound_handle_audio_profile_changes_fmod(unsigned char, unsigned char, int);
extern uint8_t dsound_hardware_reverb_enabled(void);

platform_sound_manager_definition platform_sound_dsound = {
    .platform_code                = 0,
    .initialize                   = dsound_initialize,
    .dispose                      = dsound_dispose,
    .set_listener_properties      = set_listener_properties_dsound,
    .begin_scene                  = begin_scene_dsound,
    .end_scene                    = end_scene_dsound,
    .queue_sound_to_channel       = dsound_virtual_queue,
    .channel_update               = dsound_virtual_update,
    .stop_channel                 = dsound_virtual_stop,
    .get_channel_state            = dsound_virtual_get_state,
    .set_pause                    = pause_dsound,
    .flush                        = flush_dsound,
    .set_channel_location         = dsound_virtual_set_location,
    .set_channel_properties       = dsound_virtual_set_properties,
    .handle_audio_profile_changes = dsound_handle_audio_profile_changes_fmod,
    .hardware_reverb_enabled      = dsound_hardware_reverb_enabled,
};
