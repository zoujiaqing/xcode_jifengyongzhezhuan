#ifndef _iSeer_ResourcePlistDictionary_h_
#define _iSeer_ResourcePlistDictionary_h_

#include "cocos2d.h"
#include "ResourceBase.h"

using namespace std;
USING_NS_CC;

class ResourcePlistDictionary : public ResourceBase
{
private:
	CCDictionary *	m_pData;

protected:
	ResourcePlistDictionary();

public:
	~ResourcePlistDictionary();

	static ResourcePlistDictionary *				resourceWithFile(const char *filePath);
	virtual void									releaseResource();

	virtual CCDictionary *	getData();

	virtual void	setData(CCDictionary *data);
};

#endif