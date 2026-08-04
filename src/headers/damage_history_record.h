#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* damage_history_record — one entry of an object's recent-damage history (object+1072, four 16-byte
 * records), tracking who recently damaged the object for kill/assist attribution. Layout cross-referenced
 * from game_statistics_record_kill (time@0 compared against game_time windows; damage@4 read as float) and
 * actor_perception_find_killer_prop_index (damager_object_index@8 fed to ai_get_responsible_unit). */

typedef struct damage_history_record
{
    int   time;                   /* 0x00 — game time at which the damage was dealt */
    float damage;                 /* 0x04 — accumulated damage from this damager */
    int   damager_object_index;   /* 0x08 */
    int   damager_player_index;   /* 0x0C */
} damage_history_record;          /* 16 bytes */
