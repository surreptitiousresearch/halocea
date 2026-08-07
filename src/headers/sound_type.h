#pragma once
/* sound_type — sound_datum ->type (the playback role of a playing sound: a one-shot
 * impulse or one of the looping-track lifecycle states).
 * DB-verified via types_enum_values $A8E1C366B75ABE5D28E7BB479D0F1B51 (verbatim). */
#ifndef SOUND_TYPE_H
#define SOUND_TYPE_H

enum sound_type
{
    _sound_impulse = 0,
    _sound_start_track = 1,
    _sound_loop_track = 2,
    _sound_stopping_track = 3,
    _sound_stop_track = 4,
    NUMBER_OF_SOUND_TYPES = 5
};

#endif /* SOUND_TYPE_H */
