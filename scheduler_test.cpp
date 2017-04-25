#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"scheduler.h"

void* testNormal(void *arg){
    printf("test normal %ld\n",(long)arg);
}

void* testDelay(void *arg){
    printf("test delay %ld\n",(long)arg);
}

void* testCircle(void *arg){
    printf("testCircle %ld\n",(long)arg);
}

int main(void){
  sched::Scheduler *s=new sched::Scheduler(5);
  int res=s->startup();
  if(res!=0){
    fprintf(stderr,"Scheduler startup error\n");
    exit(-1);
  }
  int err = s->schedule(testNormal,(void*)1,0,0);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  err = s->schedule(testDelay,(void*)2,2,0);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  err = s->schedule(testCircle,(void*)3,0,5);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  pause();
  return 0;
}
