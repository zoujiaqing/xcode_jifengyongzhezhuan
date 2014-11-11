//
//  EncodeValue.h
//  ahero
//
//  Created by jacke on 10/9/13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_EncodeValue_h
#define iSeer_EncodeValue_h

#include <string>
#include <vector>
#include<stdlib.h>

#define MAX_LENTH 32

#define ENCODE_BINARY_SINCE_OP(T, OP)				\
T operator OP (int) {								\
	T tmp = *this;									\
	typename T::BaseType left ;						\
	decode(left);									\
	left OP;										\
    encode(left);									\
	return tmp;										\
};													\
T& operator OP () {									\
	T tmp = *this;									\
	typename T::BaseType left ;						\
	decode(left);									\
	left OP;										\
    encode(left);									\
	return *this;									\
};	
													
#define ENCODE_BINARY_VOID_OP(T, OP)				\
void operator OP (T value) {						\
	typename T::BaseType left;						\
	decode(left);									\
    typename T::BaseType right;						\
	value.decode(right);							\
    left OP right;									\
	encode(left);									\
	return ;										\
};													\
template<class ValueType> 							\
void operator OP ( const ValueType& orig) {			\
	typename T::BaseType value;						\
	decode(value);									\
    value OP orig;									\
	encode(value);									\
	return ;										\
}; 

#define ENCODE_UNARY_T_OP(T, OP)					\
T& operator OP () {									\
	typename T::BaseType value;						\
	decode(value);									\
    value OP;										\
	encode(value);									\
	return *this;									\
};													\

#define ENCODE_NUM_TYPE(T)											\
	void encode(T value) {											\
		unsigned char* value_ext = (unsigned char*) (&value); 		\
		size_t i = sizeof(T);										\
		if(mFlag + i>=MAX_LENTH) 	{									\
			mFlag = 0;												\
		}															\
		mCheck = calc_crc8(value_ext , i);							\
		for (size_t n=0; n<i ; n++) {								\
			unsigned char part = *value_ext; 						\
			part = encodeChar(part);								\
			mData[mFlag+n] = part;									\
			value_ext++;											\
		};															\
		mFlag += i;													\
	};																\
																	\
	void decode(T& value) const {									\
		if (!mFlag) return ;										\
		std::vector<unsigned char> parts;							\
		size_t i = sizeof(T);										\
		for (size_t n=0; n<i ; n++) {								\
			const unsigned char* value_ext = (&mData[mFlag - sizeof(T) + n]);\
			unsigned char part = *value_ext; 						\
			part = decodeChar(part);								\
			parts.push_back(part);									\
		};															\
		value = *((T*)(&parts[0]));									\
		if(mCheck != calc_crc8(&parts[0], sizeof(T))) {				\
			int *p = NULL;											\
			*p = 1;													\
			throw EncodeCheckException(); 							\
		}															\
	};																\

#include <exception>

class EncodeCheckException : public std::exception {
public:
    const char* what()const throw()
    {
        return "ERROR! NO CHECK EXCEPTION.\n";
    }
};



template<typename EncodeType>
class EncodeBase {
public:
	EncodeBase();

    unsigned char encodeChar(unsigned char orig);
    unsigned char decodeChar(unsigned char orig)const;

	template<typename ValueType> 
	void encode(ValueType orig);

	template<typename ValueType> 
	EncodeType decode(ValueType orig);

	ENCODE_NUM_TYPE(int)
	ENCODE_NUM_TYPE(unsigned int)							
	ENCODE_NUM_TYPE(short)
	ENCODE_NUM_TYPE(unsigned short)
	ENCODE_NUM_TYPE(char)
	ENCODE_NUM_TYPE(unsigned char)
	ENCODE_NUM_TYPE(long)
	ENCODE_NUM_TYPE(unsigned long)
	ENCODE_NUM_TYPE(long long)
	ENCODE_NUM_TYPE(unsigned long long)
	ENCODE_NUM_TYPE(float)
	ENCODE_NUM_TYPE(double)

	unsigned char calc_crc8(const unsigned char* body, uint32_t body_len) const {
		unsigned char crc8_val = 0;
		for (unsigned char i = 0; i < body_len; i++) {
			crc8_val ^= (body[i] & 0xff);
		}
	/*85 magic number, but it has means*/
		return crc8_val ^ 85;
    }

private:
	size_t mFlag;
	unsigned char mData[MAX_LENTH];
	unsigned char mCheck;
};

template<typename EncodeType, typename EncodeStragy = EncodeBase<EncodeType> >														
class EncodeValue
{
typedef EncodeType BaseType;
public:
   
    EncodeValue();
    EncodeValue(const EncodeType& orig);
   
	template<class ValueType> 
    const EncodeValue& operator= (const ValueType& orig);
	const EncodeValue& operator= (const EncodeValue& value);
  
	ENCODE_BINARY_VOID_OP(EncodeValue, +=)
	ENCODE_BINARY_VOID_OP(EncodeValue, -=)
	ENCODE_BINARY_VOID_OP(EncodeValue, *=)
	ENCODE_BINARY_VOID_OP(EncodeValue, /=)
	ENCODE_BINARY_VOID_OP(EncodeValue, %=)
	ENCODE_BINARY_VOID_OP(EncodeValue, &=)
	ENCODE_BINARY_VOID_OP(EncodeValue, |=)
	ENCODE_BINARY_VOID_OP(EncodeValue, ^=)
	ENCODE_BINARY_VOID_OP(EncodeValue, <<=)
	ENCODE_BINARY_VOID_OP(EncodeValue, >>=)
	
	ENCODE_BINARY_SINCE_OP(EncodeValue, ++)
	ENCODE_BINARY_SINCE_OP(EncodeValue, --)
 
	template<class StreamType>
	friend StreamType& operator>>(StreamType& stream, EncodeValue<EncodeType, EncodeStragy>& value)	{
		EncodeType orig;
		stream >> orig;
		value.encode(orig);
		return stream;
	};
 
    operator EncodeType();
	operator EncodeType() const;

    EncodeType getData() const;
   
	EncodeType operator()();

	EncodeType operator()() const;
 
private:
	template<typename ValueType>
	void encode(ValueType orig) {return m_stragy.encode(orig);};
	
	template<typename ValueType>
	void decode(ValueType& orig) const {return m_stragy.decode(orig);};

private:
	EncodeStragy		m_stragy;
};

template<typename EncodeType, typename EncodeStragy>
template<class ValueType> 
const EncodeValue<EncodeType, EncodeStragy>& EncodeValue<EncodeType, EncodeStragy>::operator= (const ValueType& orig) {
	EncodeType value = orig;
	encode(value);
	return *this;	
}

template<typename EncodeType, typename EncodeStragy>
EncodeValue<EncodeType, EncodeStragy>::EncodeValue()
{
}

template<typename EncodeType, typename EncodeStragy>
EncodeValue<EncodeType, EncodeStragy>::EncodeValue(const EncodeType& orig)
{
	encode(orig);    
}

template<typename EncodeType, typename EncodeStragy>
const EncodeValue<EncodeType, EncodeStragy>& EncodeValue<EncodeType, EncodeStragy>::operator=(const EncodeValue<EncodeType, EncodeStragy>& value)
{
	EncodeType type_value;
	value.decode(type_value);
	encode(type_value);
	return *this;
}

template<typename EncodeType, typename EncodeStragy>
EncodeValue<EncodeType, EncodeStragy>::operator EncodeType()
{
	EncodeType value;
	decode(value);
	return value;
}

template<typename EncodeType, typename EncodeStragy>
EncodeValue<EncodeType, EncodeStragy>::operator EncodeType()const
{
	EncodeType value;
	decode(value);
	return value;
}

template<typename EncodeType, typename EncodeStragy>
EncodeType EncodeValue<EncodeType, EncodeStragy>::operator() () const
{
	EncodeType value;
	decode(value);	 
	return value;
}

template<typename EncodeType, typename EncodeStragy>
EncodeType EncodeValue<EncodeType, EncodeStragy>::operator() ()
{
	EncodeType value;
	decode(value);	 
	return value;
}
	
template<typename EncodeType>
EncodeBase<EncodeType>::EncodeBase():
	mFlag(0),
	mCheck(0)
{
}

template<typename EncodeType>
unsigned char EncodeBase<EncodeType>::encodeChar(unsigned char orig)
{
	unsigned char byte0 = orig & (0x1 << 0);
	unsigned char byte1 = orig & (0x1 << 1);
	unsigned char byte2 = orig & (0x1 << 2);
	unsigned char byte3 = orig & (0x1 << 3);
	unsigned char byte4 = orig & (0x1 << 4);
	unsigned char byte5 = orig & (0x1 << 5);
	unsigned char byte6 = orig & (0x1 << 6);
	unsigned char byte7 = orig & (0x1 << 7);
	
	unsigned char result =  (byte7 >> 1) | (byte6 << 1) |
							(byte5 >> 1) | (byte4 << 1) |
							(byte3 >> 1) | (byte2 << 1) |
							(byte1 >> 1) | (byte0 << 1);  
	
	
	return result;    
}

template<typename EncodeType>
unsigned char EncodeBase<EncodeType>::decodeChar(unsigned char orig)const
{
	unsigned char byte0 = orig & (0x1 << 0);
	unsigned char byte1 = orig & (0x1 << 1);
	unsigned char byte2 = orig & (0x1 << 2);
	unsigned char byte3 = orig & (0x1 << 3);
	unsigned char byte4 = orig & (0x1 << 4);
	unsigned char byte5 = orig & (0x1 << 5);
	unsigned char byte6 = orig & (0x1 << 6);
	unsigned char byte7 = orig & (0x1 << 7);
	
	unsigned char result =  (byte7 >> 1) | (byte6 << 1) | 
							(byte5 >> 1) | (byte4 << 1) |
							(byte3 >> 1) | (byte2 << 1) |
							(byte1 >> 1) | (byte0 << 1);   
	
	return result;    
}

#endif
