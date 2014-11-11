// ASprite_defines.h - ASprite specific macros
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ASPRITE_DEFINE_H__
#define __ASPRITE_DEFINE_H__

//#define USE_MULTIPLE_MODULE_TYPES
#define USE_PRECOMPUTED_FRAME_RECT
//#define USE_FRAME_COLL_RC

#if	(defined USE_MIDP1_API || defined USE_MIDP2_API)

#define GRAPHICS_LEFT		Graphics::LEFT
#define GRAPHICS_HCENTER	Graphics::HCENTER
#define GRAPHICS_RIGHT		Graphics::RIGHT

#define GRAPHICS_TOP		Graphics::TOP
#define GRAPHICS_VCENTER	Graphics::VCENTER
#define GRAPHICS_BOTTOM		Graphics::BOTTOM

#endif


#define T_PAL_BYTE					BBYTE
#define T_PAL_SHORT					short
#define T_PAL_INT					int

#define T_MODULE_DATA_OFF			int

////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(IMAGE_USAGE_RGB_ARRAYS)
	

	#define CREATE_T_PAL(a,r,g,b)	((((((a << 8) + r) << 8) + g) << 8) + b)
	#define	MAP_COLOR(val)			pal[val]

	#ifdef USE_CACHE_RGB_ARRAYS
		#define T_IMAGE				int*
	#else //USE_CACHE_RGB_ARRAYS
		#define T_IMAGE				Image
	#endif //USE_CACHE_RGB_ARRAYS

#endif //IMAGE_USAGE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_SINGLE_ARRAY_FOR_FM_AF
	#define _FMODULES_ID(sprite, fmodule)	(sprite->_fmodules[(fmodule << 2)] & 0xFF)
	#define _FMODULES_OX(fmodule)	(_fmodules[(fmodule << 2) + 1])
	#define _FMODULES_OY(fmodule)	(_fmodules[(fmodule << 2) + 2])
	#define _FMODULES_FLAG(fmodule)	(_fmodules[(fmodule << 2) + 3])
#else //USE_SINGLE_ARRAY_FOR_FM_AF
	#define _FMODULES_ID(sprite, fmodule)	(sprite->_fmodules_id[fmodule] & 0xFF)
	#define _FMODULES_OX(fmodule)	(_fmodules_ox[fmodule])
	#define _FMODULES_OY(fmodule)	(_fmodules_oy[fmodule])
	#define _FMODULES_FLAG(fmodule)	(_fmodules_flag[fmodule])
#endif //USE_SINGLE_ARRAY_FOR_FM_AF


#ifdef USE_MODULES_WH_SHORT
	#define T_MODULE_WH				short
	#define MODULE_WH_MASK			0xFFFF
#else //USE_MODULES_WH_SHORT
	#define T_MODULE_WH				short//BBYTE
	#define MODULE_WH_MASK			0xFF
#endif //USE_MODULES_WH_SHORT

#ifdef USE_FM_OFF_SHORT
	#define T_FMODULE_OFF			short
#else //USE_FM_OFF_SHORT
	#define T_FMODULE_OFF			BBYTE
#endif //USE_FM_OFF_SHORT

#ifdef USE_AF_OFF_SHORT
	#define T_AFRAME_OFF			short
#else //USE_AF_OFF_SHORT
	#define T_AFRAME_OFF			BBYTE
#endif //USE_AF_OFF_SHORT

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_MULTIPLE_MODULE_TYPES

	#define MD_IMAGE		0
	#define MD_RECT			(-1)
	#define MD_FILL_RECT	(-2)
	#define MD_ARC			(-3)
	#define MD_FILL_ARC		(-4)

#endif	

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_BSPRITE_FLAGS
	#define TEST_BSPRITE_FLAGS(x)	if ((_bs_flags & (x)) != 0)
#else
	#define TEST_BSPRITE_FLAGS(x)
#endif

#define PAL_COLOR(c) (c) 

#endif //__ASPRITE_DEFINE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
