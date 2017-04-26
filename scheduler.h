#ifndef _SCHEDULER_
#define _SCHEDULER_
#include<queue>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

namespace sched{

  #define NORMAL 0
  #define DELAY 1
  #define CIRCLE 2

  typedef void*(*func)(void*);
  typedef void(*rejectFunc)(void*);

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
    Scheduler(uint numThread,uint maxTask);
    Scheduler(uint numThread,uint maxTask,rejectFunc rf);
    int startup();
    int schedule(func f,void *arg,int delay,int interval);
    int stopAll();
    void decrCount();
  private:
    static void* scheduleLoop(void* arg);
    Scheduler& operator=(const Scheduler&);
    Scheduler(const Scheduler&);
  private:
    uint currentCount;
    uint maxTask;
    uint numThread;
    rejectFunc rejectHandler;
    pthread_mutex_t queueMutex;
    pthread_cond_t queueCond;
    std::vector<pthread_t> tidVector;
    std::queue<Task*> taskQueue;
  };
}

#endif
