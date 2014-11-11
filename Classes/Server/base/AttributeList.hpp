/**
 * =====================================================================================
 *      @file  AttributeList.hpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  Nov 13, 2011
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  TaoMee.Inc, ShangHai.
 *   Copyright  Copyright (c) 2011, TaoMee.Inc, ShangHai.
 *
 *     @author  jacke (), jacke@taomee.com
 * This source code is wrote for TaoMee,Inc. ShangHai CN.
 * =====================================================================================
 */

#ifndef ATTRIBUTELIST_HPP_
#define ATTRIBUTELIST_HPP_
#include <typeinfo>
#include <map>
#include <string>
#include <exception>

namespace base {


class AttributeError: std::exception {

};

class Attribute {
public:
	Attribute():
		content(0) {
	}

	template<typename ValueType>
	Attribute(const ValueType & value):
		content(new holder<ValueType> (value)) {
	}
	
	Attribute(const Attribute & other) :
		content(other.content ? other.content->clone():0) {
	}

	~Attribute() {
		delete content;
	}

public:
	Attribute & swap(Attribute & rhs) {
		std::swap(content, rhs.content);
		return *this;
	}

	template<typename ValueType>
	Attribute & operator=(const ValueType & rhs) {
		Attribute(rhs).swap(*this);
		return *this;
	}

	Attribute & operator=(Attribute rhs) {
		rhs.swap(*this);
		return *this;
	}

public:
	bool empty() const {
		return !content;
	}

	const std::type_info & type() const {
		return content? content->type() : typeid(void);
	}

	class placeholder {
	public:
		virtual ~placeholder() {
		}
	public:
		virtual const std::type_info & type() const =0;
		virtual placeholder * clone() const = 0;
	};

	template<typename ValueType>
	class holder: public placeholder {
	public:
		holder(const ValueType & value) :
			held(value) {
		}

		virtual const std::type_info & type() const {
			return typeid(ValueType);
		}

		virtual placeholder * clone() const {
			return new holder<ValueType> (held);
		}
	public:
		ValueType held;
	private:
		holder & operator=(const holder &);
	};

	placeholder* content;
};

typedef std::map<std::string, Attribute> AttributeMap;

class AttributeList {
public:
	AttributeList() {
	}

	void init(std::string key) {
		if(m_object_map.find(key) != m_object_map.end()){
			throw AttributeError();
		}
		else m_object_map[key] = NULL;
	}

	template<class Type> void init(std::string key) {
		if(m_object_map.find(key) != m_object_map.end()){
			throw AttributeError();
		}
		else m_object_map[key] = Type();
	}


	template<class Type> void insert(std::string key, Type value) {
		m_object_map[key] = value;
	}

	template<class Type> Type& get(std::string key) {
		if (m_object_map.find(key) != m_object_map.end() && m_object_map[key].type() != typeid(Type)) {
			throw AttributeError();
		}else if(m_object_map.find(key) == m_object_map.end()){
			m_object_map[key] = Type();
		}
		return (static_cast< Attribute::holder<Type> * >(m_object_map[key].content))->held ;
	}

	template<class Type> void get(std::string key, Type& value) {
		if (m_object_map.find(key) != m_object_map.end() && m_object_map[key].type() != typeid(Type)) {
			throw AttributeError();
		}else if(m_object_map.find(key) == m_object_map.end()){
			m_object_map[key] = Type();
		}
		value = (static_cast< Attribute::holder<Type> * >(m_object_map[key].content))->held ;
		return;
	}

	template<class Type> Type* get_ref(std::string key) {
		if (m_object_map.find(key) != m_object_map.end() && m_object_map[key].type() != typeid(Type)) {
			throw AttributeError();
		} else if (m_object_map.find(key) == m_object_map.end()) {
			m_object_map[key] = Type();
		}
		return &(static_cast< Attribute::holder< Type> * >(m_object_map[key].content))->held ;
	}

	template<class Type> void set(std::string key, Type value) {
		if (m_object_map.find(key) != m_object_map.end() && m_object_map[key].type() != typeid(Type)) {
			throw AttributeError();
		} else {
			m_object_map[key] = value ;
		}
	}

	void del(std::string key) {
		if (m_object_map.find(key) != m_object_map.end()){
			m_object_map.erase(m_object_map.find(key));
		}
	}

	bool exist(std::string key) {
		return m_object_map.find(key) != m_object_map.end();
	}

	template<class Type> bool exist(std::string key) {
		if(m_object_map.find(key) != m_object_map.end() && m_object_map[key].type() == typeid(Type)) {
			return true;
		}
		return false;
	}

private:
	AttributeMap m_object_map;
};

}

#endif /* ATTRIBUTELIST_HPP_ */
