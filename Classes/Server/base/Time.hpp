#ifndef TIME_HPP_
#define TIME_HPP_

#include <CCStdC.h>


namespace base {

class Time {
public:
	Time()
	{	
		update();
	}

	explicit Time(const timeval& tv)
	{
		m_tv = tv;
	}
	
	explicit Time(const long& second, const long& usecond=0)
	{
		m_tv.tv_sec = second;
		m_tv.tv_usec = usecond;
	}

	void ms_timeadd(uint32_t ms)
	{
		m_tv.tv_sec += ms /1000;
		m_tv.tv_usec += (ms % 1000) * 1000;
		if ( m_tv.tv_usec > 999999 ) {
			m_tv.tv_sec += m_tv.tv_usec / 1000000;
			m_tv.tv_usec = m_tv.tv_usec % 1000000;
		}
	}

	void now_ms_timeadd(uint32_t ms)
	{
			// Note: 在副本中引很多的怪之后，切到后台，等待一段时间后，怪物不会攻击主角 《-》修复
		update();
		ms_timeadd(ms);
	}

	void update()
	{
		gettimeofday(&m_tv, 0);
	}
	
	uint32_t u_seconds()
	{
		return m_tv.tv_usec;
	}

	uint32_t seconds() {
		return m_tv.tv_sec + m_tv.tv_usec / 1000000;
	}

	Time end_of_day() {
		struct tm * data;
		time_t new_time = seconds();
		data = localtime(&new_time);
    	data->tm_hour = 0;
    	data->tm_min = 0;
    	data->tm_sec = 0;
    	return Time(mktime(data)+ 24*3600, 0);		
	}
	
	Time end_of_week() {
		struct tm * data;
		time_t new_time = seconds();
		data = localtime(&new_time);
    	data->tm_hour = 0;
    	data->tm_min = 0;
    	data->tm_sec = 0;
		uint32_t append_time=0;
		if(data->tm_wday == 0) {
			append_time += 24*60*60;
		} else {
			append_time += (8-data->tm_wday)* 24*60*60;
		}
    	return Time(mktime(data) + append_time, 0);		
	}

	Time start_of_day() {
		struct tm * data;
		time_t new_time = seconds();
		data = localtime(&new_time);
    	data->tm_hour = 0;
    	data->tm_min = 0;
    	data->tm_sec = 0;
    	return Time(mktime(data),0);		
	}

	uint32_t WeekDay() {
		struct tm * data;
		time_t new_time = seconds();
		data = localtime(&new_time);
		return data->tm_wday;
	}	

	uint32_t MonthDayIndex() {
		struct tm * data;
		time_t new_time = seconds();
		data = localtime(&new_time);
		return data->tm_mday;
	}
	
	bool time_is_between(const Time& earlier, const Time& later) {
		if (earlier <= later) {
			return ( (earlier <= *this) && (*this <= later) );
		} else {
			return ( (later <= *this) && (*this <= earlier) ); 
		}
	}
	
public:
	
	Time& operator+(int second) {
		m_tv.tv_sec +=second;
		return *this;
	}

	const Time& operator =(const timeval& tv)
    {
        m_tv = tv;
        return *this;
    }

private:
    timeval m_tv;

public:
	friend bool operator <(const Time& t1, const Time& t2);
    friend bool operator ==(const Time& t1, const Time& t2);
    friend bool operator <=(const Time& t1, const Time& t2);
};

inline bool operator <(const Time& t1, const Time& t2)
{
    return (t1.m_tv.tv_sec < t2.m_tv.tv_sec) || (t1.m_tv.tv_sec == t2.m_tv.tv_sec && t1.m_tv.tv_usec < t2.m_tv.tv_usec);
}

inline bool operator ==(const Time& t1, const Time& t2)
{
    return (t1.m_tv.tv_sec == t2.m_tv.tv_sec && t1.m_tv.tv_usec == t2.m_tv.tv_usec);
}

inline bool operator <=(const Time& t1, const Time& t2)
{
    return (t1 < t2) || (t1 == t2);
}

}

#endif