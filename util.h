#ifndef _SCHEDUTIL_
#define _SCHEDUTIL_

#include"scheduler.h"

namespace sched{

  void* normalExec(Task *task);
  void* delayExec(Task *task);
  void* circleExec(Task *task);

  void* threadExec(void *arg);
  void* taskExec(void* arg);

  class ThreadWrapper{
  public:
    static int startThread(func f,void* arg);
  };

  class TaskWrapper{
  public:
    static int startTask(Task *task);
  };

}

#endif
