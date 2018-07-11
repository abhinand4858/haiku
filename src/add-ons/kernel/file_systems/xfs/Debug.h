#ifndef DEBUG_H
#define DEBUG_H

#include "system_dependencies.h"

#ifdef USER
#	define __out printf
#else
#	define __out dprintf
#endif

// Short overview over the debug output macros:
//	PRINT()
//		is for general messages that very unlikely should appear in a release build
//	FATAL()
//		this is for fatal messages, when something has really gone wrong
//	INFORM()
//		general information, as disk size, etc.
//	REPORT_ERROR(status_t)
//		prints out error information
//	RETURN_ERROR(status_t)
//		calls REPORT_ERROR() and return the value
//	D()
//		the statements in D() are only included if DEBUG is defined

#if 0//DEBUG
	#define PRINT(x) { __out("bfs: "); __out x; }
	#define FATAL(x) { __out("bfs: "); __out x; }
#else
	#define PRINT(x) { __out("bfs: "); __out x; }
	#define FATAL(x) { __out("bfs: "); __out x; }
#endif

#endif	/* DEBUG_H */
