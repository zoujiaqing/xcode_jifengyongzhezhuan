/* 
 * debug_new.h 
 * 
 * Header file for checking leakage by operator new 
 * 
 * 
 * 
 */ 


#ifndef _DEBUG_NEW_G_H 
#define _DEBUG_NEW_G_H 
#ifdef OUR_DEBUG
#ifdef __cplusplus

#if defined(_USRDLL)
#define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CC_DLL     __declspec(dllimport)
#endif


CC_DLL   void*  operator new(size_t size, const char* file, int line); 
CC_DLL   void*  operator new[](size_t size, const char* file, int line); 


  


  



CC_DLL void  operator delete(void* pointer, const char* file, int line) ;
CC_DLL void  operator delete[](void* pointer, const char* file, int line);
__inline void  operator delete(void* pointer)   
{   
operator delete(pointer ,__FILE__ ,__LINE__);
}   
__inline   void   operator delete[](void* pointer)   
{   
operator delete[](pointer ,__FILE__ ,__LINE__);
}

 

__inline  void*  operator new(size_t size)   
{   
	return operator new(size, __FILE__, __LINE__ );   
}   

__inline  void*  operator new[](size_t size)   
{   
	return operator new(size, __FILE__, __LINE__ );  
} 

#define new  DEBUGMFC_NEW 
#define DEBUGMFC_NEW   new(__FILE__, __LINE__)	
#define _MFC_OVERRIDES_NEW


#endif
#if  0
extern "C" void* debugmy_malloc(size_t size, const char* file, int line); 
extern "C" void debugmy_free(void * pointer);
 



#define debugmy_malloc  DEBUGMFC_MALLOC
#define DEBUGMFC_MALLOC   debugmy_malloc(__FILE__, __LINE__)
#define malloc     debugmy_malloc
#define free       debugmy_free
#endif
#endif // OUR_DEBUG
#endif // _DEBUG_NEW_H 