#ifndef _iSeer_ResourcePlistArray_h_
#define _iSeer_ResourcePlistArray_h_

#include "cocos2d.h"
#include "ResourceBase.h"

using namespace std;
USING_NS_CC;

class ResourcePlistArray : public ResourceBase
{
private:
	CCArray *			m_pData;

protected:
	ResourcePlistArray();

public:
	~ResourcePlistArray();

	static ResourcePlistArray *			resourceWithFile(const char *filePath);
	virtual void						releaseResource();

	virtual CCArray *	getData();

	virtual void		setData(CCArray *data);
};

#endif