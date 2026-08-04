#pragma once
/* ds_vector_generic — the common 16-byte header shared by every ws-engine dsVECTOR<T,8> instance
 * (apCL source cookie, data pointer, element count, capacity). Element access and mutation go through
 * the templated ds_vector ops declared extern by each caller; this header only models the fields read
 * directly (notably nElem). The element type is opaque here.
 * Type-erased view: the DB stores only concrete dsVECTOR<T,8> instantiations, never a generic
 * header type. Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */

typedef struct ds_vector_generic
{
    void *__cl;       /* apCL source-location cookie */
    void *pData;
    int   nElem;
    int   allocated;
} ds_vector_generic;
