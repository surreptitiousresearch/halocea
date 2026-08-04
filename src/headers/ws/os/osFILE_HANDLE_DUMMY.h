#pragma once
// ws-engine os: opaque placeholder type for a Win32/xboxkrnl file HANDLE.
// DB-verified (types_members osFILE_HANDLE_DUMMY): no members -- it is a deliberately
// opaque tag struct; real values are plain Win32 HANDLEs (void*) cast through a
// pointer-to-this-type for type safety at the os* API boundary. Never dereferenced.
struct osFILE_HANDLE_DUMMY;
