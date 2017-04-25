#ifndef _SCHEDULER_
#define _SCHEDULER_
#include<queue>
#include<pthread.h>
#include<stdlib.h>

namespace sched{

  #define NORMAL 0
  #define DELAY 1
  #define CIRCLE 2

  typedef void*(*func)(void*);

  struct Task{
    int type;
    int delay;
    int interval;
    void* arg;
    func f;
    Task(func f,void* arg,int delay,int interval):f(f),arg(arg),delay(delay),interval(interval){
      if(delay==0 && interval==0){
        type=NORMAL;
      }else if(delay!=0 && interval==0){
        type=DELAY;
      }else{
        type=CIRCLE;
      }
    }
  };

  class Scheduler{
  public:
    Scheduler(uint maxTask);
    int startup();
    int schedule(func f,void *arg,int delay,int interval);
  private:
    static void* scheduleLoop(void* arg);
  private:
    uint maxTask;
    uint count;
    pthread_mutex_t queueMutex;
    pthread_cond_t queueCond;
    std::queue<Task*> taskQueue;
  };
}

#endif
