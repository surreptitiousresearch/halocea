#include "../headers/havok/hkxNode.h"
#include "../headers/havok/hkClass.h"

/* Depth-first search of a scene-graph subtree for the node whose variant holds
   exactly (object, type). Returns the node itself if it matches, otherwise the
   first matching descendant, otherwise null. */
hkxNode *hkdAssetProcessingUtil_findNode(hkxNode *node, const hkClass *type, void *object)
{
    int i;

    if (node->m_object.m_class == type && node->m_object.m_object == object)
        return node;

    for (i = 0; i < node->m_numChildren; ++i)
    {
        hkxNode *found = hkdAssetProcessingUtil_findNode(node->m_children[i], type, object);
        if (found)
            return found;
    }
    return 0;
}
