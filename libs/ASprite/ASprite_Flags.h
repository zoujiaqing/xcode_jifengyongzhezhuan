// ASprite_Flags.hxx - ASprite Flags
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ASPRITE_FLAG_H__
#define __ASPRITE_FLAG_H__
	//////////////////////////////////////////////////

	#define BSPRITE_v003	 (short)0x03DF // supported version

	//////////////////////////////////////////////////
	// BSprite flags

	#define BS_MODULES				(1 << 0)		// modules
	#define BS_MODULES_XY			(1 << 1)
	#define BS_MODULES_IMG			(1 << 2)
	#define BS_MODULES_WH_SHORT		(1 << 4)		// export w, h for each module as short
	#define BS_MODULES_XY_SHORT		(1 << 5)		// export x, y for each module as short
	#define BS_MODULES_USAGE		(1 << 6)		// export for each module which transformations are used in the sprite
	#define BS_FRAMES				(1 << 8)		// frames
	#define BS_FM_OFF_SHORT			(1 << 10)    // export fm offsets as shorts
	#define BS_NFM_1_BYTE			(1 << 11)    // export nfm as byte
	#define BS_SKIP_FRAME_RC		(1 << 12)    // do not export frame rect
	#define BS_FRAME_COLL_RC		(1 << 13)	// export collision rect
	#define BS_FM_PALETTE			(1 << 14)	// export palette used by the module
	#define BS_FRAME_RECTS			(1 << 15)	// export frame rects
	#define BS_ANIMS				(1 << 16)	// animations
	#define BS_NO_AF_START			(1 << 17)	// do not export start of AFrames
	#define BS_AF_OFF_SHORT			(1 << 18)    // export af offsets as shorts
	#define BS_NAF_1_BYTE			(1 << 19)    // export naf as byte

#define BS_RGB_8888_IMAGES		(1 << 20)
#define BS_RGB_4444_IMAGES		(1 << 21)
#define BS_RGB_565_IMAGES		(1 << 22)

	#define BS_MODULE_IMAGES_FX		(1 << 23)	// export encoded images for each module (flipped horizontally)
	#define BS_MODULE_IMAGES		(1 << 24)
	#define BS_PNG_CRC				(1 << 25)
	#define BS_KEEP_PAL				(1 << 26)
	#define BS_TRANSP_FIRST			(1 << 27)
	#define BS_TRANSP_LAST			(1 << 28)
	#define BS_SINGLE_IMAGE			(1 << 29)
	#define BS_MODULE_USAGE			(1 << 30)
	#define BS_GIF_HEADER			(1 << 31)	// export gif header instead of palette

	#define BS_DEFAULT_DOJA	 (BS_MODULES | BS_FRAMES | BS_ANIMS)
	#define BS_DEFAULT_MIDP2	 (BS_MODULES | BS_FRAMES | BS_ANIMS | BS_MODULE_IMAGES)
	#define BS_DEFAULT_NOKIA	 BS_DEFAULT_MIDP2
	#define BS_DEFAULT_MIDP1	 (BS_MODULES | BS_MODULES_XY | BS_FRAMES | BS_ANIMS)
	#define BS_DEFAULT_MIDP1b	 (BS_MODULES | BS_FRAMES | BS_ANIMS | BS_MODULE_IMAGES | BS_PNG_CRC)
	#define BS_DEFAULT_MIDP1c	 (BS_MODULES | BS_MODULES_XY | BS_FRAMES | BS_ANIMS | BS_SINGLE_IMAGE)

	//////////////////////////////////////////////////

	#define PIXEL_FORMAT_8888		 (short)0x8888
	#define PIXEL_FORMAT_4444		 (short)0x4444
	#define PIXEL_FORMAT_1555		 (short)0x5515
	#define PIXEL_FORMAT_0565		 (short)0x6505

	//////////////////////////////////////////////////

	#define ENCODE_FORMAT_I2		0x0200
	#define ENCODE_FORMAT_I4		0x0400
	#define ENCODE_FORMAT_I8		0x0800
	#define ENCODE_FORMAT_I16		0x1600
//	#define ENCODE_FORMAT_I16MP		0x16??
//	#define ENCODE_FORMAT_I32		0x3200
//	#define ENCODE_FORMAT_I64		0x6400
//	#define ENCODE_FORMAT_I128		0x2801
	#define ENCODE_FORMAT_I256		0x5602
//	#define ENCODE_FORMAT_I127_		0x2701
	#define ENCODE_FORMAT_I64RLE	0x64F0
	#define ENCODE_FORMAT_I127RLE	0x27F1
	#define ENCODE_FORMAT_I256RLE	0x56F2

	//////////////////////////////////////////////////
	// Frames/Anims flags...
	// --- BREW ver - moved in ASprite.h
	/*
	#define FLAG_FLIP_X	 0x01
	#define FLAG_FLIP_Y	 0x02
	#define FLAG_ROT_90	 0x04
	*/

	#define FLAG_USER0	 0x10 // user flag 0
	#define FLAG_USER1	 0x20 // user flag 1

	#define FLAG_HYPER_FM	 0x10 // Hyper FModule, used by FModules

	// Index extension...
	#define FLAG_INDEX_EX_MASK  0xC0 // 11000000, bits 6, 7
	#define INDEX_MASK			 0x03FF // max 1024 values
	#define INDEX_EX_MASK		 0x0300
	#define INDEX_EX_SHIFT 	 2

	//////////////////////////////////////////////////
	// flags passed as params...

	#define FLAG_OFFSET_FM  0x10
	#define FLAG_OFFSET_AF  0x20

	//////////////////////////////////////////////////

#endif // __ASPRITE_FLAG_H__

