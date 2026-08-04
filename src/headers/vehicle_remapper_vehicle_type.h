#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* GUESS: enum recovered by reconciliation, no ground-truth source — rationale: the vehicle remapper's
 * 6 vehicle-type rows index the packed vehicle-set bitfield whose field names ARE ground truth
 * (headers_ref _BBD05D2D53689E3828EB6DA140E3AB26: warthogs bits 25-27, ghosts 22-24, scorpions 19-21,
 * rwarthogs 16-18, banshees 13-15, cg_turrets 10-12). vehicle_remapper_get_spawn_count maps row index
 * 0..5 to exactly those bit positions, fixing each row's vehicle unambiguously. Member names taken from
 * the reference bitfield's field names; enum name/values are the reconstruction. */

typedef enum vehicle_remapper_vehicle_type
{
    _vehicle_remapper_warthog   = 0,  /* bits 25-27 */
    _vehicle_remapper_ghost     = 1,  /* bits 22-24 */
    _vehicle_remapper_scorpion  = 2,  /* bits 19-21 */
    _vehicle_remapper_banshee   = 3,  /* bits 13-15 */
    _vehicle_remapper_cg_turret = 4,  /* bits 10-12 */
    _vehicle_remapper_rwarthog  = 5,  /* bits 16-18 */
    NUMBER_OF_VEHICLE_REMAPPER_VEHICLE_TYPES = 6
} vehicle_remapper_vehicle_type;
