#include "util.h"
#include "scheduler.h"
#include<pthread.h>
#include<unistd.h>

namespace sched{
  int normalExec(Task *task){
    func f=task->f;
    void* arg=task->arg;
    f(arg);
    return 0;
  }

  int delayExec(Task *task){
    if(!task->delay){
      return -1;
    }
    sleep(task->delay);
    return normalExec(task);
  }

  int circleExec(Task *task){
    if(!task->interval){
      return -1;
    }
    if(!task->delay){
      sleep(task->delay);
    }
    while(1){
      normalExec(task);
      sleep(task->interval);
    }
    return -1;
  }

  int TaskWrapper::startTask(Task *task){
    int res;
    switch(task->type){
      case NORMAL:
        res=normalExec(task);
        break;
      case DELAY:
        res=delayExec(task);
        break;
      case CIRCLE:
        res=circleExec(task);
        break;
      default:
        res=-1;
    }
    delete task;//回收内存
    return res;
  }
}
