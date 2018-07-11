#include "Inode.h"


Inode::Inode(Volume* volume, ino_t id)
        :
        fVolume(volume),
        fID(id),
        fCache(NULL),
        fMap(NULL)
{
    PRINT(("Inode::Inode(volume = %p, id = %Ld) @ %p\n", volume, id, this));

    rw_lock_init(&fLock, "xfs inode");
}

Inode::~Inode()
{
    TRACE("Inode destructor\n");
    file_cache_delete(FileCache());
    file_map_delete(Map());
    TRACE("Inode destructor: Done\n");
}