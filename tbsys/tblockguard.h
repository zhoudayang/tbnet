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

#ifndef TBSYS_LOCK_GUARD_H_
#define TBSYS_LOCK_GUARD_H_

// simple lock guard, a RAII tool
namespace tbsys
{
/**
 * @brief  CLockGuard是一个模板类，它需要CThreadMutex作为它的模板参数
 * 构造函数调用传入参数的lock方法,析构函数调用unlock方法
 */
template<class T>
class CLockGuard
{
 public:
  /// block 标明是否采用阻塞式加锁方式
  CLockGuard(const T &lock, bool block = true) : _lock(lock)
  {
    _acquired = !(block ? _lock.lock() : _lock.tryLock());
  }

  /// 析构函数，解锁
  ~CLockGuard()
  {
    _lock.unlock();
  }

  bool acquired() const
  {
    return _acquired;
  }

 private:
  const T &_lock;
  /// 没有获取到锁？
  mutable bool _acquired;
};
}
#endif
