/* hcex_stat_vbuf_unlock @ 0x823EFB80
   Unlocks a static vertex buffer by dispatching through its vtable. The object
   is a ws-engine GPU-buffer class (boundary, not reversed); slot 3 (vtable byte
   offset 0xC) is its Unlock() method. Tail-called in the binary. */

extern "C" void hcex_stat_vbuf_unlock(void *static_vbuf)
{
    void (**vtable)(void *) = *(void (***)(void *))static_vbuf;
    vtable[3](static_vbuf);   /* vtable[3] == byte offset 0xC — Unlock() */
}
