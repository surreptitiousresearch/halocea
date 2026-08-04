#include "objOBJ_ITER_SAFE.h"
#include "objOBJ.h"

// objOBJ_ITER_SAFE::objOBJ_ITER_SAFE(objOBJ *root) @ 0x82BD4C38
// Snapshot `root`'s direct children into childList (a circular singly-linked list via
// objOBJ::next), so the caller can safely mutate the tree while iterating a fixed set.
// CAVEAT: dsVECTOR's allocation call-site cookie (__cl) is set from a local {file, line}
// initializer built at obj_iter.cpp:18 — reproduced as childList's __cl fields directly rather
// than via the local apCL temporary the decompiler shows.
objOBJ_ITER_SAFE::objOBJ_ITER_SAFE(objOBJ *root)
{
    this->childList.pData = nullptr;
    this->childList.allocated = 0;
    this->childList.nElem = 0;
    this->cur = 0;
    this->childList.__cl.file = "D:\\Projects\\code\\common\\src.sys\\objects\\obj_iter.cpp";
    this->childList.__cl.line = 18;

    if (root && root->child) {
        objOBJ *firstChild = root->child;
        objOBJ *child = firstChild;
        do {
            this->childList.PushBack(child); // PushBack takes const objOBJ*&, not objOBJ**
            child = child->next;
        } while (child != firstChild);
    }
}
