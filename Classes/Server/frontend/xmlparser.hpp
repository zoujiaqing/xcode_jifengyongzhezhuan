/**
 *============================================================
 *  @file      xmlparser.hpp
 *  @brief    utilities for parsing xml files\n
 *            使用方法：请参阅conf_parser/xml.cpp和xml.xml\n
 *            g++ xml.cpp -I/usr/include/libxml2 -lxml2 -ltaomee++
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *  copyright:  TaoMee, Inc. ShangHai CN. All rights reserved.
 *
 *============================================================
 */

#ifndef LIBTAOMEEPP_XMLPARSER_HPP_
#define LIBTAOMEEPP_XMLPARSER_HPP_

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include "GameResourceManager.h"
#include "CCFileUtils.h"

extern "C" {
#include <libxml/tree.h>
}
using namespace std;
namespace taomee {

typedef std::ios_base& (&manip_t)(std::ios_base&);

/**
 * @class XmlParseError
 * @brief 解析xml配置文件出错时抛出的异常。继承自std: :logic_error
 */
class XmlParseError : public std::logic_error {
public:
	explicit XmlParseError(const std::string& errmsg) : std::logic_error(errmsg)
		{ }
};

/**
 * @brief 把某个xml属性的值读取到数组中
 * @param arr xml的属性值将被读取到arr中
 * @param cur 当前xml节点指针
 * @param prop xml属性名
 * @param manip 以什么形式解释xml属性值，默认是dec十进制，还可用oct八进制和hex十六进制
 * @throws XmlParseError
 * @return 读取到数组arr中的值的个数
 * @see get_xml_prop_arr_def
 */
template <typename T1, size_t len>
size_t get_xml_prop_arr(T1 (&arr)[len], xmlNodePtr cur, const void* prop, manip_t manip = std::dec)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		throw XmlParseError(std::string("failed to parse prop '")
								+ reinterpret_cast<const char*>(prop) + "'");
	} else {
		size_t i = 0;
		std::istringstream iss(reinterpret_cast<const char*>(str));

        if (*((char*)str) == 0) {//只有一个纯粹的双引号 没有默认值抛出异常
            xmlFree(str);
            throw XmlParseError(std::string("failed to parse prop '")
								+ reinterpret_cast<const char*>(prop) + "'");
        } else {
            while ((i != len) && (iss >> manip >> arr[i])) {
                ++i;
            }
		    xmlFree(str);
		    return i;
        }
	}
}

/**
 * @brief 把某个xml属性的值读取到字符数组中
 * @param val xml的属性值将被读取到val中
 * @param cur 当前xml节点指针
 * @param prop xml属性名
 * @throws XmlParseError
 * @see get_xml_prop_raw_str_def
 */
template <size_t len>
void get_xml_prop_raw_str(char (&val)[len], xmlNodePtr cur, const void* prop)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		throw XmlParseError(std::string("failed to parse prop '")
								+ reinterpret_cast<const char*>(prop) + "'");
	} else {
		strncpy(val, reinterpret_cast<char*>(str), len - 1);
		val[len - 1] = '\0';
		xmlFree(str);
	}
}

/**
 * @brief 把某个xml属性的值读取出来
 * @param val xml的属性值将被读取到val中
 * @param cur 当前xml节点指针
 * @param prop xml属性名
 * @param manip 以什么形式解释xml属性值，默认是dec十进制，还可用oct八进制和hex十六进制
 * @throws XmlParseError
 * @note 注意！不要用这个函数来读取xml属性值到字符数组中，否则有越界的危险！如果需要读取字符串，\n
 *       可以用这个函数把字符串读到string对象里，或者用get_xml_prop_raw_str把字符串读到字符数组里。
 * @see get_xml_prop_raw_str, get_xml_prop_def
 */
template <typename T1>
void get_xml_prop(T1& val, xmlNodePtr cur, const void* prop, manip_t manip = std::dec)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		throw XmlParseError(std::string("failed to parse prop '")
								+ reinterpret_cast<const char*>(prop) + "'");
	} else {
		std::istringstream iss(reinterpret_cast<const char*>(str));
		iss >> manip >> val;
		xmlFree(str);
		if (!iss) {
			throw XmlParseError(std::string("failed to parse prop '")
									+ reinterpret_cast<const char*>(prop) + "'");
		}
	}
}

/**
 * @brief 同get_xml_prop_arr，但如果cur为空或者prop属性不存在的话，会把默认值def赋给arr
 * @return 读取到数组arr中的值的个数，如果prop不存在，则返回0
 * @see get_xml_prop_arr
 */
template <typename T1, typename T2, size_t len>
size_t get_xml_prop_arr_def(T1 (&arr)[len], xmlNodePtr cur, const void* prop, const T2& def, manip_t manip = std::dec)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		for (size_t i = 0; i != len; ++i) {
			arr[i] = def;
		}
		return 0;
	} else {
		size_t i = 0;
        if (*((char*)str) == 0) {//只有一个纯粹的双引号 没有值赋默认值
            for (size_t i = 0; i != len; ++i) {
                arr[i] = def;
            }
            xmlFree(str);
            return 0;
        } else {
            std::istringstream iss(reinterpret_cast<const char*>(str));
            while ((i != len) && (iss >> manip >> arr[i])) {
                ++i;
            }
            xmlFree(str);
            return i;
        }
	}
}

/**
 * @brief 同get_xml_prop_raw_str，但如果cur为空或者prop属性不存在的话，会把默认值def赋给val
 * @see get_xml_prop_raw_str
 */
template <size_t len>
void get_xml_prop_raw_str_def(char (&val)[len], xmlNodePtr cur, const void* prop, const char* def)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		strncpy(val, def, len - 1);
		val[len - 1] = '\0';
	} else {
		strncpy(val, reinterpret_cast<char*>(str), len - 1);
		val[len - 1] = '\0';
		xmlFree(str);
	}
}

/**
 * @brief 同get_xml_prop，但如果cur为空或者prop属性不存在的话，会把默认值def赋给val
 * @see get_xml_prop
 */
template <typename T1, typename T2>
void get_xml_prop_def(T1& val, xmlNodePtr cur, const void* prop, const T2& def, manip_t manip = std::dec)
{
	xmlChar* str;
	if (!cur || !(str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
		val = def;
	} else {
		std::istringstream iss(reinterpret_cast<const char*>(str));
		if (*((char*)str) == 0) {
            val = def;
        } else {
            iss >> manip >> val;
        }
		xmlFree(str);
	}
}

/**
 * @brief 解析xml文件
 * @param file 待解析的xml文件
 * @param parser 用于解析xml文件的函数
 * @throws XmlParseError
 */
inline void load_xmlconf(const char* file, int (*parser)(xmlNodePtr cur_node)){

	CCLOG("load_xmlconf  %s", file);
	int err = -1;

	std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(file);

	unsigned long	_size;
	char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "r", &_size);

	CCAssert(_pFileContent, "load_xmlconf xml read failed");

	//xmlDocPtr doc = xmlParseMemory(_pFileContent, (int)_size);
	//xmlDocPtr doc = xmlReadFile(file, 0, XML_PARSE_NOBLANKS);
	xmlDocPtr doc = xmlReadMemory(_pFileContent, (int)_size, fullFilePath.c_str(), 0, XML_PARSE_NOBLANKS);
	if (!doc) {

		throw XmlParseError(string("failed to load file '") + file + "'");

	}	

	xmlNodePtr cur = xmlDocGetRootElement(doc);
	if (!cur) {

		xmlFreeDoc(doc);
		throw XmlParseError(string("xmlDocGetRootElement error when loading file '") + file + "'");
			
	}

	err = parser(cur);
	xmlFreeDoc(doc);

	if (err) {

			throw XmlParseError(string("failed to parse file '") + file + "'");
				
	}
}

}

#endif // LIBTAOMEEPP_XMLPARSER_HPP_

