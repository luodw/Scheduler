#include"scheduler.h"
#include"util.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

namespace sched{

  void defaultRejectHandler(void* arg){
    printf("the numTask is up to max\n");
  }

  Scheduler::Scheduler(uint numThread, uint maxTask, rejectFunc rf):numThread(numThread),
      maxTask(maxTask),currentCount(0),rejectHandler(rf){
    pthread_mutex_init(&queueMutex,NULL);
    pthread_cond_init(&queueCond,NULL);
  }

  Scheduler::Scheduler(uint numThread, uint maxTask):numThread(numThread),
      maxTask(maxTask),currentCount(0),rejectHandler(defaultRejectHandler){
    pthread_mutex_init(&queueMutex,NULL);
    pthread_cond_init(&queueCond,NULL);
  }

  void* Scheduler::scheduleLoop(void *arg){
    Scheduler *scheduler=(Scheduler*)arg;
    while(1){
      pthread_mutex_lock(&scheduler->queueMutex);
      if(scheduler->taskQueue.empty()){
        pthread_cond_wait(&scheduler->queueCond,&scheduler->queueMutex);
      }
      Task* task=scheduler->taskQueue.front();
      scheduler->taskQueue.pop();
      scheduler->decrCount();
      pthread_mutex_unlock(&scheduler->queueMutex);
      int res = TaskWrapper::startTask(task);
      if(res!=0){
        fprintf(stderr,"[Scheduler::ScheduleLoop] pthread_create error!\n");
        pthread_exit((void*)-1);
      }
    }
    return (void*)0;
  }

  int Scheduler::schedule(func f,void *arg,int delay,int interval){
    if(f == NULL){
      fprintf(stderr,"[Scheduler::Schedule] schedule error, func can not be NULL!\n");
      return -1;
    }
    if(currentCount >= maxTask){
      printf("up to limit %ld\n",(long)arg);
      rejectHandler((void*)1);
      return 0;
    }

    pthread_mutex_lock(&queueMutex);
    taskQueue.push(new Task(f,arg,delay,interval));
    currentCount++;
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&queueCond);
    return 0;
  }

  int Scheduler::startup(){
    int err;
    pthread_t t;
    for(int i=0;i<numThread;i++){
      err = pthread_create(&t,NULL,Scheduler::scheduleLoop,(void*)this);
      if(err != 0){
        fprintf(stderr,"[Scheduler::Startup] pthread_create error!\n");
        return -1;
      }
      tidVector.push_back(t);
    }
    printf("scheduler startup successfully!\n");
    return 0;
  }

  void Scheduler::decrCount(){
    printf("currentCount=%d\n",currentCount);
    currentCount--;
  }

  int Scheduler::stopAll(){
    int err;
    for(std::vector<pthread_t>::iterator iter=tidVector.begin();iter!=tidVector.end();iter++){
      err = pthread_cancel(*iter);
      if(err!=0){
        fprintf(stderr, "%s\n","pthread_cancel error!" );
        return -1;
      }
    }
    return 0;
  }

}
