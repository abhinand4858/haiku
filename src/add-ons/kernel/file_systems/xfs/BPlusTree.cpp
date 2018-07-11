#include "BPlusTree.h"
#include "Inode.h"

#define TRACE_XFS
#ifdef TRACE_XFS
#	define TRACE(x...) dprintf("\n\33[34mxfs:\33[0m " x)
#else
#	define TRACE(x...) ;
#endif