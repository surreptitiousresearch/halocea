#pragma once

/* Minimal Win32 boundary declarations (kernel32) used by file I/O paths. */
typedef void *HANDLE;
#define WIN32_INVALID_HANDLE ((void *)-1)

/* _CONTEXT — the PowerPC CPU register-dump structure used by stack-walking/exception code.
   Only ever touched through a bare pointer in this codebase so far; declared opaque. */
typedef struct _CONTEXT _CONTEXT;

extern void *CreateFileA(const char *name, unsigned int access, unsigned int share,
                         void *security, unsigned int disposition,
                         unsigned int flags, void *templ);
extern int CloseHandle(void *handle);
extern unsigned int GetFileSize(void *handle, unsigned int *high);
extern int ReadFile(void *handle, void *buffer, unsigned int size,
                    unsigned int *read, void *overlapped);

extern int WriteFile(void *handle, const void *buffer, unsigned int size,
                     unsigned int *written, void *overlapped);
