#pragma once
// ws-engine os: opaque placeholder type for a Win32/xboxkrnl synchronization HANDLE
// (event/semaphore/etc.). DB-verified (types_members osHANDLE_DUMMY): no members --
// deliberately opaque; real values are plain Win32 HANDLEs (void*) cast through a
// pointer-to-this-type for type safety. Never dereferenced.
struct osHANDLE_DUMMY;
