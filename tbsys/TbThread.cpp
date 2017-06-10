/*
 * (C) 2007-2010 Taobao Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id$
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *
 */

#include <climits>
#include <exception>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <bits/time.h>
#include "TbThread.h"
#include "Time.h"
#include "ThreadException.h"
#include "Cond.h"
#include "PublicDefine.h"

using namespace std;
namespace tbutil
{
Thread::Thread() :
    _running(false),
    _started(false),
    _detachable(false),
    _thread(0)
{
}

Thread::~Thread()
{
}

extern "C"
{
static void *startHook(void *arg)
{
  ThreadPtr thread;
  try
  {
    Thread *rawThread = static_cast<Thread *>(arg);
    thread = rawThread;
    rawThread->__decRef();
    thread->run();
  }
  catch (...)
  {
    //terminate the thread when error occurred
    std::terminate();
  }
  //thread is done now
  thread->_done();
  return 0;
}
}

int Thread::start(size_t stackSize)
{
  ThreadPtr keepMe = this;
  Mutex::Lock sync(_mutex);

  if (_started)
  {
#ifdef _NO_EXCEPTION
    JUST_RETURN(_started == true, -1);
    TBSYS_LOG(ERROR, "%s", "ThreadStartedException");
#else
    throw ThreadStartedException(__FILE__, __LINE__);
#endif
  }

  __incRef();

  if (stackSize > 0)
  {
    pthread_attr_t attr;
    int rt = pthread_attr_init(&attr);
#ifdef _NO_EXCEPTION
    if (0 != rt)
    {
      __decRef();
      TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
      return -1;
    }
#else
    if (0!=rt) {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif

    if (stackSize < PTHREAD_STACK_MIN)
    {
      stackSize = PTHREAD_STACK_MIN;
    }

    rt = pthread_attr_setstacksize(&attr, stackSize);
#ifdef _NO_EXCEPTION
    if (0 != rt)
    {
      __decRef();
      TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
      return -1;
    }
#else
    if (0!=rt) {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif
    // 首先尝试使用attr属性来创建线程
    rt = pthread_create(&_thread, &attr, startHook, this);
#ifdef _NO_EXCEPTION
    if (0 != rt)
    {
      __decRef();
      TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
      return -1;
    }
#else
    if (0!=rt) {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif
  }
    //创建失败使用默认模式创建线程
  else
  {
    const int rt = pthread_create(&_thread, 0, startHook, this);
#ifdef _NO_EXCEPTION
    if (0 != rt)
    {
      __decRef();
      TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
      return -1;
    }
#else
    if (0!=rt) {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif
  }
  //if thread is detachable, detatch the thread
  if (_detachable)
  {
    detach();
  }
  _started = true;
  _running = true;
  return 0;
}

bool Thread::isAlive() const
{
  return _running;
}

//set mark, thread is done, set _running to false
void Thread::_done()
{
  Mutex::Lock lock(_mutex);
  _running = false;
}

int Thread::join()
{
  // 线程已经分离, 不能够join
  if (_detachable)
  {
#ifdef _NO_EXCEPTION
    TBSYS_LOG(ERROR, "%s", "BadThreadControlException");
    JUST_RETURN(_detachable == true, -1);
#else
    throw BadThreadControlException(__FILE__, __LINE__);
#endif
  }
  // join the thread
  const int rt = pthread_join(_thread, NULL);
#ifdef _NO_EXCEPTION
  if (0 != rt)
  {
    TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
    return -1;
  }
#else
  if (0!=rt) {
    throw ThreadSyscallException(__FILE__, __LINE__, rt);
  }
#endif
  return 0;
}
// 这里使用_NO_EXCEOTION宏定义来判断是否允许exception, 这样就能够实现允许异常和不允许异常的快速切换
int Thread::detach()
{
  if (!_detachable)
  {
#ifdef _NO_EXCEPTION
    TBSYS_LOG(ERROR, "%s", "BadThreadControlException");
    JUST_RETURN(_detachable == false, -1);
#else
    throw BadThreadControlException(__FILE__, __LINE__);
#endif
  }
  // detatch the thread
  const int rt = pthread_detach(_thread);
#ifdef _NO_EXCEPTION
  if (0 != rt)
  {
    TBSYS_LOG(ERROR, "%s", "ThreadSyscallException");
    return -1;
  }
#else
  if (0!=rt) {
    throw ThreadSyscallException(__FILE__, __LINE__, rt);
  }
#endif
  return 0;
}

// return thread id
pthread_t Thread::id() const
{
  return _thread;;
}

void Thread::ssleep(const tbutil::Time &timeout)
{
  struct timeval tv = timeout;
  struct timespec ts;
  ts.tv_sec = tv.tv_sec;
  //convert from us to ns unit
  ts.tv_nsec = tv.tv_usec * 1000L;
  nanosleep(&ts, 0);
}

void Thread::yield()
{
  /* Yield the processor.  */
  sched_yield();
}
}//end namespace tbutil
