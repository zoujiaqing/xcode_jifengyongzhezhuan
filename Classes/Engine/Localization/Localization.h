#pragma once

//enum LANGUAGE_TYPE
//{
//	kType_Language_SimpleChinese = 0,
//	kType_Language_Chinese,
//	kType_Language_English,
//	kType_Language_Korean,
//	kType_Language_Japanese,
//	kType_Language_UnKnow
//};

class Localizatioin
{
public :	
	virtual ~Localizatioin();

	static const char* getLocalization(const char* szFlag);	
private:
	Localizatioin();
	//static void CheckLanguageEnv();

	//static LANGUAGE_TYPE m_languageType;
	//static bool m_bCheckLanguageEnv;
};