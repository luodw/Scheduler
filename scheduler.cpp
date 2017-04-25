#include"scheduler.h"
#include"util.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

namespace sched{
  Scheduler::Scheduler(uint maxTask):maxTask(maxTask),count(0){
    pthread_mutex_init(&queueMutex,NULL);
    pthread_cond_init(&queueCond,NULL);
  }

  void* Scheduler::scheduleLoop(void *arg){
    pthread_t tid;
    Scheduler *scheduler=(Scheduler*)arg;
    while(1){
      pthread_mutex_lock(&scheduler->queueMutex);
      if(scheduler->taskQueue.empty()){
        pthread_cond_wait(&scheduler->queueCond,&scheduler->queueMutex);
      }
      Task* task=scheduler->taskQueue.front();
      scheduler->taskQueue.pop();
      pthread_mutex_unlock(&scheduler->queueMutex);
      int res = TaskWrapper::startTask(task);
      if(res!=0){
        fprintf(stderr,"[Scheduler::ScheduleLoop] pthread_create error!\n");
        pthread_exit((void*)-1);
      }
      scheduler->count--;
    }
    return (void*)0;
  }

  int Scheduler::schedule(func f,void *arg,int delay,int interval){
    if(f == NULL){
      fprintf(stderr,"[Scheduler::Schedule] schedule error, func can not be NULL!\n");
      return -1;
    }
    if(count == maxTask){
      fprintf(stderr,"[Scheduler::Schedule] schedule error, task count is up to maxTask!\n");
      return -1;
    }

    pthread_mutex_lock(&queueMutex);
    taskQueue.push(new Task(f,arg,delay,interval));
    if(taskQueue.size()==1){
      pthread_cond_signal(&queueCond);
    }
    pthread_mutex_unlock(&queueMutex);
    count++;
    return 0;
  }

  int Scheduler::startup(){
    int err;
    pthread_t t;
    err = ThreadWrapper::startThread(Scheduler::scheduleLoop,(void*)this);
    if(err != 0){
      fprintf(stderr,"[Scheduler::Startup] pthread_create error!\n");
      return -1;
    }
    printf("scheduler startup successfully!\n");
    return 0;
  }
}
