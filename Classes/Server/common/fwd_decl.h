#ifndef RU_FORWARD_DECLARATION_H_
#define RU_FORWARD_DECLARATION_H_

#include "../base/TimerEvent.hpp"
#include "../base/Singleton.hpp"
//#define SELF_DEF_DAILY_START_TIME   (7200)     // Զÿʼʱ( ÿ2:00 2*3600)
#define SELF_DEF_DAILY_START_TIME   (0)     // Զÿʼʱ( ÿ00:00 )


#define FOREACH(container,it) \
    for(__typeof__((container).begin()) it = (container).begin();it!=(container).end();++it)
	
#define RFOREACH(container,it) \
    for(__typeof__((container).rbegin()) it = (container).rbegin();it!=(container).rend();++it)
	

class MsgHandlerMgr;  // Ϣ���
class World;          // 
class Map;            // ͼ
class BattleField;    // սͼ
class Player;         // 
class Globle: public Singleton<Globle>
{
public:
    Globle(){
	}
public:
	/* *
	 * @brief õԶʱtmĽʱ
	 * */
	uint32_t get_self_def_end_of_day(uint32_t tm) {
		if (base::Time(tm).seconds() < base::Time(tm).start_of_day().seconds() + SELF_DEF_DAILY_START_TIME) {
			return base::Time(tm).start_of_day().seconds() + SELF_DEF_DAILY_START_TIME;
		} else {
			return base::Time(tm).end_of_day().seconds() + SELF_DEF_DAILY_START_TIME;
		}
	}
	/* *
	 * @brief õԶʱtm1tm2
	 * */
	uint32_t get_self_def_diff_days(uint32_t tm1, uint32_t tm2) {
		uint32_t end_tm1 = get_self_def_end_of_day(tm1);
		uint32_t end_tm2 = get_self_def_end_of_day(tm2);
		// NOTI תһʱ,϶86400
		return abs(static_cast<int32_t>(end_tm1 - end_tm2)) / 86400;
	}
	/* *
	 * @brief õtmСʱڵĵһ
	 * */
	uint32_t get_start_of_hour(uint32_t tm) {
		return tm - tm % 3600;
	}
#ifndef FRONT
	/* *
	 * @brief õtmܼ 
	 * */
	uint32_t get_wday(uint32_t sec) {
		time_t sec_temp = sec;
		struct tm sec_tm;
		localtime_r(&sec_temp, &sec_tm);
		if (sec_tm.tm_wday == 0) {
			return 7;
		} else {
			return sec_tm.tm_wday;
		}
	}
	/* *
	 * @brief õsvcʱƫ
	 * */
	uint32_t get_time_zone() {
		/*
		struct timezone{
			int tz_minuteswest;	// ʱ����ʱ
			int tz_dsttime;		// DST ʱʽ
		} 
		*/
		static uint32_t time_zone = 0;
		if (!time_zone) {
			struct timeval tv;
			struct timezone tz;
			gettimeofday (&tv, &tz);
			if (tz.tz_minuteswest < 0) {
				// 
				time_zone = 1;
				time_zone <<= 16;
				time_zone |= (-1)*tz.tz_minuteswest;
			} else {
				// 
				time_zone = 2;
				time_zone <<= 16;
				time_zone |= tz.tz_minuteswest;
			}
		}
		return time_zone;
	}
#endif
public:
	base::TimerManager Timer;
};

#define sGloble Globle::get_singleton()

#endif // RU_FORWARD_DECLARATION_H_
