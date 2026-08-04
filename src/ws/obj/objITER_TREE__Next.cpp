#include "objITER_TREE.h"
#include "objOBJ.h"
// 0x823CD820  ?Next@objITER_TREE@@QAAXXZ
// Pre-order advance: descend into the current node's first child unless that child was
// explicitly skipped (isSkipped) or absent, otherwise walk to the next sibling, climbing
// back up through parents until one has an unvisited next sibling. pCur is cleared once the
// traversal returns to the subtree root.

void objITER_TREE::Next()
{
    objOBJ *cur = pCur;
    if (!cur)
        return;

    objOBJ *next;
    if (isSkipped || (next = cur->child) == nullptr) {
        isSkipped = 0;
        if (cur == pRoot) {
            pCur = nullptr;
            return;
        }
        while (true) {
            objOBJ *node = pCur;
            objOBJ *parent = node->parent;
            next = node->next;
            if (parent->child != next)
                break; // `next` is a real sibling of `node`; visit it
            // `node` was the parent's last child — ascend and retry from the parent.
            pCur = parent;
            if (parent == pRoot) {
                pCur = nullptr;
                return;
            }
        }
    }
    pCur = next;
}
