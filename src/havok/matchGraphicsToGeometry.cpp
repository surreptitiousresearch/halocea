/* matchGraphicsToGeometry @0x8383FF50 */
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkClass.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkMeshShape.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/hkGeometryMatchingUtils_TriangleMap.h"
#include "../headers/havok/hkdAssetProcessingUtil_BaseObjectSource.h"
#include "../headers/havok/hkdAssetProcessingUtil_SplitPlaneSources.h"

/* Memory-class tag: DB exposes the enumerator name at the call sites but not its
   numeric value, so it is externed rather than invented (cf. hkThreadMemory.h). */
extern const int HK_MEMORY_CLASS_UTILITIES;

/* Runtime class descriptor of hkdMeshGraphicsShape (Havok reflection global). */
extern const hkClass hkdMeshGraphicsShapeClass;

/* --- Havok SDK boundaries (mangled hk* / hkd* siblings — not reversed) --------- */

/* getClassType(): virtual, returns the object's runtime class; takes the shared
   hkReferencedObject base per the real DEF. */
extern const hkClass *hkReferencedObject_getClassType(hkReferencedObject *self);

/* hkClass::isSuperClass(other): returns whether `self` is a base of `other`.
   The ABI returns an hkBool through a hidden sret buffer (arg0); synced to that
   real 3-arg form. */
extern hkBool *hkClass_isSuperClass(hkBool *out, const hkClass *self, const hkClass *other);

/* meshShape virtual: number of mesh sections. */
extern int hkMeshShape_getNumSections(const hkMeshShape *meshShape);

/* Extract this mesh section's vertices/indices into the two output arrays
   (appends; the arrays are pre-reserved by the caller). */
extern void hkMeshTohkGeometryConverter_getVerticesIndicesFromMeshSection(
    const hkMeshShape *meshShape,
    int sectionIndex,
    hkArray<hkVector4> *verticesOut,
    hkArray<int> *indicesOut);

/* Grow an hkArray by at least `numElements` more elements (element size implied
   by the instantiation; here the section-count arrays are int, size 4). */
extern void hkArrayUtil__reserveMore(hkArray<int> *array, int numElements);

/* hkGeometryMatchingUtils::TriangleMap::~TriangleMap */
extern void hkGeometryMatchingUtils_TriangleMap_destruct(
    hkGeometryMatchingUtils_TriangleMap *triangleMap);

/* hkdAssetProcessingUtil::BaseObjectSource::~BaseObjectSource */
extern void hkdAssetProcessingUtil_BaseObjectSource_destruct(
    hkdAssetProcessingUtil_BaseObjectSource *baseObjectSource);

/* Sibling (src/havok/rematchGraphics.c): builds the triangle map for the base
   and each split-plane geometry, then re-links every child face identifier. */
extern void rematchGraphics(
    hkdAssetProcessingUtil_BaseObjectSource *baseObjectSource,
    hkdAssetProcessingUtil_SplitPlaneSources *splitPlaneSources);

/* Pre-reserved scratch sizes for the reference vertex/index buffers (bump-
   allocated from the thread stack; capacity is kept in each hkArray's
   m_capacityAndFlags with the DONT_DEALLOCATE bit 0x80000000 set). */
#define REFERENCE_VERTICES_SCRATCH_BYTES 65552 /* 4096 hkVector4 + guard */
#define REFERENCE_INDICES_SCRATCH_BYTES  32784 /* 8192 int + guard */

/* Match a breakable shape's mesh graphics shape against its physics geometry:
   pull the reference vertices/indices from every mesh section, record the
   cumulative per-section vertex and triangle counts, then delegate the actual
   geometry<->reference correspondence to rematchGraphics. Pairs with
   rematchGraphics (0x8383FCA8). */
void matchGraphicsToGeometry(
    hkdBreakableShape *breakableShape,
    hkdAssetProcessingUtil_SplitPlaneSources *splitPlaneSrc)
{
    hkThreadMemory *threadMemory = hkThreadMemory_getCurrent();

    hkdAssetProcessingUtil_BaseObjectSource baseObjectSource;
    baseObjectSource.m_numVerticesPerSection.m_data = 0;
    baseObjectSource.m_numVerticesPerSection.m_size = 0;
    baseObjectSource.m_numVerticesPerSection.m_capacityAndFlags = 0x80000000;
    baseObjectSource.m_numTrianglesPerSection.m_data = 0;
    baseObjectSource.m_numTrianglesPerSection.m_size = 0;
    baseObjectSource.m_numTrianglesPerSection.m_capacityAndFlags = 0x80000000;

    /* Two scratch buffers grabbed LIFO off the thread stack, presented as empty
       hkArrays with DONT_DEALLOCATE capacity so the converter appends into them
       without heap churn. */
    hkArray<hkVector4> referenceVertices;
    hkArray<int> referenceTriangleIndices;

    referenceVertices.m_data = 0;
    referenceVertices.m_size = 0;
    referenceVertices.m_capacityAndFlags = 0x80000000;

    void *referenceVerticesStackBase =
        hkThreadMemory_allocateStack(threadMemory, REFERENCE_VERTICES_SCRATCH_BYTES);
    referenceVertices.m_data = (hkVector4 *)referenceVerticesStackBase; /* void*->typed for hkArray<hkVector4> */
    referenceVertices.m_capacityAndFlags = 0x80001000; /* 4096 | DONT_DEALLOCATE */

    void *referenceTriangleIndicesStackBase =
        hkThreadMemory_allocateStack(threadMemory, REFERENCE_INDICES_SCRATCH_BYTES);
    referenceTriangleIndices.m_data = (int *)referenceTriangleIndicesStackBase; /* void*->typed for hkArray<int> */
    referenceTriangleIndices.m_size = 0;
    referenceTriangleIndices.m_capacityAndFlags = 0x80002000; /* 8192 | DONT_DEALLOCATE */

    const hkdGraphicsShape *graphicsShape = breakableShape->m_graphicsShape.m_pntr;
    const hkClass *actualClass = hkReferencedObject_getClassType((hkReferencedObject *)graphicsShape);

    hkBool superClassResult;
    if (hkClass_isSuperClass(&superClassResult, &hkdMeshGraphicsShapeClass, actualClass)->m_bool)
    {
        /* The mesh interface is the second base sub-object of the graphics shape
           (multiple inheritance): one hkdGraphicsShape (16 bytes) further on. */
        const hkMeshShape *meshShape = (const hkMeshShape *)&graphicsShape[1];
        int numSections = hkMeshShape_getNumSections(meshShape);

        for (int section = 0; section < numSections; ++section)
        {
            hkMeshTohkGeometryConverter_getVerticesIndicesFromMeshSection(
                meshShape, section, &referenceVertices, &referenceTriangleIndices);

            /* cumulative vertex count so far -> numVerticesPerSection[section] */
            int cumulativeVertexCount = referenceVertices.m_size;
            if (baseObjectSource.m_numVerticesPerSection.m_size ==
                (baseObjectSource.m_numVerticesPerSection.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&baseObjectSource.m_numVerticesPerSection, 4);
            ((int *)baseObjectSource.m_numVerticesPerSection.m_data)
                [baseObjectSource.m_numVerticesPerSection.m_size++] = cumulativeVertexCount;

            /* cumulative triangle count so far -> numTrianglesPerSection[section] */
            int cumulativeTriangleCount = referenceTriangleIndices.m_size / 3;
            if (baseObjectSource.m_numTrianglesPerSection.m_size ==
                (baseObjectSource.m_numTrianglesPerSection.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&baseObjectSource.m_numTrianglesPerSection, 4);
            ((int *)baseObjectSource.m_numTrianglesPerSection.m_data)
                [baseObjectSource.m_numTrianglesPerSection.m_size++] = cumulativeTriangleCount;
        }

        baseObjectSource.m_breakableShape = breakableShape;
        baseObjectSource.m_triangleMap = 0;
        baseObjectSource.m_referenceVertices = (const hkVector4 *)referenceVertices.m_data;
        baseObjectSource.m_referenceTriangleIndices = (const int *)referenceTriangleIndices.m_data;

        rematchGraphics(&baseObjectSource, splitPlaneSrc);

        /* Release the lazily built triangle map (mirrors newTriangleMap in the
           sibling: free the fixed-size chunk back to its thread-memory bucket). */
        hkGeometryMatchingUtils_TriangleMap *triangleMap = baseObjectSource.m_triangleMap;
        if (triangleMap)
        {
            hkGeometryMatchingUtils_TriangleMap_destruct(triangleMap);
            hkThreadMemory_deallocateChunk(threadMemory, triangleMap,
                sizeof(hkGeometryMatchingUtils_TriangleMap), HK_MEMORY_CLASS_UTILITIES);
        }

        /* Tear down the two scratch reference arrays: pop the stack chunk, then
           free any heap the array may have grown into (skipped while
           DONT_DEALLOCATE is set). */
        hkThreadMemory_deallocateStack(threadMemory, referenceTriangleIndicesStackBase);
        if (referenceTriangleIndices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory, referenceTriangleIndices.m_data,
                4 * referenceTriangleIndices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

        hkThreadMemory_deallocateStack(threadMemory, referenceVerticesStackBase);
        if (referenceVertices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory, referenceVertices.m_data,
                16 * referenceVertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

        /* Free the two per-section count arrays. */
        if (baseObjectSource.m_numTrianglesPerSection.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory,
                baseObjectSource.m_numTrianglesPerSection.m_data,
                4 * baseObjectSource.m_numTrianglesPerSection.m_capacityAndFlags,
                HK_MEMORY_CLASS_ARRAY);
        if (baseObjectSource.m_numVerticesPerSection.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory,
                baseObjectSource.m_numVerticesPerSection.m_data,
                4 * baseObjectSource.m_numVerticesPerSection.m_capacityAndFlags,
                HK_MEMORY_CLASS_ARRAY);
    }
    else
    {
        hkErrStream errorStream;
        char errorBuffer[608];
        hkErrStream_construct(&errorStream, errorBuffer, 512);
        hkOstream_insertString(&errorStream.base,
            "Your supplied graphics shape is not of type 'hkdMeshGraphicsShape', "
            "cannot call matchGraphicsToGeometry on this object");
        hkError_message(hkError_s_instance, MESSAGE_WARNING, -1413860125, errorBuffer,
            "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp",
            2561);
        hkOstream_destruct(&errorStream.base);

        /* Same scratch teardown as the success path (arrays were never filled). */
        hkThreadMemory_deallocateStack(threadMemory, referenceTriangleIndicesStackBase);
        if (referenceTriangleIndices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory, referenceTriangleIndices.m_data,
                4 * referenceTriangleIndices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

        hkThreadMemory_deallocateStack(threadMemory, referenceVerticesStackBase);
        if (referenceVertices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(threadMemory, referenceVertices.m_data,
                16 * referenceVertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

        hkdAssetProcessingUtil_BaseObjectSource_destruct(&baseObjectSource);
    }
}
