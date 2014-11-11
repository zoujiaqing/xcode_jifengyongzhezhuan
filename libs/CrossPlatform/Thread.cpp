#include "Thread.h"
#include <stdio.h>

int Thread::threadInitNumber = 1;
int Thread::getNextThreadNum()
{    
    return threadInitNumber++;
}

#ifndef WIN32
void* Thread::run0(void* pVoid)
{
    Thread* p = (Thread*) pVoid;
    p->run1();
    return p;
}
#else
DWORD Thread::run0(LPVOID lpParameter)
{
    Thread* p = (Thread*) lpParameter;
    p->run1();
	return 0;
}
#endif

void Thread::run1()
{
    threadStatus = THREAD_STATUS_RUNNING;
    tid = pthread_self();
    run();
    threadStatus = THREAD_STATUS_EXIT;
    tid = 0;
	//by benyang
#ifndef WIN32
	pthread_exit(NULL);
#else
	int exit_code = 0;
	pthread_exit(&exit_code);
#endif
}

void Thread::run()
{
    if (target != NULL)
    {
        (*target).run();
    }
}

Thread::Thread()
{
    tid = 0;
    threadStatus = THREAD_STATUS_NEW;
    curThreadInitNumber = getNextThreadNum();
    pthread_attr_init(&attr);
}

Thread::Thread(Runnable *iTarget)
{
    target = iTarget;
    tid = 0;
    threadStatus = THREAD_STATUS_NEW;
    curThreadInitNumber = getNextThreadNum();
    pthread_attr_init(&attr);
}

Thread::~Thread()
{
    
    pthread_attr_destroy(&attr);
    //by Stanley
#ifdef __APPLE__
    pthread_cancel(tid);
#endif
//    pthread_exit((void*)3);
}

void Thread::cancelThread()
{
    //by Stanley
#ifdef __APPLE__
    pthread_cancel(tid);
#endif
}
bool Thread::start()
{
#ifndef WIN32
    pthread_create(&tid, &attr, Thread::run0, this);
#else
	tid = CreateThread(NULL, NULL, run0, this, NULL, NULL);
#endif
    
    
    return pthread_detach(tid) ==0;
}

pthread_t Thread::getCurrentThreadID()
{
    return pthread_self();
}

pthread_t Thread::getThreadID()
{
    return tid;
}

int Thread::getState()
{
    return threadStatus;
}

void Thread::join()
{
    if (tid > 0)
    {
        pthread_join(tid, NULL);
    }
}

void Thread::join(unsigned long millisTime)
{
    if (tid == 0)
    {
        return;
    }
    if (millisTime == 0)
    {
        join();
    }else
    {
        unsigned long k = 0;
        while (threadStatus != THREAD_STATUS_EXIT && k <= millisTime)
        {
            usleep(100);
            k++;
        }
    }
}

bool Thread::operator ==(const Thread *otherThread)
{
    if (otherThread == NULL)
    {
        return false;
    }
    if (curThreadInitNumber == (*otherThread).curThreadInitNumber)
    {
        return true;
    }
    return false;
}

bool Thread::isEquals(Thread *iTarget)
{
    if (iTarget == NULL)
    {
        return false;
    }
    return pthread_self() == iTarget->tid;
}

void Thread::setThreadScope(bool isSystem)
{
    if (isSystem)
    {
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    }
    else
    {
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
    }
}

bool Thread::getThreadScope()
{
    int scopeType = 0;
    //by Stanley
#ifdef __APPLE__
    pthread_attr_getscope(&attr, &scopeType);
#else
    pthread_attr_getscope(&attr);
#endif
    return scopeType == PTHREAD_SCOPE_SYSTEM;
}

void Thread::setThreadPriority(int priority)
{
#ifndef WIN32
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority= priority;
  
    pthread_attr_setschedparam(&attr, &param);
#else
	// not very secure to use this func
	int level = priority % 5 - 2;
	SetThreadPriority(tid, level);
#endif
}

int Thread::getThreadPriority()
{
#ifndef WIN32
    pthread_attr_getschedparam(&attr, &param);
    return param.sched_priority;
#else
	// not very secure to use this func
	return GetThreadPriority(tid) + 2;
#endif
}