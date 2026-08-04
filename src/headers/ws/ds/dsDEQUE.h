#pragma once
#include "dsVECTOR.h"
// ws-engine ds: double-ended queue over a dsVECTOR-backed ring buffer.
// DB-verified layout (types_members dsDEQUE<jbmJOB*>):
//   data@0 (dsVECTOR<T,8>, 20), head@20, tail@24, length@28 — size 32.

template<class T>
struct dsDEQUE {
    dsVECTOR<T, 8> data;   // 0x00 backing storage
    int            head;   // 0x14 ring start index
    int            tail;   // 0x18 ring end index
    int            length; // 0x1C live element count

    // Construct an empty deque: default-construct the backing vector (adopting the allocation
    // call-site cookie `cl`, passed by value per the mangle) and zero the ring indices/length.
    // Reversed per-instantiation.
    dsDEQUE(apCL cl);
    // Default ctor: lets a deque be a default-constructible member (dsERASE_MANAGER's ctor
    // default-constructs eraseQueue, then pokes its fields directly). boundary (2026-07-31)
    dsDEQUE();
    // Destroy the deque: destroys the backing vector (implicit member teardown).
    ~dsDEQUE();

    // Grow (or shrink) the queue so it holds newLength elements, extending at the back.
    void ResizeBack(int newLength);        // reversed per-instantiation
    // Pointer to the last live element slot.
    T *Back();                      // boundary
    // Pointer to the first live element slot (ring head).
    T *Front();                     // boundary
    // Shrink (or grow) the queue to newLength elements by trimming from the front (head).
    void ResizeFront(int newLength); // boundary

    // 0x826EC6D8 (dsDEQUE<jbmJOB*>) — append a copy of val at the back, return the new slot.
    T &PushBack(const T &val);

    // Remove the front element (shrink to length-1 from the head). Reversed per-instantiation.
    void PopFront();

    // Ensure the backing store holds at least minSize slots, growing it to 3*minSize/2 (then
    // Reconstruct-ing the ring) when it must. Reversed per-instantiation.
    void GrowData(int minSize);

    // dsERASE_MANAGER drives its eraseQueue's Reconstruct directly (as the original source does);
    // grant it access rather than widening Reconstruct's visibility.
    template<class ERASER> friend struct dsERASE_MANAGER;

protected:
    // 0x826EC0E0 (dsDEQUE<jbmJOB*>) — after the backing store grew from `oldLength` slots, relocate
    // whichever wrapped ring run is smaller into the freshly added tail space so the queue is
    // contiguous-ordered again. Called from ResizeBack. Reversed per-instantiation.
    void Reconstruct(int oldLength);
};
