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
#ifndef ENCRY_HPP
#define ENCRY_HPP
#include <string.h>

#define encry_buf_len	8192
inline void rand_list(uint16_t seed, int len, char* res) 
{
	uint16_t base = 65280;// 2^16-2^8

	seed = seed % base;
	int loop = 0;
	int i = 0;
	uint32_t x = seed, sum = 0;
	while (1) {
		sum = x*x + x + 1;
		if (loop + 4 >= len) {
			for ( ; i < len - loop; i++)
				res[loop + i] = (sum >> (3-i)) & 0xFF;

			break;
		}
		*(uint32_t*)&res[loop] = sum;
		x = sum % base;
		loop += 4;
	}
}

inline uint32_t calc_sum(uint16_t seed, const char* pkg, int len)
{
	static char res[encry_buf_len];
	if (len == 0) return 0;

	char* pres = res;
	int  size = encry_buf_len;
	if (len > encry_buf_len) {
		size = (len / 128 + 1) * 128;
		pres = (char*)malloc(size);
	}
	memset(pres, 0, size);
	rand_list(seed, len, pres);
	uint32_t sum = 0;
	int loop = 0;
	for ( ; loop < len; loop++) {
		sum += (pkg[loop] & pres[loop]);
	}
	if (size > encry_buf_len)
		free(pres);
	return sum;
}

#endif
