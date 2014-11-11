#pragma once

#include "Singleton_t.h"
#include "map"
#include "vector"
#include "string"

/*
*   »»×°ÏµÍ³
*   
*
*/

class DressUpManager : public TSingleton<DressUpManager>
{
public:
	DressUpManager();
	virtual ~DressUpManager();

	bool LoadXML();

	static bool SplitStringWithDot(std::string str,std::vector<std::string> &vecOut);
	bool GetHeroEquipByID(unsigned int heroLevel,std::vector<std::string> &);
	bool GetAdaptedImageName(std::string &srcImageName,std::vector<std::string> &sourceVecNames,std::string &outImageName);
private:
	bool mIsLoadData;
	std::map<unsigned int,std::vector<std::string> > mIDAndEquipNames;
	std::vector<std::string> mVecAdaptedString;
};