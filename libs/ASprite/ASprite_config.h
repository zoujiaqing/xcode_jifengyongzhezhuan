// ASprite_config.h - ASprite platform specific configuration
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ASPRITE_CONFIG_H__
#define __ASPRITE_CONFIG_H__

#ifndef USE_SLOW_PHONE_TUNNING
	#define TMP_BUFFER_SIZE												 (60 * 1024) 
#else //USE_SLOW_PHONE_TUNNING
	#define TMP_BUFFER_SIZE												 (10 * 1024) 
#endif //USE_SLOW_PHONE_TUNNING

////////////////////////////////////////////////////////////////////////////////////////////////////
// Set these switches to best suite your game!
// Unused code will be removed by the obfuscator!

	#define WRAPTEXT_TYPE short

//	#define USE_MIDP1_API
	#define USE_MIDP2_API

	#define IMAGE_USAGE_RGB_ARRAYS

	#define USE_MODULE_IMAGES

	#define USE_FAST_VISIBILITY_TEST
	#define USE_MODULE_MAPPINGS

	//#define USE_OPERATION_COMPUTERECT

	#define USE_HYPER_FM
	#define USE_INDEX_EX_FMODULES
	#define USE_INDEX_EX_AFRAMES

	#define USE_PIXEL_FORMAT_4444
	#define USE_PIXEL_FORMAT_8888
	#define USE_PIXEL_FORMAT_0565
	#define USE_ENCODE_FORMAT_I2
	#define USE_ENCODE_FORMAT_I4
	#define USE_ENCODE_FORMAT_I8
	#define USE_ENCODE_FORMAT_I16
	#define USE_ENCODE_FORMAT_I256
	#define USE_ENCODE_FORMAT_I64RLE
	#define USE_ENCODE_FORMAT_I127RLE
	#define USE_ENCODE_FORMAT_I256RLE

	#define USE_TRANSF_ROT
	#define USE_TRANSF_FLIP

	#define USE_MODULES_WH_SHORT
	#define USE_FM_OFF_SHORT
	#define USE_BSPRITE_FLAGS
	#define USE_FM_PALETTE

    #define USE_AF_OFF_SHORT

typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef signed char     GLbyte;
typedef short           GLshort;
typedef int             GLint;
typedef int             GLsizei;
typedef unsigned char   GLubyte;
typedef unsigned short  GLushort;
typedef unsigned int    GLuint;
typedef float           GLfloat;
typedef float           GLclampf;
typedef void            GLvoid;
typedef int             GLfixed;
typedef int             GLclampx;

typedef unsigned char BBYTE;
typedef unsigned char boolean;

typedef unsigned long int uint32;
typedef unsigned char uint8;
#define T_PAL int
#define gll_new new
#define null 0
#define SAFE_DEL(a) if(a) {delete a; a = null;}
#define SAFE_DEL_ARRAY(a) if(a) {delete [] a; a = null;}
#define ASSERT CCAssert

#endif // __ASPRITE_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
