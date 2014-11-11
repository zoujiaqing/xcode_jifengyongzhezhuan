#ifndef _iSeer_ResourceBase_h_
#define _iSeer_ResourceBase_h_

#include <string>

using namespace std;

typedef enum
{
	ResBase = 0,
	ResTexture,
	ResPlistArray,
	ResPlistDictionary
} ResourceType;

class ResourceBase
{
protected:
	string							m_strResourceName;
	int								m_iSize;
	int								m_iType;

protected:
	ResourceBase()
		:m_strResourceName(""),
		m_iSize(0),
		m_iType(ResBase)
	{
	}
		
public:
	~ResourceBase() {}

	virtual void					releaseResource() {}

	virtual string					getName(){return m_strResourceName;}

	virtual int						getSize(){return m_iSize;}
};

#endif