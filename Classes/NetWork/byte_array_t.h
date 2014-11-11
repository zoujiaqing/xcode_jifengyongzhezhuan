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
#include <vector>
#include <string.h>
#include <stdlib.h>

//by benyang
#ifdef WIN32
#	include <winsock2.h>
#	include "support/data_support/uthash.h"
#   include <stdint.h>
#else
#	include <arpa/inet.h>
#endif

#define GEN_READ_SINGLE_P_VALUE_FUNC( value_type  ) \
	bool read(value_type *p_value ){\
	if ( !this->m_is_read_only ) return false;\
	if(this->m_postion+sizeof(value_type)<=this->m_size) {\
	(*p_value)=*(value_type*)(this->m_buf+this->m_postion );\
	if(this->is_bigendian){value_type##_swap((*p_value)); }\
	this->m_postion+=sizeof(value_type);\
	return true;			\
	}else{\
	return false; \
	}\
		}

#define GEN_WRITE_SINGLE_P_VALUE_FUNC( value_type  ) \
	bool write(const  value_type * p_value){\
	if ( this->m_is_read_only ) return false;\
	if (!this->reset_size(sizeof(value_type) )) return false;\
	*(value_type *)(this->m_buf+this->m_postion )= *p_value;\
	if (this->is_bigendian ){\
	value_type##_swap( *(value_type *)(this->m_buf+this->m_postion ) ); \
	}\
	this->m_postion+= sizeof(value_type) ;\
	return true;\
		}\





#define GEN_READ_SINGLE_VALUE_FUNC( func ,value_type  ) \
	bool func (value_type &value ){\
	if ( !this->m_is_read_only ) return false;\
	if(this->m_postion+sizeof(value_type)<=this->m_size) {\
	value=*(value_type*)(this->m_buf+this->m_postion );\
	if(this->is_bigendian){value_type##_swap(value); }\
	this->m_postion+=sizeof(value_type);\
	return true;			\
	}else{\
	return false; \
	}\
		}

#define GEN_WRITE_SINGLE_VALUE_FUNC( func ,value_type  ) \
	bool func(const  value_type value){\
	if ( this->m_is_read_only ) return false;\
	if (!this->reset_size(sizeof(value) )) return false;\
	*(value_type *)(this->m_buf+this->m_postion )= value;\
	if (this->is_bigendian ){\
	value_type##_swap( *(value_type *)(this->m_buf+this->m_postion ) ); \
	}\
	this->m_postion+= sizeof(value) ;\
	return true;\
		}\




inline void uint32_t_swap(uint32_t &value ){
	value=ntohl(value );
}
inline void int32_t_swap(int32_t &value ){
	value=ntohl(value );
}
inline void uint16_t_swap(uint16_t &value ){
	value=ntohs(value );
}
inline void int16_t_swap(int16_t &value ){
	value=ntohs(value );
}
inline void uint8_t_swap(uint8_t &value ){
}
inline void int8_t_swap(int8_t &value ){
}

inline void uint64_t_swap(uint64_t &value ){
	value=ntohl(value>>32) +(((uint64_t )ntohl( value&0xFFFFFFFF))<<32);
}
inline void int64_t_swap(int64_t &value ){
	value=ntohl(value>>32) +(((uint64_t )ntohl( value&0xFFFFFFFF))<<32);
}

inline void char_swap(char &value ){
}

inline void double_swap(double &value ){
}

class  byte_array_t {
	char * m_buf;
	uint32_t m_size  ;
	bool 	m_is_read_only; 
	uint32_t m_postion;
	bool   is_bigendian;

public:
	inline const char * get_buf( ){ return this->m_buf; }
	inline uint32_t get_postion( ){ return this->m_postion; }
	inline uint32_t get_bufsize( ){ return this->m_size; }
	inline void init_postion( ){ this->m_postion=0; }
	inline void set_is_bigendian( bool value ){ this->is_bigendian=value; }
	inline bool get_is_read_only(){ return this->m_is_read_only; }

	byte_array_t(const char * buf,int size ){
		this->m_is_read_only=false;
		this->m_buf=NULL;
		this->is_bigendian=false;
		this->init_read_mode(buf,size );
	}


	void init_read_mode(const char * buf,int size ){
		if(!this->m_is_read_only){
			free(this->m_buf);
		}
		this->m_buf=(char*)buf;
		this->m_size=size;
		this->m_postion=0;
		this->m_is_read_only=true;
	}

	byte_array_t(uint32_t malloc_size=32 ){
		this->m_buf=(char*)malloc(malloc_size );
		this->m_size=malloc_size;
		this->m_postion=0;
		this->m_is_read_only=false;
		this->is_bigendian=false;
	}

	GEN_READ_SINGLE_P_VALUE_FUNC(uint32_t )
		GEN_READ_SINGLE_P_VALUE_FUNC(uint16_t )
		GEN_READ_SINGLE_P_VALUE_FUNC(uint8_t )

		GEN_WRITE_SINGLE_P_VALUE_FUNC(uint32_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(uint16_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(uint8_t )

		GEN_READ_SINGLE_P_VALUE_FUNC(int32_t )
		GEN_READ_SINGLE_P_VALUE_FUNC(int16_t )
		GEN_READ_SINGLE_P_VALUE_FUNC(int8_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(int32_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(int16_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(int8_t )

		GEN_WRITE_SINGLE_P_VALUE_FUNC(int64_t )
		GEN_WRITE_SINGLE_P_VALUE_FUNC(uint64_t )

		GEN_READ_SINGLE_P_VALUE_FUNC(int64_t )
		GEN_READ_SINGLE_P_VALUE_FUNC(uint64_t )

		GEN_READ_SINGLE_P_VALUE_FUNC(double)
		GEN_WRITE_SINGLE_P_VALUE_FUNC(double)


	    GEN_READ_SINGLE_VALUE_FUNC(read_uint32,uint32_t )
		GEN_READ_SINGLE_VALUE_FUNC(read_uint16,uint16_t )
		GEN_READ_SINGLE_VALUE_FUNC(read_uint8,uint8_t )

		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint32,uint32_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint16,uint16_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint8,uint8_t )

		GEN_READ_SINGLE_VALUE_FUNC(read_int32,int32_t )
		GEN_READ_SINGLE_VALUE_FUNC(read_int16,int16_t )
		GEN_READ_SINGLE_VALUE_FUNC(read_int8,int8_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int32,int32_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int16,int16_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_int8,int8_t )

		GEN_WRITE_SINGLE_VALUE_FUNC(write_int64,int64_t )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_uint64,uint64_t )

		GEN_READ_SINGLE_VALUE_FUNC(read_int64,int64_t )
		GEN_READ_SINGLE_VALUE_FUNC(read_uint64,uint64_t )


		GEN_READ_SINGLE_VALUE_FUNC(read_char,char )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_char,char )

		GEN_READ_SINGLE_VALUE_FUNC(read_double ,double )
		GEN_WRITE_SINGLE_VALUE_FUNC(write_double,double )




		bool read_buf(char*  buf, uint32_t size ){
			if(this->m_postion+size<=this->m_size) {
				memcpy(buf,this->m_buf+this->m_postion,size);
				this->m_postion+= size;
				return true;			
			}else{
				return false; 
			}
	}

	bool reset_size(uint32_t add_size ){

		if (this->m_is_read_only) return false;
		if(this->m_postion+add_size>this->m_size) {
			this->m_buf=(char*)realloc(this->m_buf,(this->m_postion+add_size));
			if (!this->m_buf) return false;
			this->m_size=(this->m_postion+add_size);
		}
		return true;
	}

	bool write_buf(const char*  buf, uint32_t size ){
		if (!this->reset_size(size)) return false;
		memcpy(this->m_buf+this->m_postion,buf,size);
		this->m_postion+= size;
		return true;
	}



	bool is_end( ){ return this->m_postion==this->m_size; }
	virtual ~byte_array_t(){
		if( ! this->m_is_read_only ){
			free(this->m_buf);
		}
	}

};