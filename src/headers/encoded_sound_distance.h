#pragma once
/* Encoded BSP sound-path distance between two structure clusters
 * (structure_bsp_get_cluster_encoded_sound_distance): one byte where bit 7 marks "no sound path"
 * (the clusters are not acoustically connected) and the low 7 bits are the path length in steps.
 * World units = steps * 256/127. Basis: actor_audibility_at_point / actors_handle_unit_effect
 * treat bit 7 as inaudible, then scale (encoded & 0x7F) by 2.015748. */

#define ENCODED_SOUND_DISTANCE_NO_PATH_FLAG          0x80
#define ENCODED_SOUND_DISTANCE_HAS_NO_PATH(e)        (((e) & ENCODED_SOUND_DISTANCE_NO_PATH_FLAG) != 0)
#define ENCODED_SOUND_DISTANCE_STEPS(e)              ((e) & 0x7F)
#define ENCODED_SOUND_DISTANCE_WORLD_UNITS_PER_STEP  2.015748f   /* 256.0f / 127.0f */
