// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#pragma once

//////////////////////////////////////////////////////////////////////////
///  define the macros here. control the whole project source files
///

#undef	KMAX_TYPE_LEN
#define KMAX_TYPE_LEN	16      /// 
#define KMAX_ELF_NAME_LEN	32



#define GAME_MODE    1

#ifdef GAME_MODE
#	define GAME_DEBUG		1
#	define GAME_RELEAE   0
#else
#	define GAME_DEBUG		0
#   define GAME_RELEASE  1
#endif

// use debug mode
#ifdef GAME_DEBUG
#	ifdef _DEBUG
#		define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#	else
#		define DEBUG_CLIENTBLOCK
#	endif // _DEBUG

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#	include <stdlib.h>
#ifndef OUR_DEBUG
#	include <crtdbg.h>
#	ifdef _DEBUG

#		define new DEBUG_CLIENTBLOCK
#	endif // _DEBUG

#else //OUR_DEBUG
#   include "debug_new_g.h"
#endif //OUR_DEBUG


#endif //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#endif // GAME_DEBUG

/// use release mode
#ifdef GAME_RELEASE



#endif
