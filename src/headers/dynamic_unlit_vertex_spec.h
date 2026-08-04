#pragma once

#include "real_point3d.h"
#include "real_point2d.h"

/* 28-byte unlit dynamic-screen vertex (position + packed colors + one texcoord). */
typedef struct dynamic_unlit_vertex_spec
{
    real_point3d position;   // 0x00
    unsigned int color;      // 0x0C  packed ARGB
    unsigned int specular;   // 0x10  packed ARGB
    real_point2d texcoord;   // 0x14
} dynamic_unlit_vertex_spec;
