#include "util.h"
#include "scheduler.h"
#include<pthread.h>
#include<unistd.h>

namespace sched{
  void* normalExec(Task *task){
    func f=task->f;
    void* arg=task->arg;
    return f(arg);
  }

  void* delayExec(Task *task){
    if(!task->delay){
      return (void*)-1;
    }
    sleep(task->delay);
    return normalExec(task);
  }

  void* circleExec(Task *task){
    if(!task->interval){
      return (void*)-1;
    }
    if(!task->delay){
      sleep(task->delay);
    }
    while(1){
      normalExec(task);
      sleep(task->interval);
    }
    return (void*)-1;
  }

  void* taskExec(void *arg){
    void* res;
    Task *t=(Task*)arg;
    switch(t->type){
      case NORMAL:
        res=normalExec(t);
        break;
      case DELAY:
        res=delayExec(t);
        break;
      case CIRCLE:
        res=circleExec(t);
        break;
      default:
        res=(void*)-1;
    }
    free(t);//回收内存
    return res;
  }

  int ThreadWrapper::startThread(func f, void* arg){
    int err=-1;
    pthread_t tid;
    err = pthread_create(&tid,NULL,f,arg);
    if(err != 0){
      return err;
    }
    return 0;
  }

  int TaskWrapper::startTask(Task *task){
    int err=-1;
    pthread_t tid;
    err = pthread_create(&tid,NULL,taskExec,(void*)task);
    if(err != 0){
      return err;
    }
    return err;
  }
}
