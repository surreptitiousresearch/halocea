#include "MSG_PROTOCOL.h"

extern "C" void dlFree(void *ptr); // boundary — debug heap allocator

namespace con {

// Not a separate DB symbol -- the decompiler inlines this identical length-fixup sequence at
// all three Read() call sites in Update(). Bounds the string's reported length to the first
// NUL byte found in the locked buffer, or to the buffer's capacity if none was found.
// Factored out here purely for readability.
static void FixupReadLength(dsTSTRING<char> &str)
{
    dsTSTRING_BUF_HEADER<char> *buf = str.pBuffer;
    int capacity = buf->maxStrLen;
    if (capacity < 0)
    {
        buf->strLen = capacity;
        buf->str[buf->strLen] = 0;
        return;
    }
    int i = 0;
    while (buf->str[i])
    {
        if (++i > capacity)
        {
            buf->strLen = capacity;
            buf->str[buf->strLen] = 0;
            return;
        }
    }
    buf->strLen = i;
}

// dsTSTRING<char> has no modeled destructor here (it is ref-counted; the decompiler shows
// the teardown inline as an explicit refcount-decrement-and-free rather than via a dtor
// call) -- replicate that manual release-on-zero-refcount teardown.
static void ReleaseTString(dsTSTRING<char> &str)
{
    dsTSTRING_BUF_HEADER<char> *buf = str.pBuffer;
    if (--buf->refCount == 0)
        dlFree(buf);
}

// con::MSG_PROTOCOL::Update() @ 0x82AC78D8
// Per-frame pump: drives the OnConnected/OnDisconnected transition hooks off
// pComm->isConnected, then drains every queued inbound message -- decoding its
// {page,ctrl,msg} header text from its memory-backed request stream and dispatching to
// OnMsg, replying via pComm->SendMsg for id==2 (response-expected) requests.
void MSG_PROTOCOL::Update()
{
    if (!pComm)
        return;

    if (isConnected)
    {
        if (!pComm->isConnected)
        {
            isConnected = 0;
            OnDisconnected();
            return;
        }
    }
    else
    {
        if (!pComm->isConnected)
            return;
        isConnected = 1;
        OnConnected();
    }

    if (!pComm->HaveMsg())
        return;

    // Guarded, lazily-constructed function-local static in the decompile (guard byte
    // `_S2_74`); an ordinary C++ function-local static lowers to the identical pattern.
    static fioFILE_MEM resp;

    do
    {
        // CAVEAT: the decompile zero-inits CON_MSG::data's 12 payload bytes and separately
        // stashes an apCL debug cookie ({conShared.h, line 22}) into data.__cl before the
        // GetMsg() call; the cookie is never read back (GetMsg's field-swap only touches
        // pData/nElem/allocated), so it is dropped here as inconsequential debug bookkeeping
        // and `msg` is simply value-initialized.
        CON_MSG msg = {};
        pComm->GetMsg(msg);

        fioFILE_MEM requestStream;
        requestStream.InitMemory(msg.data.pData, msg.data.nElem, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_R});
        requestStream.state.val |= 0x20u; // FIO_OPEN_BYTE_REORDER

        dsTSTRING<char> pageText;
        pageText.pBuffer = nullptr;
        pageText.UnsafeInitEmpty();
        dsTSTRING<char> ctrlText;
        ctrlText.pBuffer = nullptr;
        ctrlText.UnsafeInitEmpty();
        dsTSTRING<char> msgText;
        msgText.pBuffer = nullptr;
        msgText.UnsafeInitEmpty();

        requestStream.Read(pageText.Lock(1024), 1024);
        FixupReadLength(pageText);
        requestStream.Read(ctrlText.Lock(1024), 1024);
        FixupReadLength(ctrlText);
        requestStream.Read(msgText.Lock(1024), 1024);
        FixupReadLength(msgText);

        if (msg.id == 1)
        {
            OnMsg(&pageText, &ctrlText, &msgText, &requestStream, nullptr);
        }
        else if (msg.id == 2)
        {
            resp.Term();
            resp.InitAllocate(256, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_W});
            resp.state.val |= 0x20u; // FIO_OPEN_BYTE_REORDER

            int handled = OnMsg(&pageText, &ctrlText, &msgText, &requestStream, &resp);
            pComm->SendMsg(resp.pBuf, resp.streamSize, (handled == 0) + 3);
        }

        ReleaseTString(msgText);
        ReleaseTString(ctrlText);
        ReleaseTString(pageText);
        // requestStream destructs automatically at scope exit (fioFILE_MEM::~fioFILE_MEM()).
        dlFree(msg.data.pData);
    }
    while (pComm->HaveMsg());
}

} // namespace con
