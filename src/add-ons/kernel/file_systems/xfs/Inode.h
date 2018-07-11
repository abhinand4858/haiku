#ifndef INODE_H
#define INODE_H

#include "xfs.h"
#include "Volume.h"


#define TRACE_XFS
#ifdef TRACE_XFS
#	define TRACEI(x...) dprintf("\n\33[34mxfs:\33[0m " x)
#else
#	define TRACEI(x...) ;
#endif


class Inode {
public:
                Inode(Volume* volume, ino_t id);
                ~Inode();

    status_t	InitCheck();

    ino_t		ID() const { return fID; }

    rw_lock*	Lock() { return& fLock; }

    bool		IsDirectory() const
        { return S_ISDIR(Mode()); }
    bool		IsFile() const
        { return S_ISREG(Mode()); }
    bool		IsSymLink() const
        { return S_ISLNK(Mode()); }
    status_t	CheckPermissions(int accessMode) const;

    mode_t		Mode() const { return fNode.Mode(); }
    //size ??
    uid_t		UserID() const { return fNode.UserID(); }
    gid_t		GroupID() const { return fNode.GroupID(); }
    void		GetChangeTime(struct timespec& timespec) const
    { fNode.GetChangeTime(timespec); }
    void		GetModificationTime(struct timespec& timespec) const
    { fNode.GetModificationTime(timespec); }
    void		GetCreationTime(struct timespec& timespec) const
    { fNode.GetCreationTime(timespec); }

    Volume*		GetVolume() const { return fVolume; }

private:
        rw_lock		        fLock;
        Volume*	            fVolume;
        ino_t		        fID;
        void*		        fCache;
        void*		        fMap;
        status_t	        fInitStatus;
        xfs_dinode_core     fNode;
};


// The Vnode class provides a convenience layer upon get_vnode(), so that
// you don't have to call put_vnode() anymore, which may make code more
// readable in some cases

class Vnode {
public:
    Vnode(Volume* volume, ino_t id)
            :
            fInode(NULL)
    {
        SetTo(volume, id);
    }

    Vnode()
            :
            fStatus(B_NO_INIT),
            fInode(NULL)
    {
    }

    ~Vnode()
    {
        Unset();
    }

    status_t InitCheck()
    {
        return fStatus;
    }

    void Unset()
    {
        if (fInode != NULL) {
            put_vnode(fInode->GetVolume()->FSVolume(), fInode->ID());
            fInode = NULL;
            fStatus = B_NO_INIT;
        }
    }

    status_t SetTo(Volume* volume, ino_t id)
    {
        Unset();

        return fStatus = get_vnode(volume->FSVolume(), id, (void**)&fInode);
    }

    status_t Get(Inode** _inode)
    {
        *_inode = fInode;
        return fStatus;
    }

    void Keep()
    {
        TRACEI("Vnode::Keep()\n");
        fInode = NULL;
    }

private:
    status_t	fStatus;
    Inode*		fInode;
};


#endif	// INODE_H