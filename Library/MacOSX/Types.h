////////////////////////////////////////////////////////////////////////////////
// Types.h
////////////////////////////////////////////////////////////////////////////////

#ifndef	HOST_COMMON_INCLUDE_TYPES_H_INCLUDED_37C49TY24C
#define	HOST_COMMON_INCLUDE_TYPES_H_INCLUDED_37C49TY24C

#if	defined(_WIN32)

#include <windows.h>

#else

////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////

typedef int							BOOL;
typedef unsigned char				BYTE;
typedef unsigned short				WORD;
typedef unsigned int				UINT;
#if	defined(__APPLE__)
typedef unsigned long				DWORD;
typedef unsigned long long			ULONGLONG;
#else
typedef unsigned int				DWORD;
typedef unsigned int				ULONG;
#define ULONGLONG int64_t
#endif

typedef char						*LPSTR;
typedef const char					*LPCSTR;
typedef BOOL						*LPBOOL;
typedef BYTE						*LPBYTE;
typedef WORD						*LPWORD;
typedef UINT						*LPUINT;
typedef DWORD						*LPDWORD;
typedef void						*LPVOID;

typedef void						*HANDLE;
typedef HANDLE						*LPHANDLE;

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define INFINITE					0xFFFFFFFF

#if	!defined(FALSE)
#define	FALSE (0)
#endif
#if	!defined(TRUE)
#define	TRUE (1)	// TODO: or !FALSE?
#endif

#define MIN(a,b)                                        ((a)<(b)?(a):(b))
#define MAX(a,b)                                        ((a)>(b)?(a):(b))

// TODO: Delete these. Use host_common/silabs_utils.h::SILABS_MAKEWORD(), SILABS_LOBYTE() & SILABS_HIBYTE()
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define LOWORD(l)                                       ((WORD)(l))
#define HIWORD(l)                                       ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#endif	// defined(_WIN32)

#endif // HOST_COMMON_INCLUDE_TYPES_H_INCLUDED_37C49TY24C

