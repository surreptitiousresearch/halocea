#pragma once

/* Win32 overlapped-I/O boundary (kernel32). Declare, don't descend.
 *
 * WHY A HEADER AND NOT <windows.h>. The obvious fix for three TUs disagreeing about ReadFileEx is
 * to delete their hand-rolled externs and let the platform header be the single declaration — the
 * same fix that settled memset/strncpy. It does not work here, and the failure is structural
 * rather than a preference (measured 2026-08-06):
 *
 *     src/headers/_OVERLAPPED.h(6): error C2011: '_OVERLAPPED': 'struct' type redefinition
 *
 * The corpus reconstructs OVERLAPPED as its own `_OVERLAPPED` shim, and `blam_data_globals.h` —
 * included by ~2,950 TUs — is typed in terms of that shim. Pulling in <windows.h> anywhere that
 * reaches it redefines the struct. Removing the shim instead would be a corpus-wide retype of every
 * Win32 handle type, not a boundary fix. So the single authoritative declaration lives here, and
 * divergence becomes a compile error the same way it would have.
 *
 * Signatures follow Win32. `__stdcall` is kept on the import for the reason the callconv drain kept
 * the other eight: these are the corpus's only *authored* convention annotations (the DB spells the
 * same APIs `__fastcall`, which is IDA's label for the PPC register ABI, not source). It is inert on
 * x64 and on the real PPC target, and load-bearing for stack cleanup on any x86 port.
 *
 * NOT ASSERTED HERE: Win32 also calls lpCompletionRoutine itself with the stdcall convention on
 * x86. Spelling that would have to propagate to the completion routines' own definitions
 * (cache_file_blocking_io_completion_routine, cache_copy_FileIOCompletionRoutine) and through
 * cached_map_issue_async_request's async_fn parameter — and on x64 the mismatch compiles silently,
 * which is exactly the accident-of-target that hid 88 divergences until the callconv pass. The
 * callback type below therefore keeps the spelling the corpus already uses at both call sites;
 * changing it is a separate, wider edit and should be made deliberately, not as a side effect.
 */

#include <stdint.h>
#include "_OVERLAPPED.h"

/* Win32 LPOVERLAPPED_COMPLETION_ROUTINE. */
typedef void (*overlapped_completion_routine)(uint32_t dwErrorCode,
                                              uint32_t dwNumberOfBytesTransfered,
                                              _OVERLAPPED *lpOverlapped);

extern int __stdcall ReadFileEx(void *hFile, void *lpBuffer, uint32_t nNumberOfBytesToRead,
                                _OVERLAPPED *lpOverlapped,
                                overlapped_completion_routine lpCompletionRoutine);
