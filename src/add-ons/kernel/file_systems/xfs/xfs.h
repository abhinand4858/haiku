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
    uint32            agblocks;
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

    uint32      Magic() const { return B_HOST_TO_BENDIAN_INT32(magicnum); }
    uint32      BlockSize() const { return B_HOST_TO_BENDIAN_INT32(blocksize); }
    uint64      NumBlocks() const { return B_HOST_TO_BENDIAN_INT64(dblocks); }
    uint64      LogStart() const { return B_HOST_TO_BENDIAN_INT64(logstart); }
    xfs_ino_t   Root() const { return B_HOST_TO_BENDIAN_INT64(rootino); }
    uint32      BlocksPerAllocationGroup() const { return B_HOST_TO_BENDIAN_INT64(agblocks); }
    uint32      AllocationGroups() const { return B_HOST_TO_BENDIAN_INT64(agcount); }
    uint16      SectorSize() const { return B_HOST_TO_BENDIAN_INT64(sectsize); }
    uint16      InodeSize() const { return B_HOST_TO_BENDIAN_INT64(inodesize); }
    uint64      InodeTotalCount() const { return B_HOST_TO_BENDIAN_INT64(icount); }
    uint64      InodeFreeCount() const { return B_HOST_TO_BENDIAN_INT64(ifree); }

    // implemented in Volume.cpp:
    bool IsValid();
} _PACKED;

//**************************************
/*
 * Allocation group free space block
 * TODO
 */

//**************************************

/*
 * Allocation group header information
 *
 */
#define	XFS_AGI_MAGIC	0x58414749	/* 'XAGI' */
#define	XFS_AGI_VERSION	1


/*
 * Size of the unlinked inode hash table in the agi.
 */
#define	XFS_AGI_UNLINKED_BUCKETS	64

struct xfs_agi {
    uint32              agi_magicnum;
    uint32              agi_versionnum;
    uint32              agi_seqno;
    uint32              agi_length;
    /*
	 * Inode information
	 * Inodes are mapped by interpreting the inode number, so no
	 * mapping data is needed here.
	 */
    uint32              agi_count;
    uint32              agi_root;
    uint32              agi_level;
    uint32              agi_freecount;
    uint32              agi_newino;
    uint32              agi_dirino;
    /*
    * Hash table of inodes which have been unlinked but are
    * still being referenced.
    */
    uint32              agi_unlinked[64];
} _PACKED;

//**************************************

/* short form block header */
struct xfs_btree_sblock {
    uint32                    bb_magic;
    uint16                    bb_level;
    uint16                    bb_numrecs;
    uint32                    bb_leftsib;
    uint32                    bb_rightsib;

    //Version 5 filesystem entries to be included

    uint32 Magic() const { return B_HOST_TO_BENDIAN_INT32(bb_magic); }
    uint32 Level() const { return B_HOST_TO_BENDIAN_INT16(bb_level); }

} _PACKED;

/* long form block header */
struct xfs_btree_lblock {
    uint32                    bb_magic;
    uint16                    bb_level;
    uint16                    bb_numrecs;
    uint64                    bb_leftsib;
    uint64                    bb_rightsib;

    //Version 5 filesystem entries to be included

} _PACKED;

struct xfs_alloc_rec {
    uint32          ar_startblock;
    uint32          ar_blockcount;
} _PACKED;

typedef uint32 xfs_alloc_ptr;

//**************************************

/* Inode B+trees */

/* Block header */
typedef struct xfs_btree_sblock xfs_inobt_block;

/* Leaves */
struct xfs_inobt_rec {
    uint32                    ir_startino;
    uint32                    ir_freecount;
    uint64                    ir_free;
} _PACKED;

/* Node containing key-pointer pairs */
struct xfs_inobt_key {
    uint32                     ir_startino;
} _PACKED;

typedef uint32 xfs_inobt_ptr;

//**************************************

/*
 * Inode Core
 *
 */
#define	XFS_DINODE_MAGIC		0x494e	/* 'IN' */

struct xfs_timestamp {
    uint32		t_sec;		/* timestamp seconds */
    uint32		t_nsec;		/* timestamp nanoseconds */
} _PACKED;

struct xfs_dinode {
    uint16                di_magic;
    uint16                di_mode;      // see sys/stat.h
    int8                  di_version;
    int8                  di_format;
    uint16                di_onlink;
    uint32                di_uid;
    uint32                di_gid;
    uint32                di_nlink;
    uint16                di_projid;
    uint16                di_projid_hi;
    uint8                 di_pad[6];
    uint16                di_flushiter;
    xfs_timestamp_t     di_atime;
    xfs_timestamp_t     di_mtime;
    xfs_timestamp_t     di_ctime;
    uint64              di_size;
    uint64              di_nblocks;
    uint32              di_extsize;
    uint32              di_nextents;
    uint16              di_anextents;
    uint8               di_forkoff;
    int8                di_aformat;
    uint32              di_dmevmask;
    uint16              di_dmstate;
    uint16              di_flags;
    uint32              di_gen;

    /* di_next_unlinked is the only non-core field in the old dinode */
    uint32              di_next_unlinked;

    uint16 Magic() const { return B_HOST_TO_BENDIAN_INT32(di_magic); }
    uint16 Mode() const { return B_HOST_TO_BENDIAN_INT32(di_mode); }
    uint16 OnLink() const { return B_HOST_TO_BENDIAN_INT32(di_onlink); }
    uint32 UserID() const { return B_HOST_TO_BENDIAN_INT32(di_uid); }
    uint32 GroupID() const { return B_HOST_TO_BENDIAN_INT32(di_gid); }
    static void _DecodeTime(struct timespec& timespec,
                            const xfs_timestamp& time)
    {
        timespec.tv_sec = B_LENDIAN_TO_HOST_INT64(time.t_sec);
        timespec.tv_nsec = B_LENDIAN_TO_HOST_INT32(time.t_nsec);
    }
    void GetAccessTime(struct timespec& timespec) const
    { _DecodeTime(timespec, di_atime); }
    void GetModificationTime(struct timespec& timespec) const
    { _DecodeTime(timespec, di_mtime); }
    void GetChangeTime(struct timespec& timespec) const
    { _DecodeTime(timespec, di_ctime); }

} _PACKED;

enum xfs_dinode_fmt {
    XFS_DINODE_FMT_DEV,
    XFS_DINODE_FMT_LOCAL,
    XFS_DINODE_FMT_EXTENTS,
    XFS_DINODE_FMT_BTREE,
    XFS_DINODE_FMT_UUID
};


//**************************************


/*
 * Data extents
 * Allocates space for a file using extents
 *
*/

/* B+tree Extent List */

#define XFS_BMAP_MAGIC		0x424d4150	/* 'BMAP' */

struct xfs_bmbt_irec {
    xfs_fileoff_t             br_startoff;
    xfs_fsblock_t             br_startblock;
    xfs_filblks_t             br_blockcount;
    xfs_exntst_t              br_state;
} _PACKED;

typedef enum {
    XFS_EXT_NORM,
    XFS_EXT_UNWRITTEN,
    XFS_EXT_INVALID
} xfs_exntst_t;


// BMAP root header
struct xfs_bmdr_block {
    uint16                     bb_level;
    uint16                     bb_numrecs;
} _PACKED;

//Key structure for non-leaf levels of the tree
struct xfs_bmbt_key {
    xfs_fileoff_t              br_startoff;
} _PACKED;

//btree pointer type
typedef uint64 xfs_bmbt_ptr_t, xfs_bmdr_ptr_t;

//B+Tree extent list
typedef struct xfs_btree_lblock xfs_bmbt_block;

//**************************************

/*
 * Directories
 */

/* Directory layout when stored internal to an inode */
struct xfs_dir2_sf_hdr {
    uint8       count;
    uint8       i8count;
    uint8       parent[8];
} _PACKED;

struct xfs_dir2_sf_entry {
    uint8       namelen;
    uint8       offset[2];
    uint8       name[1];
    //TODO: Version 3 directory entries
} _PACKED;


#endif /*XFS_H*/
