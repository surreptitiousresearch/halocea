#pragma once
// ws-engine obj: pre-order iterator over an objOBJ scene tree. DB-verified layout
// (types_members objITER_TREE) — size 12.

struct objOBJ; // ../obj/objOBJ.h — pointer only

typedef struct objITER_TREE {
    objOBJ *pRoot;     // 0x00 subtree root the traversal is confined to
    objOBJ *pCur;      // 0x04 current node (nullptr once traversal completes)
    int     isSkipped; // 0x08 when set, skip the current node's children on the next advance

    // 0x823CD820 — advance to the next node in pre-order (children first, then siblings,
    // walking back up to a parent's next sibling); clears pCur at the end of the subtree.
    void Next();

    // Construction / reset over a subtree root — bodies external to this batch.
    objITER_TREE(objOBJ *root); // boundary

    // No-arg form: several HCEX call sites (e.g. HCEX_ANIM_MNG_FP::ApplyAnim) inline the
    // trivial 3-field init (isSkipped=0; pCur=pRoot=root) directly at the call site rather than
    // calling a real ctor; this overload lets that pattern construct the object, to be
    // immediately field-initialized by the caller. Trivial — no boundary body needed.
    objITER_TREE() {}
} objITER_TREE;
