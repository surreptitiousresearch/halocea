#pragma once
/* sound_compression — Blam sound sample compression format (sound_definition.compression).
 * DB anonymous enum _4E4B6056A804F36A23212C0C36F4B4CC (verbatim). */
#ifndef SOUND_COMPRESSION_H
#define SOUND_COMPRESSION_H

enum sound_compression
{
    _sound_compression_none = 0,
    _sound_compression_xbox_adpcm = 1,
    _sound_compression_ima_adpcm = 2,
    _sound_compression_ogg = 3,
    NUMBER_OF_SOUND_COMPRESSION_TYPES = 4
};

#endif /* SOUND_COMPRESSION_H */
