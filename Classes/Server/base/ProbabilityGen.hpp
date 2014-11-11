/**
 * =====================================================================================
 *      @file  ProbabilityGen.hpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  Aug 14, 2011
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  TaoMee.Inc, ShangHai.
 *   Copyright  Copyright (c) 2011, TaoMee.Inc, ShangHai.
 *
 *     @author  jacke (), jacke@taomee.com
 * This source code is wrote for TaoMee,Inc. ShangHai CN.
 * =====================================================================================
 */

#ifndef PROBABILITYGEN_HPP_
#define PROBABILITYGEN_HPP_

#include <map>
#include <vector>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <exception>

namespace base {

class GetOneException : public std::exception {
public:
	const char* what()const throw()   
   	{  
		return "ERROR! NO ITEM IN GEN.\n";  
   	}   
};

template<typename ItemType>
class ProbabilityGen {
public:
	ProbabilityGen() :
		m_sum_probability(0) {
	}
	int push(uint32_t probability, ItemType item);
	//	int pop(ItemType item);
	virtual ItemType get_one(bool exclusion = true);

	void clear();

	bool empty() const;

	uint32_t size() const;

	bool exist(ItemType) const;

	bool operator ==(ProbabilityGen<ItemType>&);

	ProbabilityGen<ItemType>& operator+=(ProbabilityGen<ItemType>&);

protected:
	typedef std::pair<uint32_t, ItemType> ItemPro;

	typedef std::vector<ItemPro> ItemPros;

	ItemPros m_probability_list;

	uint32_t m_sum_probability;

	ItemType m_last_get_item;

private:
	static int just_for_rand();
};

template<typename ItemType>
int ProbabilityGen<ItemType>::push(uint32_t probability, ItemType item) {
/*	size_t size = m_probability_list.size();
	for (size_t i = 0; i < size; i++) {
		if (m_probability_list[i].second == item) {
			m_probability_list[i].first += probability;
			m_sum_probability += probability;
			return 0;
		}
	}*/
	m_sum_probability += probability;
	m_probability_list.push_back(ItemPro(probability, item));
	return 0;
}

//template<typename ItemType>
//int ProbabilityGen<ItemType>::pop(ItemType item) {
//	ItemPros::iterator itr = m_probability_list.begin();
//	for (; itr != m_probability_list.end(); itr++) {
//		if (itr->second() == item) {
//			m_sum_probability -= itr->first();
//			m_probability_list.erase(itr);
//			return 0;
//		}
//	}
//	return 1;
//}

template<typename ItemType>
ItemType ProbabilityGen<ItemType>::get_one(bool exclusion) {
	static int temp = just_for_rand(); 
	if (!m_sum_probability*temp) {
		throw GetOneException();
	}
	int seed = rand() % m_sum_probability;
	int sum = 0;
	size_t size = m_probability_list.size();
	for (size_t i = 0; i < size; i++) {
		sum += m_probability_list[i].first;
		if (seed < sum) {
			if (exclusion) {
				m_sum_probability -= m_probability_list[i].first;
				m_probability_list[i].first = 0;
			}
			return (m_probability_list[i].second);
		}
	}
	throw GetOneException();
}

template<typename ItemType>
void ProbabilityGen<ItemType>::clear() {
	m_sum_probability = 0;
	m_probability_list.clear();	
}

template<typename ItemType>
bool ProbabilityGen<ItemType>::empty() const {
	if (!m_probability_list.size()) {
		return true;
	} else {
		size_t size = m_probability_list.size();
		for (size_t i = 0; i < size; i++) {
			if (m_probability_list[i].first) {
				return false;
			}
		}
	}
	return true;
}

template<typename ItemType>
uint32_t ProbabilityGen<ItemType>::size() const {
	if (!m_probability_list.size()) {
		return 0;
	} else {
		uint32_t num = 0;
		size_t size = m_probability_list.size();
		for (size_t i = 0; i < size; i++) {
			if (m_probability_list[i].first) {
				num++;
			}
		}
		return num;
	}
}

template<typename ItemType>
bool ProbabilityGen<ItemType>::exist(ItemType item) const {
	size_t size = m_probability_list.size();
	for (size_t i = 0; i < size; i++) {
			if(m_probability_list[i].first && m_probability_list[i].second == item){
				return true;
			}
	}
	return false;
}

template<typename ItemType>
bool ProbabilityGen<ItemType>::operator ==(ProbabilityGen<ItemType>& Pro) {
	if(Pro.m_probability_list == this->m_probability_list){
		return true;
	}
	return false;
}
	
template<typename ItemType>
ProbabilityGen<ItemType>& ProbabilityGen<ItemType>::operator += (ProbabilityGen<ItemType>& Pro) {
	m_probability_list.insert(m_probability_list.end(), Pro.m_probability_list.begin(), Pro.m_probability_list.end());
	m_sum_probability += Pro.m_sum_probability;
	return *this;	
}

template<typename ItemType>
int ProbabilityGen<ItemType>::just_for_rand() {
	srand(time(NULL));
	return 1;
}


template<typename ItemType>
class AverageGen: public ProbabilityGen<ItemType> {
public:
	int push(ItemType item) {
		return ProbabilityGen<ItemType>::push(1, item);
	}
	;
};

inline int ranged_random(int a, int b)
{
	int max = a>b?a:b;
	int min = a<=b?a:b;
    // generates ranged random number
    return (rand() % (max - min + 1)) + min;
}

};

#endif /* PROBABILITYGEN_HPP_ */
