#pragma once
#include "hkxAttributeHolder.h"
#include "hkVariant.h"
#include "hkBool.h"

typedef struct hkMatrix4 hkMatrix4;

/* hkxNode::AnnotationData, size 8 */
typedef struct hkxNode_AnnotationData
{
    float m_time;
    char *m_description;
} hkxNode_AnnotationData;

/* The DB spells this nested type hkxNode::AnnotationData; member declarations
   reference the unqualified nested name. */
typedef hkxNode_AnnotationData AnnotationData;

/* scene-graph node, size 52 */
typedef struct hkxNode
{
    hkxAttributeHolder base;          /* offset 0 */
    char *m_name;                     /* offset 8 */
    hkVariant m_object;               /* offset 12 */
    hkMatrix4 *m_keyFrames;           /* offset 20 */
    int m_numKeyFrames;               /* offset 24 */
    struct hkxNode **m_children;      /* offset 28 */
    int m_numChildren;                /* offset 32 */
    AnnotationData *m_annotations;    /* offset 36 — DB hkxNode::AnnotationData * */
    int m_numAnnotations;             /* offset 40 */
    char *m_userProperties;           /* offset 44 */
    hkBool m_selected;                /* offset 48 */
} hkxNode;
