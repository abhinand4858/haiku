#include "system_dependencies.h"
#include "Volume.h"

#ifndef FS_SHELL
#	include <io_requests.h>
#	include <util/fs_trim_support.h>
#endif

#define TRACE_XFS
#ifdef TRACE_XFS
#	define TRACE(x...) dprintf("\n\33[34mxfs:\33[0m " x)
#else
#	define TRACE(x...) ;
#endif



fs_volume_ops gXFSVolumeOps = {
        NULL,
        NULL,
        NULL,
        NULL,
};

struct identify_cookie {
    xfs_super_block super_block;
};


static status_t
xfs_std_ops(int32 op, ...)
{
    switch (op) {
        case B_MODULE_INIT:
                return B_OK;
        case B_MODULE_UNINIT:
                return B_OK;

        default:
                return B_ERROR;
    }
}

static float
xfs_identify_partition(int fd, partition_data* partition, void** _cookie)
{
    TRACE("Identify partitioned being called\n");
    xfs_super_block superBlock;
    status_t status = Volume::Identify(fd, &superBlock);
    if (status != B_OK)
        return -1;

    identify_cookie* cookie = new(std::nothrow) identify_cookie;
    if (cookie == NULL)
        return -1;

    memcpy(&cookie->super_block, &superBlock, sizeof(xfs_super_block));

    *_cookie = cookie;
    return 0.8f;
}



static status_t
xfs_mount(fs_volume* _volume, const char* device, uint32 flags,
          const char* args, ino_t* _rootID)
{
    TRACE("xfs_mount():Mount hook being called\n");
    //FUNCTION();

    Volume* volume = new(std::nothrow) Volume(_volume);
    if (volume == NULL)
        return B_NO_MEMORY;

    status_t status = volume->Mount(device, flags);
    if (status != B_OK) {
        delete volume;
        TRACE("ERROR in Volume::Mount()");
    }

    _volume->private_volume = volume;
    _volume->ops = &gXFSVolumeOps;
    *_rootID = volume->Root();

    TRACE("mounted \"%s\" (root node at %" B_PRIdINO ", device = %s)\n",
            volume->Name(), *_rootID, device);
    return B_OK;
}

static status_t
xfs_scan_partition(int fd, partition_data* partition, void* _cookie)
{
    TRACE("Scan partition is called\n");
/*    identify_cookie* cookie = (identify_cookie*)_cookie;

    partition->status = B_PARTITION_VALID;
    partition->flags |= B_PARTITION_FILE_SYSTEM;
    partition->content_size = cookie->super_block.NumBlocks()
                              * cookie->super_block.BlockSize();
    partition->block_size = cookie->super_block.BlockSize();
    partition->content_name = strdup(cookie->super_block.name);
    if (partition->content_name == NULL)
        return B_NO_MEMORY;*/

    return B_OK;
}




static file_system_module_info sXFileSystem = {
        {
                "file_systems/xfs" B_CURRENT_FS_API_VERSION,
        0,
        xfs_std_ops,
        },

        "xfs",						// short_name
        "XFS File System",			// pretty_name

        // DDM flags
        0
        //	| B_DISK_SYSTEM_SUPPORTS_CHECKING
        //	| B_DISK_SYSTEM_SUPPORTS_REPAIRING
        //	| B_DISK_SYSTEM_SUPPORTS_RESIZING
        //	| B_DISK_SYSTEM_SUPPORTS_MOVING
        //	| B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_NAME
        //	| B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_PARAMETERS
        //  | B_DISK_SYSTEM_SUPPORTS_INITIALIZING
        //  | B_DISK_SYSTEM_SUPPORTS_CONTENT_NAME
        //	| B_DISK_SYSTEM_SUPPORTS_DEFRAGMENTING
        //	| B_DISK_SYSTEM_SUPPORTS_DEFRAGMENTING_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_CHECKING_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_REPAIRING_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_RESIZING_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_MOVING_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_NAME_WHILE_MOUNTED
        //	| B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_PARAMETERS_WHILE_MOUNTED
        //  | B_DISK_SYSTEM_SUPPORTS_WRITING
        ,

        // scanning
        xfs_identify_partition,
        xfs_scan_partition,
        NULL, //xfs_free_identify_partition_cookie,
        NULL,	// free_partition_content_cookie()

        &xfs_mount,

        /* capability querying operations */
        NULL, //&xfs_get_supported_operations,

        NULL,	// validate_resize
        NULL,	// validate_move
        NULL,	// validate_set_content_name
        NULL,	// validate_set_content_parameters
        NULL,	// validate_initialize,

        /* shadow partition modification */
        NULL,	// shadow_changed

        /* writing */
        NULL,	// defragment
        NULL,	// repair
        NULL,	// resize
        NULL,	// move
        NULL,	// set_content_name
        NULL,	// set_content_parameters
        NULL,   //xfs_initialize,
        NULL,   //xfs_uninitialize
};

module_info* modules[] = {
        (module_info*)&sXFileSystem,
        NULL,
};
