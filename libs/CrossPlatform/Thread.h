/*  
 * File:  Thread.h 
 * Author: Null 
 * 
 * 
 * 在编译的时候记得加上参数：-lpthread 
 *  
 */
#ifndef _THREAD_H
#define _THREAD_H

#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "Win32Platform/thread_win32.h"
#endif
/* 
 * 线程运行实体类 
 */
class Runnable
{
public:    
    //运行实体    
    virtual void run() = 0;
};
/* 
 * 线程类 
 * 
 */
class Thread : public Runnable
{
private:      
    static int threadInitNumber; //线程初始化序号    
    
    int curThreadInitNumber;//当前线程初始化序号  
    
    Runnable *target;//线程体    
    
    pthread_t tid;//当前线程的线程ID     
    
    int threadStatus;//线程的状态    
    
    pthread_attr_t attr;//线程属性    
#ifndef WIN32  
    sched_param param; //线程优先级   
#endif
  
#ifndef WIN32
    static void* run0(void* pVoid); //获取执行方法的指针   
#else
	static DWORD WINAPI run0(LPVOID lpParameter);
#endif
    
    void run1(); //内部执行方法    
    
    static int getNextThreadNum();//获取一个线程序号
    
public:    
    
    static const int THREAD_STATUS_NEW = 0;  //线程的状态－新建  
    
    static const int THREAD_STATUS_RUNNING = 1; //线程的状态－正在运行   
    
    static const int THREAD_STATUS_EXIT = -1; //线程的状态－运行结束   
    
    Thread();   //构造函数 
    
    Thread(Runnable *iTarget); //构造函数    
    
    ~Thread(); //析构    
    
    void run(); //线程的运行实体   
    
    bool start();  //开始执行线程  
    
    int getState(); //获取线程状态   
    
    void join(); //等待线程直至退出   
    
    void join(unsigned long millisTime);//等待线程退出或者超时    
    
    bool operator ==(const Thread *otherThread); //比较两个线程时候相同,通过 curThreadInitNumber 判断   
    
    pthread_t getThreadID(); //获取This线程ID    
    
    static pthread_t getCurrentThreadID(); //获取当前线程ID   
    
    static bool isEquals(Thread *iTarget); //当前线程是否和某个线程相等,通过 tid 判断   
    
    void setThreadScope(bool isSystem);//设置线程的类型:绑定/非绑定    
    
    bool getThreadScope();  //获取线程的类型:绑定/非绑定  
    
    void setThreadPriority(int priority); //设置线程的优先级,1-99,其中99为实时;意外的为普通   
    
    int getThreadPriority();//获取线程的优先级 
    
    void cancelThread();
};

#endif /* _THREAD_H */