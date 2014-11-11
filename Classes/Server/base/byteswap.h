 /* Macros to swap the order of bytes in integer values.
    Copyright (C) 1997, 1998, 2000, 2002, 2003, 2007
    Free Software Foundation, Inc.
    This file is part of the GNU C Library.
 
    The GNU C Library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    The GNU C Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with the GNU C Library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA.  */

 #ifndef _BITS_BYTESWAP_H
 #define _BITS_BYTESWAP_H 1
 
 #include "wordsize.h"


 /* Swap bytes in 16 bit value.  */
 #define __bswap_constant_16(x) \
      ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
 

 /* This is better than nothing.  */
 # define __bswap_16(__x) \
	__bswap_constant_16 (__x)
 
 
 /* Swap bytes in 32 bit value.  */
 #define __bswap_constant_32(x) \
      ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |               \
       (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
 

 # define __bswap_32(__x) \
	__bswap_constant_32 (__x)

 
 #endif /* _BITS_BYTESWAP_H */