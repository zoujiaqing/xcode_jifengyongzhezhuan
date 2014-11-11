#ifndef DETECT_H
#define DETECT_H
#include <fstream>
#include <sstream>
#include <string>
#ifdef WIN32
#include <windows.h>
#include <stdint.h>
#else
#include <sys/time.h>
#endif

#include <time.h>
#include <assert.h>

using namespace std;

namespace det{


template < class type > class Singleton //保证只有一个实例化对象
{
    public:
        virtual ~Singleton( ) {
            m_singleton = 0;
        }

        //static type & get_singleton( ) { ASSERT( m_singleton ); return *m_singleton; }
        static type & get_singleton( ) { if(m_singleton == NULL) new type(); return *m_singleton; }
        static type * get_singleton_ptr( ) { if(m_singleton == NULL) new type();return m_singleton; }

    protected:
        Singleton( ) {
            /// If you hit this ASSERT, this singleton already exists -- you can't create another one!
            assert( m_singleton == 0 );
            m_singleton = static_cast<type *>(this);//this在使用时是指向子类的
        }

        /// Singleton pointer, must be set to 0 prior to creating the object
        static type * m_singleton;
};
template <class type> type * Singleton < type > :: m_singleton = 0;



#ifdef WIN32

class Time{
public:
	static uint32_t mseconds(){
		return GetTickCount();
	}

	static  clock_t cpusec(){
		return clock();
	}

	static uint32_t seconds(){
		time_t clock;
		struct tm tm;
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		tm.tm_year     = wtm.wYear - 1900;
		tm.tm_mon     = wtm.wMonth - 1;
		tm.tm_mday     = wtm.wDay;
		tm.tm_hour     = wtm.wHour;
		tm.tm_min     = wtm.wMinute;
		tm.tm_sec     = wtm.wSecond;
		tm. tm_isdst    = -1;
		clock = mktime(&tm);
		return clock;
								    
	}	
	static uint64_t useconds(){
    	LARGE_INTEGER utime,feq;
		QueryPerformanceFrequency(&feq);
		QueryPerformanceCounter(&utime);
		uint64_t temp = (utime.QuadPart/feq.QuadPart);
		temp = temp * 1000000 + (utime.QuadPart - temp * feq.QuadPart)*1000000/feq.QuadPart;
		return temp;
	}

};

#else

class Time{
public:
	static uint32_t seconds(){
		struct timeval tv;
  		gettimeofday(&tv, 0);
    	return tv.tv_sec + tv.tv_usec / 1000000;
	}

	static  clock_t cpusec(){
		return clock(); 
	}

	static uint32_t mseconds(){
		struct timeval tv;
  		gettimeofday(&tv, 0);
    	return tv.tv_sec*1000 + tv.tv_usec / 1000;
	}

	static uint64_t useconds(){
		struct timeval tv;
  		gettimeofday(&tv, 0);
    	return tv.tv_sec * 1000000 + tv.tv_usec;
	}

};

#endif //WIN32

class BuildData : public Singleton<BuildData>
{
public:
	BuildData() : split('#'){

	}
	~BuildData(){
	}

	std::string input(std::string cmd,uint32_t len){
		ss.clear();
		ss.str("");
		ss<<Time::seconds()<<split<<cmd<<split<<len;
		return ss.str();
	}

	std::string input(uint32_t cmd,uint32_t len){
		ss.clear();
		ss.str("");
		ss<<Time::seconds()<<split<<cmd<<split<<len;
		return ss.str();

	}

	std::string input(uint32_t time,std::string cmd,uint64_t len){
		ss.clear();
        ss.str("");
		ss<<time<<split<<cmd<<split<<len;
		return ss.str();
	}

	std::string input(uint32_t time,std::string cmd, clock_t len,uint64_t sec){
		ss.clear();
		ss.str("");
		ss<<time<<split<<cmd<<split<<(double)len/(double)sec<<std::endl;
		return ss.str();
	}
private:
	char split;
	std::stringstream ss;

};


#define build det::BuildData::get_singleton()

class Detect{
public:
	Detect(std::string path){
		fout.open(path.c_str(),ios::app);
	}

	~Detect(){
		fout.close();
	}

	void input(std::string cmd,uint32_t len){
			fout<<Time::seconds()<<"#"<<cmd<<"#"<<len<<std::endl;
	}

	void input(uint32_t cmd,uint32_t len){
			fout<<Time::seconds()<<"#"<<cmd<<"#"<<len<<std::endl;

	}

	void input(uint32_t time,std::string cmd,uint64_t len){
			fout<<time<<"#"<<cmd<<"#"<<len<<std::endl;
	}
	
	/*void input(uint32_t time,std::string cmd, clock_t len){
			fout<<time<<"#"<<cmd<<"#"<<len<<std::endl;
	}*/

	void input(uint32_t time,std::string cmd, clock_t len,uint64_t sec){
			fout<<time<<"#"<<cmd<<"#"<<(double)len/(double)sec<<std::endl;
	}
	
	void input(std::string data){
		fout<<data<<endl;
	}

private :
	std::ofstream fout;

};


class RDetect : public Singleton<RDetect>{
public :
	void input(std::string cmd,uint32_t len){
		detect.input(cmd,len);
	}

	void input(uint32_t cmd,uint32_t len){
		detect.input(cmd,len);
	}
	RDetect():detect("rtest"){

	}

private :
	Detect detect;
};

class SDetect : public Singleton<SDetect>{
public :
	void input(std::string cmd,uint32_t len){
		detect.input(cmd,len);
	}

	void input(uint32_t cmd,uint32_t len){
		detect.input(cmd,len);

	}
	SDetect():detect("stest"){

	}

private :
	Detect detect;
};


/*
 *
 *时间是时钟
 *CLOCKS_PER_SEC 
 *
 */

class TimeSpend : public Singleton<TimeSpend>{
public:
	TimeSpend() : detect("ttest"){

	}

	void time_begin(){
		start = Time::seconds();
		begin = Time::cpusec();
	}

	
	void time_end(std::string cmd){
		clock_t end = Time::cpusec();

		detect.input(start,cmd,(clock_t)(end -begin));
	}

	void time_end_second(std::string cmd,uint64_t sec)
	{
		clock_t end = Time::cpusec();
		detect.input(start,cmd,(clock_t)(end -begin),sec);
	}

private:
	clock_t begin;
	uint32_t start;
	Detect detect;
};

/*
 *
 *时间单位是us
 *
 */
class TimeUSpend : public Singleton<TimeUSpend>{
public:
	TimeUSpend() : detect("utest"){

	}

	void time_begin(){
		start = Time::seconds();
		begin = Time::useconds();
	}

	
	void time_end(std::string cmd){
		uint64_t end = Time::useconds();
		detect.input(start,cmd,(uint64_t)(end - begin));
	}
	
	std::string get_time(std::string cmd){
		uint64_t end = Time::useconds();
		return build.input(start,cmd,(uint64_t)(end - begin));
	}


private:
	uint32_t start;
	uint64_t begin;
	Detect detect;
};

class SpaceSpend : public Singleton<SpaceSpend>{
public:
	SpaceSpend() : detect("spacetest"),_size(0){

	}

	void size_start(int p)
	{
		//uint32_t size;
		std::ostringstream ss;
		ss<<"/proc/"<<p<<"/statm";
		std::ifstream fin;
		fin.open(ss.str().c_str());
		//for(int i=0;i<7;i++)
		//{
		//	fin>>size;
		//	ss<<size<<" ";
		//}
		fin>>_size;
		fin>>_size;
		fin.close();
		//detect.input(base::Time().seconds(),ss.str().c_str(),_size);
	}

	void size_end(string cmd,int p)
	{
		uint32_t size;
		std::ostringstream ss;
		ss<<"/proc/"<<p<<"/statm";
		std::ifstream fin;
		fin.open(ss.str().c_str());
		//for(int i=0;i<7;i++)
		//{
		//	fin>>size;
		//	ss<<size<<" ";
		//}
		fin>>size;
		fin>>size;
		fin.close();
	//	ss<<cmd<<" ";
		//detect.input(base::Time().seconds(),ss.str().c_str(),_size);
		//detect.input(base::Time().seconds(),cmd,size - _size);

	}

	void size_add(uint32_t size){
		//_size += size;
	}

	
	void size_end(std::string cmd){
		//detect.input(base::Time().seconds(),cmd,_size);
		//_size = 0;
	}


private:
	Detect detect;
	uint32_t _size;
};


}

#define rDetect det::RDetect::get_singleton()
#define sDetect det::SDetect::get_singleton()
#define TIMESPEND det::TimeSpend::get_singleton()
#define TIMEUSPEND det::TimeUSpend::get_singleton()
#define SPACE det::SpaceSpend::get_singleton()


#endif //DETECT_H

