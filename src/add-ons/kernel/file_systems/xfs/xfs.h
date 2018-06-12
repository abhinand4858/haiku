#ifndef XFS_H
#define XFS_H

#include "system_dependencies.h"

#define UUID_SIZE 16
#define XFS_SB_MAGIC   0x58465342

typedef unsigned long long xfs_ino_t;
typedef struct {
    uint8 b[UUID_SIZE];
} uuid_t;

struct xfs_super_block {
    uint32            magicnum;
    uint32            blocksize;
    uint64            dblocks;
    uint64            rblocks;
    uint64            rextents;
    uuid_t            uuid;
    uint64            logstart;
    xfs_ino_t         rootino;
    xfs_ino_t         rbmino;
    xfs_ino_t         rsumino;
    uint32            rextsize;
    uint              agblocks;
    uint32            agcount;
    uint32            rbmblocks;
    uint32            logblocks;
    uint16            versionnum;
    uint16            sectsize;
    uint16            inopblock;
    uint16            inodesize;
    char              fname[12];
    uint8             blocklog;
    uint8             sectlog;
    uint8             inodelog;
    uint8             inopblog;
    uint8             agblklog;
    uint8             rextslog;
    uint8             inprogress;
    uint8             imax_pct;
    uint64            icount;
    uint64            ifree;
    uint64            fdblocks;
    uint64            frextents;
    xfs_ino_t         uquotino;
    xfs_ino_t         gquotino;
    uint16            qflags;
    uint8             flags;
    uint8             shared_vn;
    uint32            inoalignmt;
    uint32            unit;
    uint32            width;
    uint8             dirblklog;
    uint8             logsectlog;
    uint16            logsectsize;
    uint32            logsunit;
    uint32            features2;

    uint32 BlockSize() const { return B_LENDIAN_TO_HOST_INT32(blocksize); }
    off_t NumBlocks() const { return B_LENDIAN_TO_HOST_INT64(dblocks); }


    // implemented in Volume.cpp:
    bool IsValid() const;
} _PACKED;

#endif /*XFS_H*/
