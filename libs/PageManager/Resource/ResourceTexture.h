#ifndef _iSeer_ResourceTexture_h_
#define _iSeer_ResourceTexture_h_

#include "cocos2d.h"
#include "ResourceBase.h"

using namespace std;
USING_NS_CC;

class ResourceTexture : public ResourceBase
{
private:
	CCTexture2D*						m_pData;

protected:
	ResourceTexture();

public:
	~ResourceTexture();

	static ResourceTexture *			resourceWithFile(const char *filePath);
	virtual void						releaseResource();

	virtual CCTexture2D*				getData();

	virtual void						setData(CCTexture2D* data);
};

#endif