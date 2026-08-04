#pragma once

#include "platform_sound_manager_definition.h"
#include "sound_listener.h"

typedef struct sound_manager_globals_t
{
    unsigned char initialized;                                  /* 0x00 */
    unsigned char active;                                       /* 0x01 */
    unsigned char paused;                                       /* 0x02 */
    unsigned char idling;                                       /* 0x03 */
    int game_time_when_no_scripted_dialog_will_be_playing;      /* 0x04 */
    platform_sound_manager_definition *platform;                /* 0x08 */
    int render_time;                                            /* 0x0C */
    float ticks_elapsed;                                        /* 0x10 */
    unsigned char flip_flop;                                    /* 0x14 */
    unsigned char _pad0[3]; /* db-verified padding */
    sound_listener listeners[2];                                /* 0x18 */
    int sound_environment_tag;                                  /* 0xA0 */
    float nondialog_gain;                                       /* 0xA4 */
    float music_gain;                                           /* 0xA8 */
    float master_gain;                                          /* 0xAC */
    float effects_gain;                                         /* 0xB0 */
    short channel_count;                                        /* 0xB4 */
    unsigned char disable_from_commandline;                     /* 0xB6 */
    unsigned char _pad1[1]; /* db-verified padding */
    short sound_variety;                                        /* 0xB8 */
} sound_manager_globals_t;                                      /* 188 bytes */
