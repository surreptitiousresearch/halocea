#pragma once

/* Forward-declared payload types (only used behind pointers in the vtable). */
typedef struct sound_permutation sound_permutation;
typedef struct sound_location sound_location;
typedef struct platform_sound_listener_properties platform_sound_listener_properties;
typedef struct platform_sound_channel_properties platform_sound_channel_properties;
typedef struct sound_preferences sound_preferences;

/* Platform sound backend dispatch table. */
typedef struct platform_sound_manager_definition
{
    short platform_code;                                                          // 0x00
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned char (*initialize)(sound_preferences *);                             // 0x04
    void (*dispose)(void);                                                        // 0x08
    void (*set_listener_properties)(const platform_sound_listener_properties *);  // 0x0C
    void (*begin_scene)(void);                                                    // 0x10
    void (*end_scene)(void);                                                      // 0x14
    void (*queue_sound_to_channel)(short, const sound_permutation *, int, unsigned char, short, unsigned char); // 0x18
    void (*channel_update)(short, unsigned char, short);                          // 0x1C
    void (*stop_channel)(short);                                                  // 0x20
    short (*get_channel_state)(short);                                            // 0x24
    void (*set_pause)(unsigned char);                                             // 0x28
    void (*flush)(void);                                                          // 0x2C
    void (*set_channel_location)(short, unsigned char, const sound_location *, float, float, unsigned char, short); // 0x30
    void (*set_channel_properties)(short, const platform_sound_channel_properties *, unsigned char, unsigned char, short); // 0x34
    void (*handle_audio_profile_changes)(unsigned char, unsigned char, int);      // 0x38
    unsigned char (*hardware_reverb_enabled)(void);                               // 0x3C
} platform_sound_manager_definition;
