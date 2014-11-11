
#include <vector>
#include "cocos2d.h"
#include "util.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <dirent.h>
#include "NcSDKManager.h"
#endif

using namespace std;
USING_NS_CC;

char intToHexChar(int x) {
	static const char HEX[16] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};
	return HEX[x];
}

int hexCharToInt(char hex) {
	hex = toupper(hex);
	if (isdigit(hex))
		return (hex - '0');
	if (isalpha(hex))
		return (hex - 'A' + 10);
	return 0;
}

string bytesToHexString(const byte *in, size_t size) {
	string str;
	for (size_t i = 0; i < size; ++i) {
		int t = in[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, intToHexChar(a));
		str.append(1, intToHexChar(b));
		if (i != size - 1)
			str.append(1, ' ');
	}
	return str;
}

size_t hexStringToBytes(const string &str, byte *out) {

	vector<string> vec;
	string::size_type currPos = 0, prevPos = 0;
	while ((currPos = str.find(' ', prevPos)) != string::npos) {
		string b(str.substr(prevPos, currPos - prevPos));
		vec.push_back(b);
		prevPos = currPos + 1;
	}
	if (prevPos < str.size()) {
		string b(str.substr(prevPos));
		vec.push_back(b);
	}
	typedef vector<string>::size_type sz_type;
	sz_type size = vec.size();
	for (sz_type i = 0; i < size; ++i) {
		int a = hexCharToInt(vec[i][0]);
		int b = hexCharToInt(vec[i][1]);
		out[i] = a * 16 + b;
	}
	return size;
}
void ShowToast( char* content)
{
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		if (content != NULL)
		{
			CCLog(content);
		}
		//content = "network disconnect,please try again !!!";
      //  content = str;

		JniMethodInfo methodInfo;
		bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
			"com/taomee/adventure/adventure",//需要调用的Java文件
			"showMessageFromNative",//调用的方法名
			"(Ljava/lang/String;)V");//参数
		if(isHave){
			jstring StringArg = methodInfo.env->NewStringUTF(content);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, StringArg);
		}else{
			CCLog("failed to call show toast");
		}
	}
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
    	//this function is not used for network disconnect. disable this
      //  NcSDKManager::NetworkDisconnect(str);
    }
#endif
}
/**
 * check if the app is on forground
 */
bool isAppOnForeground()
{

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
			"com/taomee/adventure/adventure",//需要调用的Java文件
			"isAppOnForeground",//调用的方法名
			"()Z");//参数
		bool ret = false;
		if(isHave){
			ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		}else {
			CCLog("failed find method isAppOnForeground");
		}	
		CCLOG("%s %d, %d",__FUNCTION__, __LINE__, ret?1:0);
		return ret;
#else
	return false;
#endif
}

void openWebUrl(std::string urlstring)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "DownLoadApkFromWeb", "(Ljava/lang/String;)V"))
	{
		jstring url = t.env->NewStringUTF(urlstring.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID,url);
		t.env->DeleteLocalRef(url);
		t.env->DeleteLocalRef(t.classID);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::openWebUrl(urlstring);
#else
		CCLOG("openWebUrl not support for ios ,win32");
#endif

}
