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
  sched::Scheduler *s=new sched::Scheduler(4,2);
  int res=s->startup();
  if(res!=0){
    fprintf(stderr,"Scheduler startup error\n");
    exit(-1);
  }
  for(long i=0;i<6;i++){
    int err;
    if(i%2==0)
        err = s->schedule(testCircle,(void*)i,0,3);
    else
        err = s->schedule(testNormal,(void*)i,0,0);
    if(err!=0){
      fprintf(stderr,"Schedule error\n");
      exit(-1);
    }
  }
  pause();
  s->stopAll();
  printf("over\n");
  delete s;
  return 0;
}
