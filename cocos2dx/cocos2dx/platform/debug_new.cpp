/**  
 * debug_new.cpp  
 *  
 * Implementation of debug versions of new and delete to check leakage  
 *  
 *   
 *  
 */   

//
//#include <new>   
//#include <stdio.h>   
//#include <stdlib.h>  

//#include "ccMacros.h"
//#include "CCPlatformDefine.h"
//#include "cocoa/CCObject.h"

#ifdef OUR_DEBUG
#pragma push_macro("new")
#undef  new
#include "malloc.h"
#include <iostream>
#include <stdio.h>

#include <Windows.h>
#include "CCStdC.h"
using namespace  std ;

typedef unsigned int   size_t;
#if defined(_USRDLL)
#define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CC_DLL     __declspec(dllimport)
#endif
//#define new DEBUG_NEW 
//#define DEBUG_NEW new(__FILE__, __LINE__)   
#ifdef _MSC_VER   
#pragma warning(disable: 4073)   
#pragma init_seg(lib)   
#endif   
   
#ifndef DEBUG_NEW_HASHTABLESIZE   
#define DEBUG_NEW_HASHTABLESIZE 16384   
#endif   
   
#ifndef DEBUG_NEW_HASH   
#define DEBUG_NEW_HASH(p) (((unsigned)(p) >> 8) % DEBUG_NEW_HASHTABLESIZE)   
#endif   
   
// The default behaviour now is to copy the file name, because we found   
// that the exit leakage check cannot access the address of the file   
// name sometimes (in our case, a core dump will occur when trying to   
// access the file name in a shared library after a SIGINT).   
#ifndef DEBUG_NEW_FILENAME_LEN   
#define DEBUG_NEW_FILENAME_LEN  200   
#endif   
#if DEBUG_NEW_FILENAME_LEN == 0 && !defined(DEBUG_NEW_NO_FILENAME_COPY)   
#define DEBUG_NEW_NO_FILENAME_COPY   
#endif   
#ifndef DEBUG_NEW_NO_FILENAME_COPY   
#include <string.h>   
#endif   
   
struct new_ptr_list_t   
{   
    new_ptr_list_t*     next;   
#ifdef DEBUG_NEW_NO_FILENAME_COPY   
    const char*         file;   
#else   
    char                file[DEBUG_NEW_FILENAME_LEN];   
#endif   
    int                 line; 
	bool                calc ;
    size_t              size;   
};   
   
 new_ptr_list_t* new_ptr_list[DEBUG_NEW_HASHTABLESIZE];   
   
bool new_verbose_flag = false;   
bool new_autocheck_flag = true;  
new_ptr_list_t* mem_ptr_list = NULL ;
   

void MemLog(const char * pszFormat, ...)
{
	

	char szBuf[100];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, 100, 100, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[100] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	printf("%s\n", szBuf);

	
}
CC_DLL  bool  check_memory()   
{   
    bool fLeaked = false;   
	unsigned int total_all = 0 ;
	new_ptr_list_t*  cur_ptr = NULL ;
    for (int i = 0; i < DEBUG_NEW_HASHTABLESIZE; ++i)   
    {   
        new_ptr_list_t* ptr = new_ptr_list[i];  
		 new_ptr_list_t* ptr_lt = NULL;
		 bool isInside = false ;
	
        if (ptr == NULL)   
            continue;   
        fLeaked = true;   
		
	
        while (ptr)   
        {   
			ptr->calc = false ;
			/*	if(strcmp(ptr->file ,"\\cframenode.cpp")== 0 || strcmp(ptr->file ,"\\ccdictionary.cpp")== 0)

			MemLog("Leaked object at %p (size %u, %s:%d)\n",   
			(char*)ptr + sizeof(new_ptr_list_t),   
			ptr->size,   
			ptr->file,   
			ptr->line); */  
			total_all+= ptr->size ;

			isInside = false ;

			ptr_lt = mem_ptr_list ;


			while(ptr_lt)
			{
				if(strcmp(ptr->file ,ptr_lt->file)== 0 &&(ptr->line == ptr_lt->line))
				{
					isInside = true ;
					break;
				}

				ptr_lt= ptr_lt->next ;
			}


			if(!isInside)
			{
				if(mem_ptr_list== NULL)
				{
					mem_ptr_list = (new_ptr_list_t*)malloc(sizeof(new_ptr_list_t));
					cur_ptr = mem_ptr_list ;
				
					

				}
				else
				{
					cur_ptr->next = (new_ptr_list_t*)malloc(sizeof(new_ptr_list_t));
					cur_ptr = cur_ptr->next ;
					
				}
				cur_ptr->next = NULL ;
				cur_ptr->size = ptr->size ;
				cur_ptr->line = ptr->line ;
				strcpy(cur_ptr->file ,ptr->file);
			}  //!isInside
			else //!isInside
			{
			     ptr_lt->size += ptr->size ;
			}  //!isInside
			
			ptr= ptr->next ;
			

        }  //while (ptr) 

    }   // for (int i = 0; i < DEBUG_NEW_HASHTABLESIZE; ++i)   
	float total_f = (float)((float)total_all /(float)(1024*1024)) ;
	//MemLog("check_mem total_all memory : %fM \n",total_f);
	while(mem_ptr_list)
	{
		cur_ptr = mem_ptr_list ;
		mem_ptr_list = mem_ptr_list->next ;
		float size_num =(float)((float)cur_ptr->size /(float)(1024*1024));
		 float per =size_num/total_f ;
		 //if(per>0.01)
			// //MemLog("check_mem new memory at (%s:%d  total_mem:%fM,%f)\n", 
			// //cur_ptr->file,  
			// //cur_ptr->line,
			// //size_num,
			// //per
			// //); 
		free(cur_ptr);


	}
	
	 
    return fLeaked ;
}   
CC_DLL  bool  check_leaks()   
{   
    bool fLeaked = false;   
	unsigned int total_all = 0 ;
    for (int i = 0; i < DEBUG_NEW_HASHTABLESIZE; ++i)   
    {   
        new_ptr_list_t* ptr = new_ptr_list[i];  
	
        if (ptr == NULL)   
            continue;   
        fLeaked = true;   
		
	
        while (ptr)   
        {   
			ptr->calc = false ;
			 MemLog("Leaked object at %p (size %u, %s:%d)\n",   
			(char*)ptr + sizeof(new_ptr_list_t),   
			ptr->size,   
			ptr->file,   
			ptr->line);   
			total_all+= ptr->size ;
			ptr= ptr->next ;
			

        }  //while (ptr) 

    }   // for (int i = 0; i < DEBUG_NEW_HASHTABLESIZE; ++i)   
	float total_f = (float)((float)total_all /(float)(1024*1024)) ;
		MemLog("check_leaks total_all memory : %fM \n",total_f);
	 return fLeaked ;
	 
    
}   
   
   
 CC_DLL void*  operator new(size_t size, const char* file, int line)   
{   
    size_t s = size + sizeof(new_ptr_list_t);   
    new_ptr_list_t* ptr = (new_ptr_list_t*)malloc(s);   
    if (ptr == NULL)   
    {   
        fprintf(stderr, "new:  out of memory when allocating %u bytes\n",   
                size);   
       // abort();   
    }   
    void* pointer = (char*)ptr + sizeof(new_ptr_list_t);   
    size_t hash_index = DEBUG_NEW_HASH(pointer);   
    ptr->next = new_ptr_list[hash_index];  
	char * ttr = (char *)file ;
	char * ctr = strrchr(ttr ,'\\');
	if(ctr == NULL)
		ctr = ttr ;
#ifdef DEBUG_NEW_NO_FILENAME_COPY   
    ptr->file = file;   
#else   
    strncpy(ptr->file, ctr, DEBUG_NEW_FILENAME_LEN - 1);   
    ptr->file[DEBUG_NEW_FILENAME_LEN - 1] = '\0';   
#endif   
    ptr->line = line;   
    ptr->size = size;   
    new_ptr_list[hash_index] = ptr;   
    if (new_verbose_flag)   
        printf("new:  allocated  %p (size %u, %s:%d)\n",   
                pointer, size, file, line);   
    return pointer;   
}   
   
CC_DLL void*   operator new[](size_t size, const char* file, int line)   
{   
    return operator new(size, file, line);   
}   
   
//CC_DLL void*  operator new(size_t size)   
//{   
//	return operator new(size, "<Unknown>", 0);   
//}   
//
//CC_DLL void*  operator new[](size_t size)   
//{   
//	return operator new(size);   
//}   
//
/*CC_DLL void*   operator new(size_t size, const std::nothrow_t&) throw()   
{   
return operator new(size);   
}   

CC_DLL void*   operator new[](size_t size, const std::nothrow_t&) throw()   
{   
return operator new[](size);   
}  */ 



 CC_DLL void  operator delete(void* pointer, const char* file, int line)
 {   
	 if (new_verbose_flag)   
		 printf("info: exception thrown on initializing object at %p (%s:%d)\n",   
		 pointer, file, line);
	 if (pointer == NULL)   
		 return;   
	 size_t hash_index = DEBUG_NEW_HASH(pointer);   
	 new_ptr_list_t* ptr = new_ptr_list[hash_index];   
	 new_ptr_list_t* ptr_last = NULL;   
	 while (ptr)   
	 {   
		 if ((char*)ptr + sizeof(new_ptr_list_t) == pointer)   
		 {   
			 if (new_verbose_flag)   
				 printf("delete: freeing  %p (size %u)\n", pointer, ptr->size);   
			 if (ptr_last == NULL)   
				 new_ptr_list[hash_index] = ptr->next;   
			 else   
				 ptr_last->next = ptr->next;   
			 free(ptr);   
			 return;   
		 }   
		 ptr_last = ptr;   
		 ptr = ptr->next;   
	 }   
	 fprintf(stderr, "delete: invalid pointer %p\n", pointer);   
	 free(pointer);
	 // abort();   
	 //operator delete(pointer);   
 }   

 CC_DLL void  operator delete[](void* pointer, const char* file, int line)   
 {   
	 operator delete(pointer, file, line);   
 }   

#if 0
 void* debugmy_malloc(size_t size, const char* file, int line);
 {
     return  operator new(size ,file ,line);
 }
void debugmy_free(void * pointer)
 {
	  operator delete(pointer, __FILE__, __LINE__);   
 }
#endif
// Proxy class to automatically call check_leaks if new_autocheck_flag is set   
class new_check_t   
{   
public:   
    new_check_t() {}   
    ~new_check_t()   
    {   
        if (new_autocheck_flag)   
        {   
            // Check for leakage.   
            // If any leaks are found, set new_verbose_flag so that any   
            // delete operations in the destruction of global/static   
            // objects will display information to compensate for   
            // possible false leakage reports.   
            if (check_leaks())   
                new_verbose_flag = true;   
        }   
    }   
};   
 new_check_t new_check_object;  
 #pragma pop_macro("new")
#endif