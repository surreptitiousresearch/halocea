// ds::LIST<msgADDR *>::PushBack(msgADDR *const &) — @0x82AC5CB0
// Deviation: the compiler outlined the DATA allocate+construct sequence separately in each arm of
// the head/tail branch (four separate scratch locals in the raw decompile); collapsed here to a
// single `node` local per arm since both allocation sites are identical `operator new(sizeof(DATA),
// __cl.file, __cl.line)` + DATA field init. The null-allocation fallthrough (node == nullptr on
// OOM) is preserved faithfully from the decompile, including the unchecked `tail->next->prev`
// dereference on the append path (matches the disassembly, not a reconstruction error).
#include "LIST.h"
#include "../msg/msgADDR.h"
void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug-tagged new (Saber)

namespace ds {

template<>
void LIST<msgADDR *>::PushBack(msgADDR *const &val) {
    ++length;
    if (head) {
        DATA *node = (DATA *)operator new(sizeof(DATA), __cl.file, __cl.line);
        if (node) {
            node->val  = val;
            node->next = nullptr;
            node->prev = nullptr;
        } else {
            node = nullptr;
        }
        tail->next = node;
        tail->next->prev = tail;
        tail = tail->next;
    } else {
        DATA *node = (DATA *)operator new(sizeof(DATA), __cl.file, __cl.line);
        if (node) {
            node->val  = val;
            node->next = nullptr;
            node->prev = nullptr;
        } else {
            node = nullptr;
        }
        tail = node;
        head = node;
    }
}

} // namespace ds
