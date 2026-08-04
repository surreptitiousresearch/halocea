#ifndef WS_OBJ_OBJ_DATA_H
#define WS_OBJ_OBJ_DATA_H

// OBJ_DATA — per-object data-block classes managed by objOBJ Alloc/FreeObjData
// (vertex streams, skinning, tangents, colors, texcoord sets, instancing, interleaved, faces).
enum OBJ_DATA
{
    OBJ_DATA_VERT = 0,
    OBJ_DATA_SKIN_WEIGHTS = 1,
    OBJ_DATA_SKIN_INDICES = 2,
    OBJ_DATA_NORM = 3,
    OBJ_DATA_TANGENT_1 = 4,
    OBJ_DATA_TANGENT_2 = 5,
    OBJ_DATA_TANGENT_3 = 6,
    OBJ_DATA_TANGENT_4 = 7,
    OBJ_DATA_TANGENT_5 = 8,
    OBJ_DATA_VERT_COLOR_1 = 9,
    OBJ_DATA_VERT_COLOR_2 = 10,
    OBJ_DATA_VERT_COLOR_3 = 11,
    OBJ_DATA_VERT_COLOR_4 = 12,
    OBJ_DATA_VERT_COLOR_5 = 13,
    OBJ_DATA_VERT_COLOR_6 = 14,
    OBJ_DATA_TEX_1 = 15,
    OBJ_DATA_TEX_2 = 16,
    OBJ_DATA_TEX_3 = 17,
    OBJ_DATA_TEX_4 = 18,
    OBJ_DATA_TEX_5 = 19,
    OBJ_DATA_TEX_6 = 20,
    OBJ_DATA_TEX_7 = 21,
    OBJ_DATA_TEX_8 = 22,
    OBJ_DATA_TEX_9 = 23,
    OBJ_DATA_TEX_10 = 24,
    OBJ_DATA_TEX_11 = 25,
    OBJ_DATA_TEX_12 = 26,
    OBJ_DATA_TEX_13 = 27,
    OBJ_DATA_TEX_14 = 28,
    OBJ_DATA_TEX_15 = 29,
    OBJ_DATA_TEX_16 = 30,
    OBJ_DATA_TEX_17 = 31,
    OBJ_DATA_TEX_18 = 32,
    OBJ_DATA_TEX_19 = 33,
    OBJ_DATA_TEX_20 = 34,
    OBJ_DATA_INST = 35,
    OBJ_DATA_INTERLEAVED = 36,
    OBJ_DATA_FACE_INDICES = 37,
    OBJ_DATA_MAX = 38,
};

#endif // WS_OBJ_OBJ_DATA_H

// OBJ_DATA: DB-verified via types_enum_values OBJ_DATA (named PDB enum)
