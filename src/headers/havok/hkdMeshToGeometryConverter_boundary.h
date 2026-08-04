#pragma once
#include "hkVertexFormat.h"

/* Boundary types for hkdMeshToGeometryConverter (2-level descent — the private
   Impl worker and its methods are not reversed here). */

typedef struct hkMeshShape hkMeshShape;
typedef struct hkdGeometry hkdGeometry;

/* hkdMeshToGeometryConverter::Input (size 20, DB types_members) — conversion
   knobs. m_mergeCoplanarTriangles is a 1-byte hkEnum occupying its own slot. */
typedef struct hkdMeshToGeometryConverter_Input
{
    unsigned char m_mergeCoplanarTriangles; /* 0  hkEnum<MergeTriangles,unsigned char> */
    unsigned char _pad1[3];                 /* 1  */
    float m_coplanarPlaneEpsilon;           /* 4  */
    float m_linearEpsilon;                  /* 8  */
    int   m_linearUsage;                    /* 12 hkVertexFormat::DataUsage */
    int   m_linearSubUsage;                 /* 16 */
} hkdMeshToGeometryConverter_Input;

/* Local alias matching the DB's nested-qualified member spelling. */
typedef hkdMeshToGeometryConverter_Input Input;

/* hkdMeshToGeometryConverterImpl — the private conversion worker. Only its size
   (240 bytes, stack-local scratch) matters here; the layout is opaque at this
   boundary. */
typedef struct hkdMeshToGeometryConverterImpl
{
    unsigned char _opaque[240];
} hkdMeshToGeometryConverterImpl;

extern void hkdMeshToGeometryConverterImpl_construct(hkdMeshToGeometryConverterImpl *self, const Input *input);          /* hkdMeshToGeometryConverterImpl::hkdMeshToGeometryConverterImpl */
extern hkdGeometry *hkdMeshToGeometryConverterImpl_convertShapeToGeometry(hkdMeshToGeometryConverterImpl *self, const hkMeshShape *shape, const char *meshName); /* hkdMeshToGeometryConverterImpl::convertShapeToGeometry */
extern void hkdMeshToGeometryConverterImpl_destruct(hkdMeshToGeometryConverterImpl *self);                              /* hkdMeshToGeometryConverterImpl::~hkdMeshToGeometryConverterImpl */
