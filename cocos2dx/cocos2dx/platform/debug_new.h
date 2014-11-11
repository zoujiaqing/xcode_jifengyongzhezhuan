/* 
 * debug_new.h 
 * 
 * Header file for checking leakage by operator new 
 * 
 * 
 * 
 */ 
 
#ifndef _DEBUG_NEW_H 
#define _DEBUG_NEW_H 
#ifdef OUR_DEBUG
//#include "debug_new.cpp"
//#include <new> 
//#include "CCPlatformDefine.h"
//#include "cocoa/CCObject.h"

#if defined(_USRDLL)
#define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CC_DLL     __declspec(dllimport)
#endif

/* Prototypes */ 
CC_DLL extern  bool  check_leaks(); 
CC_DLL extern  bool  check_memory()  ;
#ifndef _DEBUG_NEW_G_H
CC_DLL extern  void*  operator new(size_t size, const char* file, int line); 
CC_DLL  extern  void*  operator new[](size_t size, const char* file, int line); 
#endif
//
//
// CC_DLL void  operator delete(void* pointer, const char* file, int line) ;
// CC_DLL void  operator delete[](void* pointer, const char* file, int line);




/* Macros */ 
//#ifndef DEBUG_NEW_NO_NEW_REDEFINITION 
//#define new DEBUG_NEW 
//#define DEBUG_NEW new(__FILE__, __LINE__) 
//#define debug_new new 
//
//
//#else 
//#define debug_delete delete(__FILE__, __LINE__) 
//#endif // DEBUG_NEW_NO_NEW_REDEFINITION 
//#ifdef DEBUG_NEW_EMULATE_MALLOC 
////#include <stdlib.h> 
//#define malloc(s) ((void*)(debug_new char[s])) 
//#define free(p) delete[] (char*)(p) 
//#endif // DEBUG_NEW_EMULATE_MALLOC 
// 
//#define new  DEBUG_NEW 
//#define DEBUG_NEW   new(__FILE__, __LINE__)
///* Control flags */ 
#define new  DEBUG_NEW 
#define DEBUG_NEW   new(__FILE__, __LINE__)	

extern bool new_verbose_flag;	// default to false: no verbose information 
extern bool new_autocheck_flag;	// default to true: call check_leaks() on exit 
#endif // OUR_DEBUG
#endif // _DEBUG_NEW_H 