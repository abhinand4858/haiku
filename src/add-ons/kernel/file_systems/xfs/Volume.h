#ifndef VOLUME_H
#define VOLUME_H

#include "system_dependencies.h"

#include "xfs.h"

enum volume_flags {
	VOLUME_READ_ONLY	= 0x0001
};


class Volume {
public:
							Volume(fs_volume* volume);
							~Volume();

			status_t		Mount(const char* device, uint32 flags);
			status_t		Unmount();

			dev_t			ID() const { return fVolume ? fVolume->id : -1; }
			fs_volume*		FSVolume() const { return fVolume; }
            const char*		Name() const { return fSuperBlock.fname; }


			xfs_super_block& SuperBlock() { return fSuperBlock; }
            int				Device() const { return fDevice; }
            uint32			BlockSize() const { return fBlockSize; }

            void*			BlockCache() { return fBlockCache; }
            off_t			NumBlocks() const
                                { return fSuperBlock.NumBlocks(); }

            off_t          Root() const { return fSuperBlock.rootino; }

            static	status_t		CheckSuperBlock(const uint8* data,
                                        uint32* _offset = NULL);
            static	status_t		Identify(int fd, xfs_super_block* superBlock);

protected:
			fs_volume*      fVolume;
            int				fDevice;
			xfs_super_block fSuperBlock;

            uint32          fDeviceBlockSize;
            uint32			fBlockSize;
			void*			fBlockCache; //handle
			mutex			fLock;

			uint32			fFlags;
};


#endif	// VOLUME_H
