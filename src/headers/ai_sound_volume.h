#ifndef AI_SOUND_VOLUME_H
#define AI_SOUND_VOLUME_H

/* AI sound volume enum — headers_ref _ED961A502760ED4B264CF1BDDA17763B.
 * Used for prop_datum.audibility and actor_audibility_at_point's volume classification. */
enum ai_sound_volume
{
    _ai_sound_volume_silent    = 0,
    _ai_sound_volume_medium    = 1,
    _ai_sound_volume_loud      = 2,
    _ai_sound_volume_shout     = 3,
    _ai_sound_volume_quiet     = 4,
    NUMBER_OF_AI_SOUND_VOLUMES = 5,
};

#endif /* AI_SOUND_VOLUME_H */

// ai_sound_volume: DB-verified via types_enum_values _ED961A502760ED4B264CF1BDDA17763B (anonymous compiled enum, names sans leading _)
