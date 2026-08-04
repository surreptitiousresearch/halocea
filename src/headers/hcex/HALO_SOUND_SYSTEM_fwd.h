#pragma once

/* Opaque forward view of hcex HALO_SOUND_SYSTEM for C translation units.
   The full definition (headers/hcex/HALO_SOUND_SYSTEM.h) pulls the ws/snd
   C++ FMOD boundary chain and is not C-compilable; Blam-side code only ever
   holds the pointer. Per the hcex/ws boundary opaque-typedef convention. */
typedef struct HALO_SOUND_SYSTEM HALO_SOUND_SYSTEM;
