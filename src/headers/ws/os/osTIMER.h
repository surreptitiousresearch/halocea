#pragma once
// ws-engine os: the low-resolution platform timer snapshot (the sibling of osTIMER2, which tracks
// high-resolution elapsed time). DB-verified layout (types_members osTIMER): start@0, time@4 —
// size 8 (types osTIMER). Methods are boundaries: the bodies belong to the os subsystem and are
// not reversed here.
//
// Sole definition. It previously existed twice — src/headers/ws/snd/fmod_error_boundary.h and
// src/headers/ws/vid/vidDIP.h each carried a file-scope copy — which is what made the header-layout
// probe emit `redefinition of 'osTIMER'`. Both now include this file.

typedef struct osTIMER {
    int start; // 0x00
    int time;  // 0x04

#ifdef __cplusplus
    // ??0osTIMER@@QAA@_N@Z -- the parameter is a BOOL, not the initial tick count: at
    // WaitSoundBank+0x18 (0x836B5820) the caller sets `li r4, 1` for the `isStarted` slot before
    // `bl osTIMER::osTIMER(bool)`. fmod_error_boundary.h had it typed `int start`, which named the
    // wrong thing as well as the wrong type. boundary -- body owned by the os subsystem.
    osTIMER(bool isStarted);
#endif
} osTIMER;
