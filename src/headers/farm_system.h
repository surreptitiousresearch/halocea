#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* farm_system — ws farm entity manager; only the isSuspendAdd flag (@0x0) is touched by callers. */

typedef struct farm_system
{
    char isSuspendAdd; /* 0x0 (opaque tail) */
} farm_system;
