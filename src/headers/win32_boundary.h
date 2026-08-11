#pragma once

/* Minimal Win32 boundary declarations (kernel32) used by file I/O paths. */
typedef void *HANDLE;
#define WIN32_INVALID_HANDLE ((void *)-1)

/* _CONTEXT — the PowerPC CPU register-dump structure used by stack-walking/exception code.
   Only ever touched through a bare pointer in this codebase so far; declared opaque. */
typedef struct _CONTEXT _CONTEXT;

#ifdef __cplusplus
extern "C" {
#endif
extern void *CreateFileA(const char *name, unsigned int access, unsigned int share,
                         void *security, unsigned int disposition,
                         unsigned int flags, void *templ);
extern int CloseHandle(void *handle);
extern unsigned int GetFileSize(void *handle, unsigned int *high);
extern int ReadFile(void *handle, void *buffer, unsigned int size,
                    unsigned int *read, void *overlapped);

extern int WriteFile(void *handle, const void *buffer, unsigned int size,
                     unsigned int *written, void *overlapped);
/* ws/os/os_boundary.h cannot be the single home for this: it opens a bare `extern "C" {` with no
 * __cplusplus guard, so no C TU can include it. The corpus's kernel32 boundary is already split
 * that way on purpose — CreateFileA/CloseHandle/GetFileSize/WriteFile are declared verbatim in
 * both. Spelled from the DB decl (unsigned int __fastcall SetFilePointer(void *hFile, int
 * lDistanceToMove, int *lpDistanceToMoveHigh, unsigned int dwMoveMethod)), which is also the
 * spelling the eight in-corpus local externs use (file_get_position.c, file_set_eof.c,
 * game_state_write_to_persistent_storage.c, …); os_boundary.h's `void *highPtr` is the outlier. */
extern unsigned int SetFilePointer(void *hFile, int lDistanceToMove, int *lpDistanceToMoveHigh,
                                   unsigned int dwMoveMethod);
#ifdef __cplusplus
}
#endif
