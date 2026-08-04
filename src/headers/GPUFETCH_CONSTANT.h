#pragma once

/* GPUFETCH_CONSTANT - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPUFETCH_CONSTANT). */
#include "GPUTEXTURE_FETCH_CONSTANT.h"
#include "GPUVERTEX_FETCH_CONSTANT.h"

typedef union GPUFETCH_CONSTANT { // DB types_members GPUFETCH_CONSTANT
    GPUTEXTURE_FETCH_CONSTANT Texture; /* 0x00 */
    GPUVERTEX_FETCH_CONSTANT Vertex[3]; /* 0x00 */
} GPUFETCH_CONSTANT;
