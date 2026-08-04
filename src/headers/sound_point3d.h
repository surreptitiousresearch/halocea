#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* sound_point3d — bare float triple used as a pun target for real_point3d/real_vector3d in
 * material_effect_new.c. No DB type of its own (orphan helper). */
typedef struct sound_point3d { float n[3]; } sound_point3d;
