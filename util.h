#ifndef _SCHEDUTIL_
#define _SCHEDUTIL_

#include"scheduler.h"

namespace sched{

  int normalExec(Task *task);
  int delayExec(Task *task);
  int circleExec(Task *task);

  class TaskWrapper{
  public:
    static int startTask(Task *task);
  };

}

#endif
