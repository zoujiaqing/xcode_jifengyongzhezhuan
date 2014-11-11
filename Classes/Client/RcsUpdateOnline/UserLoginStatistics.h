#ifndef USER_LOGIN_STATISTICS_H
#define USER_LOGIN_STATISTICS_H

class UserLoginStatistics
{
public:
	UserLoginStatistics();
	~UserLoginStatistics();

	static void sendStatInfoToServer(const char* key);
    static void sendLaunchStat(unsigned int  channel,const char* version,const char* platform,
                               const char* equdid,const char* eqtype,const char* mac);
private:
//static	std::string strHtml;
};

#endif